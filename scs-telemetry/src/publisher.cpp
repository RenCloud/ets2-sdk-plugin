#include "publisher.hpp"

Publisher::Publisher(uint16_t port) : context(1), socket(context, zmq::socket_type::pub)
{
	std::stringstream ss;
	ss << "tcp://*:" << std::to_string(port);
	std::string addr = ss.str();
	socket.bind(addr);

	log_line(SCS_LOG_TYPE_message, std::string("Initialized publisher at ").append(addr).c_str());
}

void Publisher::Close()
{
	socket.close();
	context.shutdown();
	context.close();
}

void Publisher::Send(topic topic, zmq::message_t& msg)
{
	socket.send(zmq::buffer(std::to_string(int(topic))), zmq::send_flags::sndmore);
	socket.send(msg, zmq::send_flags::none);
}
