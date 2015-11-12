#pragma once

class Socket
{
private:

	int m_sock;
	sockaddr_in m_addr;



public:
	Socket();
	virtual ~Socket();

	// Server initialization
	bool create();
	bool bind(const int port);
	bool listen() const;
	bool accept(Socket&) const;

	// Client initialization
	bool connect(const std::string host, const int port);

	// Data Transimission
	bool send(char*, unsigned int) const;
	std::vector<char>&& recv() const;

	bool is_valid() const;


};
