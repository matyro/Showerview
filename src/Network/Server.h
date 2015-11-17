/*
 * Server.h
 *
 *  Created on: 13.11.2015
 *      Author: dominik
 */

#ifndef SRC_NETWORK_SERVER_H_
#define SRC_NETWORK_SERVER_H_

#include "Network/Socket.h"

#include <functional>
#include <thread>
#include <atomic>
#include <mutex>

#include <iostream>

namespace network
{
	
	class Server
	{
	private:
		Socket m_arb_listenSock;

		std::vector<Socket> m_std_connectionSock;

		//Callbacks:
		std::function<void(unsigned int, std::vector<char>&&)> m_func_recv;
		std::function<bool (unsigned int, Socket*)> m_func_newConnection;		//If true -> socket gets saved else discarded


		std::thread* m_threadNewCon;
		std::thread* m_threadNewData;
		std::atomic<bool> m_isRunning;
		std::mutex m_vectorMutex;
		std::mutex m_functionMutex;


		void handleConnection();
		void handleRecv();

	public:

		Server(const unsigned short port);
		~Server();


		inline void setRecvFunc(std::function<void(unsigned int, std::vector<char>&&)> func){m_functionMutex.lock(); this-> m_func_recv = func; m_functionMutex.unlock();}
		inline void setNewConFunc(std::function<bool (unsigned int, Socket*)> func){m_functionMutex.lock(); this->m_func_newConnection = func; m_functionMutex.unlock();}

	};

} /* namespace network */

#endif /* SRC_NETWORK_SERVER_H_ */
