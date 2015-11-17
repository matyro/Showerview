// Implementation of the Socket class.

#ifdef __linux__

#include "Network/Socket.h"

#include <errno.h>
#include <sys/socket.h>
#include <unistd.h>	// close, gethostname
#include <arpa/inet.h>	//INET_PTON

#include <cstring>
#include <iostream>

Socket::Socket()
		: m_sock(-1)
{

	memset(&m_addr, 0, sizeof(m_addr));

}

Socket::Socket(Socket&& rhs)
{
	this->m_addr = rhs.m_addr;
	memset(&rhs.m_addr,0,sizeof(sockaddr_in));

	this->m_sock = rhs.m_sock;
	rhs.m_sock = 0;
}

Socket::~Socket()
{
	if (this->is_valid()) ::close (m_sock);
}

bool Socket::create()
{
	m_sock = socket(AF_INET, SOCK_STREAM, 0);

	if (!is_valid()) return false;

	// TIME_WAIT - argh
	int on = 1;
	if (setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, (const char*) &on, sizeof(on)) == -1) return false;
	else return true;

}

bool Socket::bind(const unsigned short port)
{

	if (!is_valid())
	{
		return false;
	}

	m_addr.sin_family = AF_INET;
	m_addr.sin_addr.s_addr = INADDR_ANY;
	m_addr.sin_port = htons(port);

	int bind_return = ::bind(m_sock, (struct sockaddr *) &m_addr, sizeof(m_addr));

	if (bind_return == -1)
	{
		return false;
	}

	return true;
}

bool Socket::listen() const
{
	if (!is_valid())
	{
		return false;
	}

	int listen_return = ::listen(m_sock, 128);	// Second parameter = number of waiting clients in queue

	if (listen_return == -1)
	{
		return false;
	}

	return true;
}

bool Socket::accept(Socket& new_socket) const
{
	int addr_length = sizeof(sockaddr_in);
	new_socket.m_sock = ::accept(m_sock, (sockaddr *) &(new_socket.m_addr), (socklen_t *) &addr_length);

	if (new_socket.m_sock <= 0) return false;
	else return true;
}

bool Socket::send(const char* const data, unsigned int length) const
{
	int status = ::send(m_sock, data, length, MSG_NOSIGNAL);
	if (status == -1)
	{
		return false;
	}
	else
	{
		return true;
	}
}

std::vector<char> Socket::recv(bool noBlock) const
{
	std::vector<char> buf(1024);

	int flags = noBlock ? MSG_DONTWAIT : 0;
	int status = ::recv(m_sock, buf.data(), 1024, flags);

//	if(noBlock)
//	{
//		if(errno == EAGAIN || errno == EWOULDBLOCK)
//			return std::move( std::vector<char>(0) );
//	}

	if (status == -1)
	{
		if(errno != EAGAIN && errno != EWOULDBLOCK)
			std::cout << "status == -1   errno == " << errno << "  in Socket::recv\n";

		return std::vector<char>();
	}
	else if (status == 0)
	{
		return std::vector<char>();
	}
	else
	{
		buf.resize(status);
		return std::move(buf);
	}
}

bool Socket::connect(const std::string host, const int port)
{
	if (!is_valid()) return false;

	m_addr.sin_family = AF_INET;
	m_addr.sin_port = htons(port);

	int status = inet_pton(AF_INET, host.c_str(), &m_addr.sin_addr);

	if ( errno == EAFNOSUPPORT) return false;

	status = ::connect(m_sock, (sockaddr *) &m_addr, sizeof(m_addr));

	if (status == 0) return true;
	else return false;
}

bool Socket::is_valid() const
{
	return m_sock != 0;
}

std::string Socket::getIP() const
{
	std::stringstream sstr;


	char str[256];
	inet_ntop(AF_INET, &(m_addr.sin_addr), str, 256);

	sstr<<str;
	sstr << ":" << ntohs(m_addr.sin_port);

	return sstr.str();
}

#endif

