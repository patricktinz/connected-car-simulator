// Authors: Patrick Tinz and Janik Tinz

#include <iostream>
#include "Server.h"

using namespace std;

void Server::run() 
{
    while (true) 
    { 
        HttpConnection conn(acceptor, verzeichnis);
        conn.accept(); // wait for incoming connection
        conn();        // edit
    }
}

