// Authors: Patrick Tinz and Janik Tinz

#include "Sensor.h"
#include "Parameter.h"
#include <iostream>
#include <fstream>
#include <boost/asio.hpp>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <errno.h>

using namespace std;
using boost::asio::ip::udp;

Sensor::Sensor() 
{ 
}

Sensor::Sensor(string ip, string type) 
{ 
    this->ipAddressServer = ip;
    this->type = type;
    
    string filename = type + "Log.txt";
    ofstream output(filename);
        output << type << ":";
        output.put('\n');
    output.close();
}

Sensor::~Sensor() 
{
}

// Debug function
void Sensor::print(string value)
{
    cout << type << ": ";
    cout << value << endl;
}

void Sensor::writeLogFiles(string value)
{
    string filename = type + "Log.txt";
    ofstream output;
    output.open(filename, ios::out | ios::app);
    output << value;
    output.put('\n');
    output.close();
}

int Sensor::sendUDP(string valueSensor)
{   
    static int counterPackets = 0;
    string data = valueSensor;
    
    char value[1024];
    strcpy(value, data.c_str());
    int s, rc;
    struct sockaddr_in clientAddress, ServerAddress;
    struct hostent *h;

    const char *cstr = ipAddressServer.c_str();
    // check server ip-address
    h=gethostbyname(cstr);
    if (h == NULL) {
        cout << "Unknown Host" << endl;
      exit (EXIT_FAILURE);
    }
    cout << "Client send data to " << h->h_name << endl;
    cout << "Sensor " << type  << " " << "Number: " << ++counterPackets << endl;
    ServerAddress.sin_family = h->h_addrtype;
    memcpy ( (char *) &ServerAddress.sin_addr.s_addr,
             h->h_addr_list[0], h->h_length);
    ServerAddress.sin_port = htons (portServer);
    
    // create socket
    s = socket (AF_INET, SOCK_DGRAM, 0);
    if (s < 0) {
       cout << "Client can not open the socket: " << strerror(errno) << endl;
       exit (EXIT_FAILURE);
    }
    // bind port
    clientAddress.sin_family = AF_INET;
    clientAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    clientAddress.sin_port = htons(0);
    rc = ::bind(s, (struct sockaddr*) &clientAddress, sizeof(clientAddress));

    if (rc < 0) {
       cout << "Client can not bind the port: " << strerror(errno) << endl;
       exit (EXIT_FAILURE);
    }
    
    // send data
    rc = sendto (s, value, strlen (value) + 1, 0, (struct sockaddr *) &ServerAddress, sizeof (ServerAddress));
    if (rc < 0) {
       cout << "Client can not send data... " << endl;
       close (s);
       exit (EXIT_FAILURE);
    }
    return 1;
}

void Sensor::receivedUDP()
{
    // receive data
    // send data with sendUDP
    
    // listen to ports
    // port traffic - 7000
    // port speed - 7001
    // port tank - 7002
    // port milage - 7003
    
    int port = 0;   // receive data on this port
    
    if(type == "traffic")
    {
        port = 7000;
    }
    else if(type == "speed")
    {
        port = 7001;
    }
    else if(type == "tank")
    {
        port = 7002;
    }
    else if(type == "milage")
    {
        port = 7003;
    }
    
    boost::asio::io_service ioService;
    udp::socket socket(ioService, udp::endpoint(udp::v4(), port));
    
    // Cancel with ctrl+c
    cout << "Sensor wait for data on port (UDP) " << port << " ... (Cancel with ctrl+c)\n";
    
    while (true) 
    { 
        boost::asio::ip::udp::endpoint sender_endpoint;
        char data[256] = {0};
        socket.receive_from(boost::asio::buffer(data), sender_endpoint);
        value = data;
        
        // Debug function
        //print(value);
        
        writeLogFiles(value);
        string valueSensor = value + "," + type;
        sendUDP(valueSensor);
    }
}
