/*
 * TCP.h
 *
 *  Created on: 21.10.2015
 *      Author: dominik
 */

#ifndef SRC_NETWORK_TCP_H_
#define SRC_NETWORK_TCP_H_

#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <functional>
#include <asio.hpp>


using asio::ip::tcp;

class Client
{
private:

	  tcp::resolver resolver_;
	  asio::ip::tcp::socket socket_;
	  asio::streambuf request_;
	  asio::streambuf response_;

	  void handle_resolve(const asio::error_code& err, tcp::resolver::iterator endpoint_iterator);


	  void handle_connect(const asio::error_code& err, tcp::resolver::iterator endpoint_iterator);

	  void handle_write(const asio::error_code& err);

	  void handle_read(const asio::error_code& err);

public:

  Client(asio::io_service& io_service, const std::string& server, const std::string& path);


};

int testTCP(int argc, char* argv[])
{
  try
  {
    if (argc != 3)
    {
      std::cout << "Usage: async_client <server> <path>\n";
      std::cout << "Example:\n";
      std::cout << "  async_client www.boost.org /LICENSE_1_0.txt\n";
      return 1;
    }

    asio::io_service io_service;
    Client c(io_service, argv[1], argv[2]);
    io_service.run();//std::thread thread([&]{io.run();});
  }
  catch (std::exception& e)
  {
    std::cout << "Exception: " << e.what() << "\n";
  }
}




#endif /* SRC_NETWORK_TCP_H_ */
