// Authors: Patrick Tinz and Janik Tinz

#include <thread>
#include <time.h>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <cassert>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>

#include "UDPServer.h"
#include "Drive.h"
#include "MQTTPublisher.h"

UDPServer::~UDPServer() 
{
}

UDPServer::UDPServer(boost::asio::io_service& io_service, short port):socket_(io_service, udp::endpoint(udp::v4(), port))
{
    socket_.async_receive_from(
        boost::asio::buffer(data_, max_length), sender_endpoint_,
        boost::bind(&UDPServer::handle_receive_from, this,
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
    
    start = false;
    counterTraffic = 0;
    counterSpeed = 0;
    counterTank = 0;
    counterMilage = 0;
    test = false;
}

string UDPServer::generateDate()
{
    time_t timestamp_tmp;
    tm *now;
    timestamp_tmp = time(0);
    now = localtime(&timestamp_tmp);

    string time = to_string(now->tm_mday) + '-' + to_string(now->tm_mon+1) + '-' + to_string(now->tm_year+1900) + '-' + to_string(now->tm_hour) + ':' + to_string(now->tm_min) + ':' + to_string(now->tm_sec);
    return time;
}

void UDPServer::print(boost::asio::ip::address ip_address, unsigned short port, string value, string type)
{
    //system("clear");
    cout << endl;
    cout << "Sensor type: " << type << endl; 
    cout << "Port: " << port << endl;
    cout << "IP: " << ip_address << endl;
    cout << "Value: " << value << endl; 
}

void startPublisher(string date, string type, string value)
{
    CentralControl &control = CentralControl::getInstance();
    std::stringstream ss;
    namespace pt = boost::property_tree;
    pt::ptree root;
    
    root.put("numberPlate", control.getNumberPlate());
    root.put("manufacturer", control.getCar());
    root.put("date", date);
    root.put(type, value);
    
    pt::write_json(ss, root);
    
    MQTTPublisher pub;
    string data = ss.str();
    pub.startPublisher(data);
}

static bool check = false;
void UDPServer::handle_receive_from(const boost::system::error_code& error, size_t bytes_recvd)
{
    time_t timeSensor;
    CentralControl &control = CentralControl::getInstance();
    
    if(start == false)
    {
        control.startTimer();
        timerTest = time(0);
        start = true;
    }
    
    socket_.async_receive_from(
        boost::asio::buffer(data_, max_length), sender_endpoint_,
        boost::bind(&UDPServer::handle_receive_from, this,
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
    
    if(sizeof(data_) != 0){
        string strData = data_;
        size_t pos = strData.find(","); 
        string value = strData.substr (0, pos);
        string type = strData.substr (pos+1);
        string date = generateDate();
        
        boost::thread t1(startPublisher, date, type, value);
        t1.join();
        
        
        if(type == "tank" && value != "stop")
        {
            float tankValue = stof(value);
            control.addTank(tankValue);
            counterTank++;
            control.setTstartTank(time(0));
        }
        else if(type == "traffic" && value != "stop")
        {
            string trafficValue = value;
            control.addTraffic(trafficValue);
            counterTraffic++;
            control.setTstartTraffic(time(0));
        }
        else if(type == "milage" && value != "stop")
        {
            float milageValue = stof(value);
            control.addMilage(milageValue);
            counterMilage++;
            control.setTstartMilage(time(0));
        }
        else if(type == "speed" && value != "stop")
        {
            float speedValue = stof(value);
            control.addSpeed(speedValue);
            counterSpeed++;
            control.setTstartSpeed(time(0));
        }
        
        if(strData == "stop," + type && check == true)
        {
            string timestamp = generateDate();
            Drive *drive1 = new Drive(timestamp, control.getTank(), control.getTraffic(), control.getMilage(), control.getAverageSpeed());
            
            // clear all vectors for a new drive
            control.clearTraffic();
            control.clearSpeed();
            control.clearTank();
            control.clearMilage();
            
            control.addDrive(drive1);
            control.writeInFile(control.getNumberPlate());
            check = false;
        }
        else if(value != "stop")
        { 
            // sensor tank
            timeSensor = time(0) - control.getTstartTank();
            if (timeSensor > 2)
                cout << "Sensor tank does not work!" << endl;
            
            // sensor traffic
            timeSensor = time(0) - control.getTstartTraffic();
            if(timeSensor > 2)
                cout << "Sensor traffic does not work!" << endl;
            
            // sensor milage
            timeSensor = time(0) - control.getTstartMilage();
            if(timeSensor > 2)
                cout << "Sensor milage does not work!" << endl;
            
            // sensor speed
            timeSensor = time(0) - control.getTstartSpeed();
            if(timeSensor > 2)
                cout << "Sensor speed does not work!" << endl;
            
            check = true;
            
            if(test)
            {
                system("clear");
                cout << "################################################" << endl;
                cout << "                Performance Test                " << endl;
                cout << "################################################" << endl;
                cout << "Counter UDP-Packets Traffic: " << counterTraffic<< endl;
                cout << "Counter UDP-Packets Speed: " << counterSpeed << endl;
                cout << "Counter UDP-Packets Tank: " << counterTank << endl;
                cout << "Counter UDP-Packets Milage: " << counterMilage << endl;
                time_t duration = time(0) - timerTest;
                cout << "Actual Time: " <<  duration << "s" << endl;
                if(duration >= endTimeTest)
                    exit(0);
            }
            else{
            // print IP-sender, sender port, value and sensor type
            print(sender_endpoint_.address(), sender_endpoint_.port(), value, type);
            }
        } 
    }
}

void UDPServer::setTest(bool test) {
    this->test = test;
}

bool UDPServer::isTest() const {
    return test;
}

void UDPServer::setEndTimeTest(time_t endTimeTest) {
    this->endTimeTest = endTimeTest;
}

time_t UDPServer::getEndTimeTest() const {
    return endTimeTest;
}

