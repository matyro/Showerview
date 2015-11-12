#ifndef _BAACK_NETWORK_CLIENT_H_
#define _BAACK_NETWORK_CLIENT_H_

#include "lib/cppzmq.hpp"

#include <vector>

class Client
{
private:

	zmq::socket_t* socket;

	char id[255];

public:

	Client(zmq::context_t& context)
	{
		memset(id, '\0', 255);
		socket = new zmq::socket_t(context, zmq::socket_type::stream);
	}

	~Client()
	{
		delete socket;
	}

	void connect(const char* const ip)
	{
		socket->connect(ip);

		size_t size = 255;
		socket->getsockopt(ZMQ_IDENTITY,id, &size);

		int iID = *reinterpret_cast<int*>(&id[1]);
		std::cout << iID;
	}

	void recv()
	{
		zmq::message_t message;

		socket->recv(&message);

		std::cout << message.data();
	}

	bool recv_noblock()
	{

		zmq::message_t message;

		bool isEmpty = !socket->recv(&message, ZMQ_DONTWAIT);

		std::cout << message.data();

		return isEmpty;
	}

	void send(const char* const data, const unsigned int len)
	{
		if (len > 0)
		{
			zmq::message_t message(255 + len);

			memcpy(message.data(), id, 255);
			memcpy(message.data()+255, data, len);

			socket->send(message, ZMQ_SNDMORE);
		}

	}

	void close()
	{
		zmq::message_t empty(0);

		socket->send(empty, ZMQ_SNDMORE);
		socket->close();
	}

};

#endif
