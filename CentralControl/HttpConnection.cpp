// Authors: Patrick Tinz and Janik Tinz

#include "HttpConnection.h"

using namespace std;

void HttpConnection::accept() 
{ 
    acceptor.accept(socket); 
}

void HttpConnection::operator()() 
{
    constexpr size_t BUFSIZE = 1024;
    char data[BUFSIZE + 1] = {0};
    // read request
    size_t count = socket.read_some(boost::asio::buffer(data, BUFSIZE));
    data[count] = '\0';
    //cout << data << '\n';
    string header(data);
    auto space = header.find(' ');
    
    // error message wrong protocol
    if (header.substr(0, space) != "GET") 
    {
        HttpResponse httpResponse(400);
        httpResponse.addToHeader("Content-Type", "text/html");
        string message = httpResponse.getHTMLMessage("Protocol is not implement!");
        httpResponse.addToHeader("Content-Length", to_string(message.length()));
        httpResponse.sendHeader(socket);
        socket.write_some(boost::asio::buffer(message.c_str(), message.length()));
        return; 
    }
    auto space2 = header.find(' ', space + 1);
    string path = header.substr(space + 1, space2 - space - 1);
    
    auto qpos = path.find('?');
    if (qpos != string::npos) 
    {   // Request
        RequestHandler rh(socket, path.substr(qpos + 1));
        rh.process();
    } 
    else 
    {   // file
        string fileName = directory + path;
        if (path == "/" || path == "/favicon.ico") 
        {
            fileName = directory + "/index.html";
        }
        FileHandler dh(socket, fileName);
        dh.process();
    }
    socket.close();
  }
