// Authors: Patrick Tinz and Janik Tinz

#include <iostream>
#include "HttpResponse.h"

using namespace std;

void HttpResponse::addToHeader(const string &key, const string &value) 
{
    header += key + ": " + value + "\r\n";
}

void HttpResponse::sendHeader(boost::asio::ip::tcp::socket &socket) 
{
    header += "\r\n";
    socket.write_some(boost::asio::buffer(header.c_str(), header.length()));
    //cout << "SENDED HEADER:\n" << header;
}

string HttpResponse::getHTMLMessage(const string &text = "") 
{
    string statusMsg(to_string(status) + " " + getStatusText());
    string message("<html><head><title>");
    message += statusMsg + "</title></head>";
    message += "<body><h1>" + statusMsg + "</h1>" + text + "</body></html>";
    return message;
}

string HttpResponse::getStatusText() 
{
    string txt;
    switch (status) 
    { 
        case 200:
          txt = "OK";
          break;
        case 400:
          txt = "Bad Request";
          break;
        case 404:
          txt = "Not Found";
          break;
        case 500:
          txt = "Internal Server Error";
          break;
        case 501:
          txt = "Not Implemented";
          break;
        default:
          txt = "undefined";
    }
    return txt;
}
