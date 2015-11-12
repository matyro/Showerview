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
	virtual bool create();
	virtual bool bind(const int port);
	virtual bool listen() const;
	virtual bool accept(Socket&) const;

	// Client initialization
	virtual bool connect(const std::string host, const int port);

	// Data Transimission
	virtual bool send(char*, unsigned int) const;
	virtual std::vector<char>&& recv() const;

	virtual void set_non_blocking(const bool);

	virtual bool is_valid() const;


};
