// Authors: Patrick Tinz and Janik Tinz

#include <cstddef>
#include <ctime>
#include <cstring>
#include <string>
#include <boost/asio.hpp>
#include "HttpResponse.h"

using namespace std;

#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

class RequestHandler {
public:
    RequestHandler(boost::asio::ip::tcp::socket &s, const string &a): socket(s), request(a) 
    {
        //cout << "REQUEST=:" << a << '\n';
    }
    void process();
  
private:
    boost::asio::ip::tcp::socket &socket;
    string request;
};
#endif
