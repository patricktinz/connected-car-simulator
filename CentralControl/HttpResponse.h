// Authors: Patrick Tinz and Janik Tinz

#include <string>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

using namespace std;

#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H 

class HttpResponse {
public:
    HttpResponse(int st) : status(st) 
    {
        string row("HTTP/1.1 ");
        row += to_string(status);
        row += string(" ") + getStatusText() + "\r\n";
        header = row;
    }
    void addToHeader(const string &key, const string &value);
    void sendHeader(boost::asio::ip::tcp::socket &socket); 
    string getHTMLMessage(const string &);

private:
    string getStatusText();
    int status;
    string header;
};
#endif
