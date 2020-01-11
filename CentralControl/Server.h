// Authors: Patrick Tinz and Janik Tinz

#ifndef SERVER_H
#define SERVER_H
#include "HttpConnection.h"

class Server {
public:
  Server(int p, const std::string &v)
      : port(p), verzeichnis(v),
        acceptor(ioService, boost::asio::ip::tcp::endpoint(
                                boost::asio::ip::tcp::v4(), port)) {}
      void run();

private:
  int port;
  const std::string verzeichnis;
  boost::asio::io_service ioService;
  boost::asio::ip::tcp::acceptor acceptor;
};

#endif
