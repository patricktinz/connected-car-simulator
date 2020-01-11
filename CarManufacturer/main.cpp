// Authors: Patrick Tinz and Janik Tinz

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <map>
#include <boost/thread.hpp>
#include "Drive.h"
#include "MQTTSubscriber.h"
#include "Manufacturer.h"
#include "ApacheThriftHandler.h"

#include "gen-cpp/Calc.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <string>
#include <boost/thread/detail/thread.hpp>

using namespace std;

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

void check_input(unsigned int &choice)
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

void writeSettingsMQTTInFile()
{
    MQTTSubscriber &sub = MQTTSubscriber::getInstance();
    ofstream output("settingsMQTT.txt");
    
    output << sub.getIpBroker();
    output.put('\n');
    
    output << sub.getTopic();
    output.put('\n');
    
    output << sub.getQos();
    output.put('\n');
}

void readSettingsThrift()
{
    Manufacturer& manu = Manufacturer::getInstance();
    ifstream input("settingsThrift.txt");
    if (input.good()) 
    {
        string line;
    
        while(!input.eof())
        {
            getline(input, line);
            manu.addServerIP(line);  

            getline(input, line);
            int value = atoi(line.c_str());
            manu.addServerPort(value);
            
            if(input.peek() == '\n')
                break;
        }
        
        input.close();
    }
}

void writeSettingsThriftInFile()
{   
    Manufacturer& manu = Manufacturer::getInstance();
    ofstream output("settingsThrift.txt");
    
    for(int i = 0; i < manu.getServerIP().size(); i++)
    {
        output << manu.getServerIP().at(i);
        output.put('\n');
        
        output << manu.getServerPort().at(i);
        output.put('\n');
    }
}

void startApacheThriftHandler()
{
    // server 1 port: 9190
    // server 2 port: 9191
    // ...
    Manufacturer& manu = Manufacturer::getInstance();
   
    int idServer = atoi(manu.getIdServer().c_str());
    
    int port = 9189 + idServer;
    ::apache::thrift::stdcxx::shared_ptr<ApacheThriftHandler> handler(new ApacheThriftHandler());
    ::apache::thrift::stdcxx::shared_ptr<TProcessor> processor(new CalcProcessor(handler));
    ::apache::thrift::stdcxx::shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
    ::apache::thrift::stdcxx::shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
    ::apache::thrift::stdcxx::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());
    
    TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
    server.serve();
}

void submenuServer()
{
    Manufacturer &manu = Manufacturer::getInstance();
    MQTTSubscriber &sub = MQTTSubscriber::getInstance();
    
    unsigned int choice;
    while(true)
    {
        cout << "##################################" << endl;
        cout << "             Server               " << endl;
        cout << "##################################" << endl;
        for(int i = 0; i < manu.getServerIP().size(); i++)
        {
            cout << "(" << i+1 <<") Server " << i+1 << endl;
        }
        cout << "(" << manu.getServerIP().size()+1 << ") Back" << endl;
        check_input(choice);
        
        if(choice > 0 && choice <= manu.getServerIP().size())
        {
            string idServer = to_string(choice);
            manu.setIdServer(idServer);
            boost::thread f1(startApacheThriftHandler);
            sub.startSubscriber();
            break;
        }
        else if(choice == manu.getServerIP().size() + 1)
        {
            break;
        }
        else
        {
            cout << "!!!Invalid input!!!" << endl;
            cout << "Please choose a existing server..." << endl;
        }
    }
}

void submenuSettingsMQTT()
{
    while(true)
    {
        system("clear");
        string value;
        unsigned int input = 2;    
        MQTTSubscriber &sub = MQTTSubscriber::getInstance(); 
        
        cout << "############################" << endl;
        cout << "        Settings MQTT       " << endl;
        cout << "############################" << endl;
        cout << "(1) Edit IP-address Broker" << endl;
        cout << "(2) Edit topic" << endl;
        cout << "(3) Edit QoS" << endl;
        cout << "(4) Back" << endl;
        check_input(input);
        
        switch(input)
        {
            case 1:
                cout << "IP-address Broker: " << endl;
                cin >> value;
                value = "tcp://" + value + ":1883";
                sub.setIpBroker(value);
                break;
            case 2:
                cout << "Topic: " << endl;
                cin >> value;
                sub.setTopic(value);
                break;
            case 3:
            {
                unsigned int qos = 3;               
                while(qos > 2)
                {
                    cout << "### Edit QoS ###" << endl;
                    cout << "(0) QoS = 0: Fire and forget" << endl;
                    cout << "(1) QoS = 1: Message is received least once" << endl;
                    cout << "(2) QoS = 2: Message is received exactly once" << endl;
                    check_input(qos);
                    sub.setQos(qos);
                }
                break; 
            }
            case 4: 
                writeSettingsMQTTInFile();
                return;
            default:
                cout << "!!!Invalid input!!!" << endl;
                break;
        }
    }
}

void submenuSettingsApacheThrift()
{
    while(true)
    {
        system("clear");
        string value;
        unsigned int input = 2; 
        Manufacturer &manu = Manufacturer::getInstance();
        
        cout << "############################" << endl;
        cout << "   Settings Apache Thrift   " << endl;
        cout << "############################" << endl;
        cout << "(1) Add Server" << endl;
        for(int i = 0; i < manu.getServerIP().size(); i++)
        {
            cout << "(" << i+2 <<") Edit Server " << i+1 << endl;
        }
        cout << "(" << manu.getServerIP().size()+2 << ") Back" << endl;
        check_input(input);
        
        if(input == 1)
        {
            string ip = "127.0.0.1";   // default localhost
            int port = 9190;   // default first port for apache thrift in this program
            cout << "Enter IP-address and port: " << endl;
            cout << "IP: ";
            cin >> ip;
            manu.addServerIP(ip);
            cout << "Port: ";
            cin >> port;
            manu.addServerPort(port);
        } 
        else if(input == manu.getServerIP().size()+2)
        {
            writeSettingsThriftInFile();
            return;
        }
        else
        {
            unsigned int choice = 1;
            cout << "(1) Edit IP-Address" << endl;
            cout << "(2) Edit Port" << endl;
            check_input(choice);
            switch(choice)
            {
                case 1:
                {
                    string ip = "127.0.0.1";   // default localhost
                    cout << "IP: ";
                    cin >> ip;
                    manu.getServerIP().at(input -2) = ip;
                }
                    break;
                case 2:
                {
                    int port = 9190;   // default first port for apache thrift in this program
                    cout << "Port: ";
                    cin >> port;
                    manu.getServerPort().at(input -2) = port;
                }
                    break;
                default:
                    cout << "!!!Invalid input!!!" << endl;
                    break;
            }
        }
    }
}

void submenuSettings()
{
    while(true)
    {
        system("clear");
        unsigned int input = 2;            
        
        cout << "#############################" << endl;
        cout << "           Settings          " << endl;
        cout << "#############################" << endl;
        cout << "(1) MQTT" << endl;
        cout << "(2) Apache Thrift" << endl;
        cout << "(3) Back" << endl;
        check_input(input);
        
        switch(input)
        {
            case 1:
                submenuSettingsMQTT();
                break;
            case 2:
                submenuSettingsApacheThrift();
                break;
            case 3:
                return;
            default:
                cout << "!!!Invalid input!!!" << endl;
                break;
        }
    }
}

int main(int argc, char** argv) 
{
    unsigned int input = 4;
    MQTTSubscriber &sub = MQTTSubscriber::getInstance(); 
    Manufacturer& manu = Manufacturer::getInstance();
    
    readSettingsThrift();
    
    while(true)
    {
        system("clear");
        cout << "######################" << endl;
        cout << "   Car Manufacturer   " << endl;
        cout << "######################" << endl;
        cout << "(1) Start" << endl;
        cout << "(2) Settings" << endl;
        cout << "(3) Exit" << endl;
        check_input(input);
        cout << endl;
        
        switch(input)
        {
            case 1: 
            {
                submenuServer();
                //boost::thread f1(startApacheThriftHandler);
                //sub.startSubscriber(); 
            }
                break;
            case 2:
                submenuSettings();
                break;
            case 3:
                return 0;
            default:
                cout << "!!!Invalid input!!!" << endl;
                break;
        }
        
        // delete map "drives" for restart
        manu.getDrives().clear();
    } 
    return 0;
}

