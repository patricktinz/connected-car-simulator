// Authors: Patrick Tinz and Janik Tinz

#include "CentralControl.h"
#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp> 

using boost::asio::ip::udp;
using namespace std;

#ifndef UDPSERVER_H
#define UDPSERVER_H

class UDPServer {
public:
    UDPServer(boost::asio::io_service& io_service, short port);
    virtual ~UDPServer();
    void handle_receive_from(const boost::system::error_code& error, size_t bytes_recvd);
 
    string generateDate();
    void print(boost::asio::ip::address, unsigned short, string, string);
    
    void setTest(bool test);
    bool isTest() const;
    void setEndTimeTest(time_t endTimeTest);
    time_t getEndTimeTest() const;
  
private:
    udp::socket socket_;
    udp::endpoint sender_endpoint_;
    enum { max_length = 1024 };
    char data_[max_length];
    
    bool start;
    
    // Test variable
    bool test;
    
    unsigned int counterTraffic;
    unsigned int counterSpeed;
    unsigned int counterTank;
    unsigned int counterMilage;
    time_t timerTest;
    time_t endTimeTest;
};

#endif /* UDPSERVER_H */

