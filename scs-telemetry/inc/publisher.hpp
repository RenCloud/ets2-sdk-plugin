#pragma once

#include <scs-telemetry-common.hpp>
#include <vector>
#include <zmq.hpp>
#include <thread>

class Publisher {
private:
	zmq::context_t context;
	zmq::socket_t socket;

public:
	Publisher(uint16_t port);

	void Close();

	void Send(topic topic, zmq::message_t& msg);
};