#ifndef SCS_TELEMETRY_TCP_SERVER_HPP
#define SCS_TELEMETRY_TCP_SERVER_HPP

#include <set>
#include <cstdio>
#include <iostream>
#include <thread>
#include <string>

#include <scssdk.h>

#if WIN32

#include <winsock.h>
#pragma comment(lib, "Ws2_32.lib")

#else

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <cerrno>
#include <unistd.h>

#endif

#define PORT 45454

class TcpServer {
public:
#ifdef WIN32
    std::set<SOCKET> *clients = nullptr;
    SOCKET srv = INVALID_SOCKET;
#else
    std::set<int> *clients = nullptr;
    int srv = -1;
#endif
    bool finish = false;

public:
    explicit TcpServer(scs_log_t logger_);
    ~TcpServer();

    bool init();
    void broadcast(const char *data, int dataSize) const;

private:
    void acceptLoop() const;
    void logInfo(const std::string& msg) const;
    void logWarn(const std::string& msg) const;
    void logErr(const std::string& msg) const;

    scs_log_t logger = nullptr;
};

#endif //SCS_TELEMETRY_TCP_SERVER_HPP
