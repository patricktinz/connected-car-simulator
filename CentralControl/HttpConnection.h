// Authors: Patrick Tinz and Janik Tinz

#include <cstddef>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include "RequestHandler.h"
#include "FileHandler.h"

#ifndef HTTPCONNECTION_H
#define HTTPCONNECTION_H

class HttpConnection {
public:
    HttpConnection(boost::asio::ip::tcp::acceptor &a,const std::string &v)
        : acceptor(a), socket(a.get_io_service()), directory(v) {}
    void accept();
    void operator()(); 

private:
    boost::asio::ip::tcp::acceptor &acceptor;
    boost::asio::ip::tcp::socket socket;
    std::string directory;
};
#endif
