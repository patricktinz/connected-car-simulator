// Authors: Patrick Tinz and Janik Tinz

#include "MQTTPublisher.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "src/MQTTAsync.h"
#include "src/MQTTClient.h"
#include "unistd.h"
#include "CentralControl.h"
#include <iostream>
#include <fstream>

using namespace std;

volatile MQTTClient_deliveryToken deliveredtoken;

int finished = 0;

//#define ADDRESS     "tcp://192.168.178.107:1883"
#define CLIENTID    "Publisher"
//#define TOPIC       "data/sensor"
string PAYLOAD;         
//#define QOS         2
#define TIMEOUT     10000L

MQTTPublisher::MQTTPublisher() 
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

MQTTPublisher::~MQTTPublisher() 
{
}

/*MQTTPublisher& MQTTPublisher::getInstance()
{
    static MQTTPublisher instance;
    return instance;
}*/

void delivered(void *context, MQTTClient_deliveryToken dt)
{
    //printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}
int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    int i;
    char* payloadptr;
    /*printf("Message arrived\n");
    printf("     topic: %s\n", topicName);
    printf("   message: ");
    payloadptr = (char*) message->payload;
    for(i=0; i<message->payloadlen; i++)
    {
        putchar(*payloadptr++);
    }
    putchar('\n');*/
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}
void connlost(void *context, char *cause)
{
    //printf("\nConnection lost\n");
    //printf("     cause: %s\n", cause);
}

void MQTTPublisher::startPublisher(string data)
{
    PAYLOAD = data;
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    int rc;
    MQTTClient_create(&client, ipBroker.c_str(), CLIENTID,
        MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        //printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }
    pubmsg.payload = (void*) PAYLOAD.c_str();
    pubmsg.payloadlen = strlen(PAYLOAD.c_str());
    pubmsg.qos = qos;
    pubmsg.retained = 0;
    deliveredtoken = 0;
    MQTTClient_publishMessage(client, topic.c_str(), &pubmsg, &token);
    /*printf("Waiting for publication of %s\n"
            "on topic %s for client with ClientID: %s\n",
            PAYLOAD.c_str(), TOPIC, CLIENTID);*/
    while(deliveredtoken != token);
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
}

void MQTTPublisher::setQos(unsigned int qos) {
    this->qos = qos;
}

unsigned int MQTTPublisher::getQos() const {
    return qos;
}

void MQTTPublisher::setTopic(string topic) {
    this->topic = topic;
}

string MQTTPublisher::getTopic() const {
    return topic;
}

void MQTTPublisher::setIpBroker(string ipBroker) {
    this->ipBroker = ipBroker;
}

string MQTTPublisher::getIpBroker() const {
    return ipBroker;
}