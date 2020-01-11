// Authors: Patrick Tinz and Janik Tinz

#include <cstddef>
#include <fstream>
#include <string>
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include "HttpResponse.h"

using namespace std;

#ifndef FILEHANDLER_H
#define FILEHANDLER_H

class FileHandler {
public:
    FileHandler(boost::asio::ip::tcp::socket &s, const string &d)
        : socket(s), fileName(d) {}
    void process();
    void send();
private:
    boost::asio::ip::tcp::socket &socket;
    string fileName;

    string getContentType(); 
    void sendFile(ifstream &); 
};
#endif
