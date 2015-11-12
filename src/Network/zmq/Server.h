#ifndef _BAACK_NETWORK_SERVER_H_
#define _BAACK_NETWORK_SERVER_H_

#include "lib/cppzmq.hpp"

#include <string>
#include <sstream>

class Server
{
private:

	zmq::socket_t* socket;

public:

	Server(zmq::context_t& context)
	{
		socket = new zmq::socket_t(context, zmq::socket_type::stream);


		socket->setsockopt<int>(ZMQ_LINGER, 0);			// Delete all pending messages after close()

		char id[255];
		int iID = 12345;
		memset(id, '\0', 255);
		memcpy(id, &iID, 4);
		id[6] = 'A';
		id[7] = 'B';
		id[8] = 'C';
		socket->setsockopt(ZMQ_IDENTITY,id,255);

		//int one = 1;
		//socket.setsockopt(ZMQ_STREAM_NOTIFY, &one, sizeof(one));	//RECV empty message at connect and disconnect [only in 4.1 earlyer version do this automatic]
	}

	~Server()
	{
		delete socket;
	}

	void bind(unsigned short port)
	{
		std::stringstream sstr;
		sstr << "tcp://*:" << port;

		socket->bind(sstr.str().c_str());

	}

	const unsigned int recv()
	{
		zmq::message_t id;
		zmq::message_t message;

		socket->recv(&id);

		socket->recv(&message);

		std::cout << message.data();

		if (id.size() != 4)
			return 0;
		else
			return *static_cast<const unsigned int*>(id.data());
	}

	bool recv_noblock()
	{

		zmq::message_t id;
		zmq::message_t message;

		bool isEmpty1 = !socket->recv(&id, ZMQ_DONTWAIT);
		bool isEmpty2 = !socket->recv(&message, ZMQ_DONTWAIT);

		std::cout << message.data();

		return isEmpty1 || isEmpty2;
	}

	void send(const char* const data, const unsigned int id, const unsigned int len)
	{
		if (len > 0)
		{
			zmq::message_t tmpID(4);
			zmq::message_t message(len);

			memcpy((void *) message.data(), data, len);
			memcpy((void *) tmpID.data(), &id, 4);

			socket->send(tmpID, ZMQ_SNDMORE);
			socket->send(message, ZMQ_SNDMORE);
		}
	}

	void close(const unsigned int id)
	{
		zmq::message_t empty(0);
		zmq::message_t tmpID(4);

		memcpy((void *) tmpID.data(), &id, 4);

		socket->send(tmpID, ZMQ_SNDMORE);
		socket->send(empty, ZMQ_SNDMORE);
	}

	void close()
	{
		socket->close();
	}
};

#endif
