// Authors: Patrick Tinz and Janik Tinz

#include <iostream>
#include <cstdlib>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <cassert>
#include <exception>
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include "src/MQTTClient.h"
#include "MQTTSubscriber.h"
#include "Manufacturer.h"
#include "Drive.h"

using namespace std;

//#define ADDRESS     "tcp://192.168.178.107:1883"
//#define TOPIC       "data/sensor"
//#define QOS         2
#define TIMEOUT     10000L

volatile MQTTClient_deliveryToken deliveredtoken;

MQTTSubscriber::MQTTSubscriber() 
{
    ipBroker = "tcp://127.0.0.1:1883";
    topic = "data/sensor";
    qos = 2;
    
    ifstream input("settingsMQTT.txt");
    if (input.good()) 
    {
        string line;
    
        getline(input, line);
        ipBroker = line;  

        getline(input, line);
        topic = line;

        getline(input, line);
        qos = atoi(line.c_str());        
        input.close();
    }
}

MQTTSubscriber::~MQTTSubscriber() 
{
}

MQTTSubscriber& MQTTSubscriber::getInstance()
{
    static MQTTSubscriber instance;
    return instance;
}

static bool check = true;
void writeInDatabase(string dataNEW)
{
    Manufacturer& manu = Manufacturer::getInstance();
    string value, numberPlate;
    
    try
    {
        stringstream sNeW;        
        sNeW.str(dataNEW);
        
        // Debug
        cout << sNeW.str() << endl;
        
        boost::property_tree::ptree pt;
        boost::property_tree::read_json(sNeW, pt);

        BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt)
        {   
            value = v.second.data();
            if(v.first == "numberPlate")
            {
                numberPlate = value;
                
                if(manu.getDrives().count(numberPlate) == 0)
                {
                    Drive* newDrive = new Drive();
                    manu.addDrives(numberPlate, newDrive);
                }               
                manu.getDrives()[numberPlate]->setNumberPlate(value);
            }
            else if(v.first == "manufacturer")
            {
                manu.getDrives()[numberPlate]->setManufacturer(value);
            }
            else if(v.first == "date")
            {
                manu.getDrives()[numberPlate]->setTimestamp(value);
            }
            else if(v.first == "traffic" && value != "stop")
            {
                manu.getDrives()[numberPlate]->addTraffic(value);
            }
            else if(v.first == "tank" && value != "stop")
            {
                manu.getDrives()[numberPlate]->addTank(value);
            }
            else if(v.first == "speed" && value != "stop")
            {
                manu.getDrives()[numberPlate]->addSpeed(value);
            }
            else if(v.first == "milage" && value != "stop")
            {
               manu.getDrives()[numberPlate]->addMilage(value);
            }
           
            // Debug
            //std::cout << v.second.data() << std::endl;
        }    
    }
    catch (std::exception const& e)
    {
        std::cerr << e.what() << std::endl;
    }
    
    if(value == "stop" && check == true)
    {
        // file structure: manufacturer -> server [1..3] -> numberplate.txt
        
        char puffer[200];
        char *actualDir = getcwd(puffer,sizeof(puffer));
        mkdir(manu.getDrives().at(numberPlate)->getManufacturer().c_str(), S_IRWXU);
        chdir(manu.getDrives().at(numberPlate)->getManufacturer().c_str());
        string server = "server " + manu.getIdServer();
        mkdir(server.c_str(), S_IRWXU);
        chdir(server.c_str());
        

        string file =  numberPlate + ".txt";

        ifstream OpenFile(file);
        if (OpenFile.good()) {        
            OpenFile.close();
        }
        else
        {
            ofstream WriteInFile; 
            WriteInFile.open(file, ios::out);

            WriteInFile << manu.getDrives().at(numberPlate)->getManufacturer() << '\n';
            WriteInFile << '#' << '\n';

            WriteInFile.close();
        }

        ofstream WriteInFile; 
        WriteInFile.open(file, ios::out | ios::app); 

        WriteInFile << manu.getDrives().at(numberPlate)->getTimestamp() << '\n';
        
        if(manu.getDrives().at(numberPlate)->getTraffic().size() > 0)
            WriteInFile << "traffic:";
        for (unsigned int i = 0; i < manu.getDrives().at(numberPlate)->getTraffic().size(); i++)
        {      
            if(i ==  manu.getDrives().at(numberPlate)->getTraffic().size()-1)
                WriteInFile << manu.getDrives().at(numberPlate)->getTraffic()[i] << ";" << '\n';
            else
                WriteInFile << manu.getDrives().at(numberPlate)->getTraffic()[i] << ";";                                                 
        }

        if(manu.getDrives().at(numberPlate)->getAverageSpeed().size() > 0)
            WriteInFile << "speed:";
        for (unsigned int i = 0; i < manu.getDrives().at(numberPlate)->getAverageSpeed().size(); i++)
        {
            if(i == manu.getDrives().at(numberPlate)->getAverageSpeed().size()-1)
                WriteInFile << manu.getDrives().at(numberPlate)->getAverageSpeed()[i] << ";" << '\n';
            else
                WriteInFile << manu.getDrives().at(numberPlate)->getAverageSpeed()[i] << ";";                                       
        }

        if(manu.getDrives().at(numberPlate)->getTank().size() > 0)
            WriteInFile << "tank:";
        for (unsigned int i = 0; i < manu.getDrives().at(numberPlate)->getTank().size(); i++)
        {
            if(i == manu.getDrives().at(numberPlate)->getTank().size()-1)
                WriteInFile << manu.getDrives().at(numberPlate)->getTank()[i] << ";" << '\n';
            else
                WriteInFile << manu.getDrives().at(numberPlate)->getTank()[i] << ";";                                       
        }

        if(manu.getDrives().at(numberPlate)->getMilage().size() > 0)
            WriteInFile << "milage:";
        for (unsigned int i = 0; i < manu.getDrives().at(numberPlate)->getMilage().size(); i++)
        {
            if(i == manu.getDrives().at(numberPlate)->getMilage().size()-1)
                WriteInFile << manu.getDrives().at(numberPlate)->getMilage()[i] << ";" << '\n';
            else
                WriteInFile << manu.getDrives().at(numberPlate)->getMilage()[i] << ";";                                       
        }
        WriteInFile << '#' << '\n';
        WriteInFile.close();
        check = false;
        
        manu.getDrives().at(numberPlate)->clearTraffic();
        manu.getDrives().at(numberPlate)->clearTank();
        manu.getDrives().at(numberPlate)->clearSpeed();
        manu.getDrives().at(numberPlate)->clearMilage();
        manu.getDrives().erase(numberPlate);
        
        chdir(actualDir);
    }
    if(value != "stop")
        check = true;
}

void delivered(void *context, MQTTClient_deliveryToken dt)
{
    printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    int i;
    char* payloadptr;
    printf("Message arrived\n");
    printf("     topic: %s\n", topicName);
    printf("   message: ");
    payloadptr = (char*) message->payload;
    char msg[1024];
    for(i=0; i<message->payloadlen; i++)
    {
        msg[i] = *payloadptr++;
    }
    
    string str(msg);
    size_t pos = str.find("}"); 
    string value = str.substr (0, pos+1);
    
    // Debug
    //cout << value << endl;
    
    writeInDatabase(value);
   
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}
void connlost(void *context, char *cause)
{
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}

int MQTTSubscriber::startSubscriber()
{
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;
    int ch;
    Manufacturer &manu = Manufacturer::getInstance();
    MQTTClient_create(&client, ipBroker.c_str(), (manu.getIdServer()).c_str(),
        MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }
    printf("Subscribing to topic %s\nfor client %s using QoS%u\n\n"
           "Press Q<Enter> to quit\n\n", topic.c_str(), (manu.getIdServer()).c_str(), qos);
    MQTTClient_subscribe(client, topic.c_str(), qos);
    do
    {
        ch = getchar();
    } while(ch!='Q' && ch != 'q');
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}

void MQTTSubscriber::setQos(unsigned int qos) {
    this->qos = qos;
}

unsigned int MQTTSubscriber::getQos() const {
    return qos;
}

void MQTTSubscriber::setTopic(string topic) {
    this->topic = topic;
}

string MQTTSubscriber::getTopic() const {
    return topic;
}

void MQTTSubscriber::setIpBroker(string ipBroker) {
    this->ipBroker = ipBroker;
}

string MQTTSubscriber::getIpBroker() const {
    return ipBroker;
}
