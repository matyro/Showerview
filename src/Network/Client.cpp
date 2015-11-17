/*
 * Client.cpp
 *
 *  Created on: 13.11.2015
 *      Author: dominik
 */

#include "Client.h"

namespace network
{

	Client::Client(const std::string ip, const unsigned short port)
	{
		this->m_arb_connection.create();
		this->m_arb_connection.connect(ip, port);
	}

	void Client::send(const char* const data, const unsigned int len)
	{
		this->m_arb_connection.send(data,len);
	}

	std::vector<char> Client::recv()
	{
		return this->m_arb_connection.recv();
	}

} /* namespace network */
