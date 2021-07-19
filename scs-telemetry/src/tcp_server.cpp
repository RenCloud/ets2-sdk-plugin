#include "tcp_server.hpp"

TcpServer::TcpServer(scs_log_t logger_) : logger(logger_) {
#ifdef WIN32
    clients = new std::set<SOCKET>;
#else
    clients = new std::set<int>;
#endif
}

TcpServer::~TcpServer() {
    finish = true;

#ifdef WIN32
    WSACancelBlockingCall();

    shutdown(srv, 2);

    for (auto sock: *clients) {
        closesocket(sock);
    }

    closesocket(srv);
#else
    shutdown(srv, SHUT_RDWR);

    for (auto sock: *clients){
        close(sock);
    }

    close(srv);
#endif

    delete clients;
}

bool TcpServer::init() {
    logInfo("Starting TCP server");

    sockaddr_in service{};
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = INADDR_ANY;
    service.sin_port = htons(PORT);

#ifdef WIN32
    WSADATA wsaData{};

    auto res = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (res != NO_ERROR) {
        logErr("WSAStartup error: " + std::to_string(res));
        return false;
    }

    srv = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (srv == INVALID_SOCKET) {
        logErr("Error creating socket: " + std::to_string(WSAGetLastError()));
        WSACleanup();
        return false;
    }

    if (bind(srv, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
        logErr("Error binding socket: " + std::to_string(WSAGetLastError()));
        closesocket(srv);
        WSACleanup();
        return false;
    }

    if (listen(srv, 5) == SOCKET_ERROR) {
        logErr("Error listening: " + std::to_string(WSAGetLastError()));
        closesocket(srv);
        WSACleanup();
        return false;
    }
#else
    srv = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (srv <= 0) {
        logErr("Error creating socket: " + std::to_string(errno));
        return false;
    }

    int opt = 1;
    if (setsockopt(srv, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        logErr("Error setting socket options: " + std::to_string(errno));
        close(srv);
        return false;
    }

    if (bind(srv, (struct sockaddr*)&service, sizeof(service)) < 0) {
        logErr("Error binding socket: " + std::to_string(errno));
        close(srv);
        return false;
    }

    if (listen(srv, 5) < 0) {
        logErr("Error listening: " + std::to_string(errno));
        close(srv);
        return false;
    }
#endif

    std::thread t(&TcpServer::acceptLoop, this);
    t.detach();

    return true;
}

void TcpServer::broadcast(const char *data, int dataSize) const {
#ifdef WIN32
    std::set<SOCKET> toRemove;
#else
    std::set<int> toRemove;
#endif

    for (auto sock: *clients) {
#ifdef WIN32
        if (send(sock, (char*)&dataSize, sizeof(dataSize), 0) == SOCKET_ERROR) {
            logWarn("Error sending: " + std::to_string(WSAGetLastError()));
            toRemove.insert(sock);
            continue;
        }

        if (send(sock, data, dataSize, 0) == SOCKET_ERROR) {
            logWarn("Error sending: " + std::to_string(WSAGetLastError()));
            toRemove.insert(sock);
        }
#else
        if (send(sock, (char*)&dataSize, sizeof(dataSize), MSG_NOSIGNAL) < 0) {
            logWarn("Error sending: " + std::to_string(errno));
            toRemove.insert(sock);
            continue;
        }

        if (send(sock, data, dataSize, MSG_NOSIGNAL) < 0) {
            logWarn("Error sending: " + std::to_string(errno));
            toRemove.insert(sock);
        }
#endif
    }

    if (!toRemove.empty()) {
        for (auto sock: toRemove) {
#ifdef WIN32
            closesocket(sock);
#else
            close(sock);
#endif

            clients->erase(sock);
        }
    }
}

void TcpServer::acceptLoop() const {
    while (!finish) {
#ifdef WIN32
        auto s = accept(srv, nullptr, nullptr);
        if (s == INVALID_SOCKET) {
            auto code = WSAGetLastError();
            if (code == WSAEINTR) {
                logInfo("Finishing accepting thread");
            } else {
                logWarn("Accept failed: " + std::to_string(code));
            }
            break;
        }
#else
      auto s = accept4(srv, nullptr, nullptr, SOCK_NONBLOCK);
        if (s < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        if (s < 0) {
            logErr("Accept failed: " + std::to_string(errno));
            break;
        }
#endif

        logInfo("Connection accepted");
        clients->insert(s);
    }
}

void TcpServer::logInfo(const std::string &msg) const {
    logger(SCS_LOG_TYPE_message, msg.c_str());
}

void TcpServer::logWarn(const std::string &msg) const {
    logger(SCS_LOG_TYPE_warning, msg.c_str());
}

void TcpServer::logErr(const std::string &msg) const {
    logger(SCS_LOG_TYPE_error, msg.c_str());
}
