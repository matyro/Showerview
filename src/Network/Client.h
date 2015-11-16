/*
 * Client.h
 *
 *  Created on: 13.11.2015
 *      Author: dominik
 */

#ifndef SRC_NETWORK_CLIENT_H_
#define SRC_NETWORK_CLIENT_H_

#include "Network/Socket.h"

#include <functional>
#include <string>

namespace network
{
	
	class Client
	{
	private:
		Socket m_arb_connection;

		//std::function<void(Client, unsigned int, std::vector<char>&&)> m_std_callback;

	public:

		Client(const std::string ip, const unsigned short port);

		void send(const char* const data, const unsigned int len);

		std::vector<char>&& recv();




	};

} /* namespace network */

#endif /* SRC_NETWORK_CLIENT_H_ */
