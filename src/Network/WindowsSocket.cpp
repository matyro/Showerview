// Implementation of the Socket class.

#ifdef _WIN32
{
#include "Socket.h"

#include <string.h>



	Socket::Socket()
	{



	}

	Socket::~Socket()
	{

	}

	bool Socket::create()
	{


	}

	bool Socket::bind ( const int port )
	{

	}

	bool Socket::listen() const
	{

	}

	bool Socket::accept ( Socket& new_socket ) const
	{

	}

	bool Socket::send ( const std::string s ) const
	{

	}

	int Socket::recv ( std::string& s ) const
	{

	}

	bool Socket::connect ( const std::string host, const int port )
	{

	}

	void Socket::set_non_blocking ( const bool b )
	{

	}
}

#endif
