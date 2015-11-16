/*
 * Server.cpp
 *
 *  Created on: 13.11.2015
 *      Author: dominik
 */

#include "Server.h"

namespace network
{

	void Server::handleConnection()
	{
		while (m_isRunning)
		{
			this->m_arb_listenSock.listen();

			Socket tmp;
			this->m_arb_listenSock.accept(tmp);

			if ( this->m_func_newConnection(m_std_connectionSock.size(), &tmp)  )
			{
				m_vectorMutex.lock();
				this->m_std_connectionSock.push_back(std::move(tmp));
				m_vectorMutex.unlock();
			}
		}
	}

	void Server::handleRecv()
	{
		while(m_isRunning)
		{

		}
	}

	std::vector<char>&& Server::recv(const unsigned int id)
	{
		m_vectorMutex.lock();

		auto data = m_std_connectionSock[id].recv(true);

		m_vectorMutex.unlock();

		return std::move(data);


	}

	Server::Server(const unsigned short port)
	{
		this->m_arb_listenSock.create();
		this->m_arb_listenSock.bind(port);

		m_isRunning = true;
		m_thread = new std::thread(&Server::handleConnection, this);
	}

	Server::~Server()
	{
		m_isRunning = false;
		this->m_thread->join();

		delete this->m_thread;
	}

} /* namespace network */
