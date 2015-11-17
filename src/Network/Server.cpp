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
		while (m_isRunning == true)
		{
			this->m_arb_listenSock.listen();

			Socket tmp;
			this->m_arb_listenSock.accept(tmp);

			if (this->m_func_newConnection(m_std_connectionSock.size(), &tmp))
			{
				m_vectorMutex.lock();
				this->m_std_connectionSock.emplace(std::make_pair(m_nextID.load(), std::move(tmp)));
				m_nextID++;
				m_vectorMutex.unlock();
			}
		}
	}

	void Server::handleRecv()
	{
		while (m_isRunning == true)
		{
			this->m_vectorMutex.lock();

			volatile const unsigned int size = this->m_std_connectionSock.size();

			for (auto itr = this->m_std_connectionSock.begin(); itr != this->m_std_connectionSock.end(); itr++)
			{
				std::vector<char> data = itr->second.recv(true);
				this->m_vectorMutex.unlock();

				if (data.size() > 0)
				{
					m_functionMutex.lock();
					m_func_recv(itr->first, std::move(data));
					m_functionMutex.unlock();
					break;
				}
				else
				{
					this->m_vectorMutex.lock();
					if (size != m_std_connectionSock.size()) break;
				}

			}

			this->m_vectorMutex.unlock();
			std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(1));
		}
	}

	Server::Server(const unsigned short port)
	{
		m_nextID = 1;

		this->m_arb_listenSock.create();
		this->m_arb_listenSock.bind(port);

		m_isRunning = true;
		m_threadNewCon = new std::thread(&Server::handleConnection, this);
		m_threadNewData = new std::thread(&Server::handleRecv, this);
	}

	Server::~Server()
	{
		m_isRunning = false;
		this->m_threadNewCon->join();
		this->m_threadNewData->join();

		delete this->m_threadNewCon;
		delete this->m_threadNewData;
	}

	bool Server::send(unsigned int id, std::vector<char>&& data)
	{
		this->m_vectorMutex.lock();
		try
		{
			this->m_std_connectionSock.at(id).send(data.data(), data.size());
			this->m_vectorMutex.unlock();
		}
		catch (std::out_of_range e)
		{
			this->m_vectorMutex.unlock();
			return false;
		}
		return true;

	}

} /* namespace network */
