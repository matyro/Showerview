#ifndef _BAACK_NETWORK_SOCKET_H_
#define _BAACK_NETWORK_SOCKET_H_

#include <netinet/in.h>


#include <string>
#include <sstream>
#include <vector>

class Socket
{
private:

	int m_sock;
	sockaddr_in m_addr;



public:
	Socket();
	Socket(const Socket& rhs) = delete;
	Socket(Socket&& rhs);

	virtual ~Socket();

	Socket& operator=(const Socket&) = delete;

	// Server initialization
	bool create();
	bool bind(const unsigned short port);
	bool listen() const;
	bool accept(Socket&) const;

	// Client initialization
	bool connect(const std::string host, const int port);

	// Data Transimission
	bool send(const char* const data, const unsigned int length) const;
	std::vector<char>&& recv(bool) const;	//Parameter (true) = unblocking

	bool is_valid() const;

	std::string getIP() const;
};

#endif
