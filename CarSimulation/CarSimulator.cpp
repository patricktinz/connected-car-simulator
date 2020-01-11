// Authors: Patrick Tinz and Janik Tinz

#include "CarSimulator.h"
#include "Parameter.h"
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <boost/asio.hpp>

using namespace std;
using boost::asio::ip::udp;

CarSimulator::CarSimulator() 
{
    ifstream input("ip-sensors.txt");
    string line;
    
    getline(input, line);
    ipTraffic = line;  
    
    getline(input, line);
    ipSpeed = line;
    
    getline(input, line);
    ipTank = line;

    getline(input, line);
    ipMilage = line;
}

CarSimulator::~CarSimulator() 
{
    ofstream output("ip-sensors.txt");
    
    output << ipTraffic;
    output.put('\n');
    
    output << ipSpeed;
    output.put('\n');
    
    output << ipTank;
    output.put('\n');
    
    output << ipMilage;
    output.put('\n');   
}

void CarSimulator::sendUDP(string value, string ip_server, const int port)
{
    boost::asio::io_service ioService;
    boost::asio::ip::udp::endpoint server(boost::asio::ip::address::from_string(ip_server), port);
    boost::asio::ip::udp::socket socket(ioService);
    socket.open(boost::asio::ip::udp::v4());
    socket.send_to(boost::asio::buffer(value), server);
    
    // output data
    cout << "Send data to IP: " << ip_server << " Port: " << port << " Value: " << value << endl;
}

void CarSimulator::send(string value, const int port)
{
    // port 7000 - traffic
    // port 7001 - speed
    // port 7002 - tank
    // port 7003 - milage
    // port 4242 - multicast to all sensors (stop to all)
    
    string ip_server = "";
    if(port == portTraffic)
    {
        ip_server = ipTraffic;
    } else if(port == portSpeed)
    {
        ip_server = ipSpeed;
    } else if(port == portTank)
    {
        ip_server = ipTank;
    } else if(port == portMilage)
    {
        ip_server = ipMilage;
    } else if(port == 4242)
    {
        sendUDP(value, ipTraffic, portTraffic);
        sendUDP(value, ipSpeed, portSpeed);
        sendUDP(value, ipTank, portTank);
        sendUDP(value, ipMilage, portMilage);
        return;
    } else
    {
        return;
    }
    sendUDP(value, ip_server, port);
}

void CarSimulator::startSimulator()
{
    float tank = 100;
    float milage = 0;
    unsigned int microseconds = 500000;   // 0.5 seconds   
    srand((unsigned)time(0));
    
    while(true)
    {
        // simulation
        int trafficSituation = 0;
        string traffic = "";
        int speed = 0;       
     
        trafficSituation = (rand()%4);
        switch(trafficSituation){
            // free
            case 0:
                traffic = "free";
                speed = (rand()%41)+60;
                tank -= 4;
                milage += 30;
                break;
            // moderate traffic
            case 1:
                traffic = "moderate traffic";
                speed = (rand()%31)+30;
                tank -= 3;
                milage += 40;
                break;

            // heavy traffic
            case 2:
                traffic = "heavy traffic";
                speed = (rand()%21)+10;
                tank -= 2;
                milage += 20;
                break;
            // traffic jam 
            case 3:
                traffic = "traffic jam";
                speed = (rand()%11);
                tank -= 1;
                milage += 5;
                break;
        }
        string tankString = to_string(tank);
        if(tank < 0)
        {
            tankString = "0";
            send(tankString, portTank);
        }else
        {
           send(tankString, portTank); 
        }
        
        send(traffic, portTraffic);
        string speedString = to_string(speed); 
        send(speedString, portSpeed);
        string milageString = to_string(milage);
        send(milageString, portMilage);
        
        if(tank <= 0)   // tank is empty
        {
                tank = 100;   
        }       
        usleep(microseconds);
    } 
}

void CarSimulator::startSimulator(int milageSimulation)
{
    float tank = 100;
    float milage = 0;
    unsigned int microseconds = 500000;   // 0.5 seconds 
    srand((unsigned)time(0));
    
    while(milage < milageSimulation && tank > 0)
    {
        // simulation
        int trafficSituation = 0;
        string traffic = "";
        int speed = 0;       
     
        trafficSituation = (rand()%4);
        switch(trafficSituation){
            // free
            case 0:
                traffic = "free";
                speed = (rand()%41)+60;
                tank -= 4;
                milage += 30;
                break;
            // moderate traffic
            case 1:
                traffic = "moderate traffic";
                speed = (rand()%31)+30;
                tank -= 3;
                milage += 40;
                break;

            // heavy traffic
            case 2:
                traffic = "heavy traffic";
                speed = (rand()%21)+10;
                tank -= 2;
                milage += 20;
                break;
            // traffic jam 
            case 3:
                traffic = "traffic jam";
                speed = (rand()%11);
                tank -= 1;
                milage += 5;
                break;
        }
        string tankString = to_string(tank);
        if(tank < 0)
        {
            tankString = "0";
            send(tankString, portTank);
        }else
        {
           send(tankString, portTank); 
        }
        
        send(traffic, portTraffic);
        string speedString = to_string(speed); 
        send(speedString, portSpeed);
        string milageString = to_string(milage);
        send(milageString, portMilage);
        
        if(tank <= 0)   // tank is empty
        {
            unsigned int answer = 2;
            
            while(answer != 1 && answer != 2)
            {
                cout << "Would you like to refill the tank? Then the simulation will continue. (yes(1)/no(2))" << endl;
                check_input(answer);

                if(answer == 1)
                {
                    tank = 100;
                }
            }
        }        
        usleep(microseconds);
    } 
    // end of simulation
    send("stop", 4242);
}

void CarSimulator::check_input(unsigned int &choice)
{
    cout << "Entry > ";
    cin >> choice;
    while (!cin)   // if choice is no number
    {
        try
        {
            throw "- Wrong input! Enter a number -";
        }
        catch (const char* str)
        {
            cout << str << endl;
        }
        cin.clear();
        cin.ignore();

        cout << "Entry > ";
        cin >> choice;
    }
}

void CarSimulator::setIpMilage(string ipMilage) {
    this->ipMilage = ipMilage;
}

string CarSimulator::getIpMilage() const {
    return ipMilage;
}

void CarSimulator::setIpTank(string ipTank) {
    this->ipTank = ipTank;
}

string CarSimulator::getIpTank() const {
    return ipTank;
}

void CarSimulator::setIpSpeed(string ipSpeed) {
    this->ipSpeed = ipSpeed;
}

string CarSimulator::getIpSpeed() const {
    return ipSpeed;
}

void CarSimulator::setIpTraffic(string ipTraffic) {
    this->ipTraffic = ipTraffic;
}

string CarSimulator::getIpTraffic() const {
    return ipTraffic;
}
