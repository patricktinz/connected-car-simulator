// Authors: Patrick Tinz and Janik Tinz

#include "stdio.h"
#include "stdlib.h"
#include "src/MQTTAsync.h"
#include "unistd.h"
#include <vector>
#include <string>

using namespace std;

#ifndef MQTTSUBSCRIBER_H
#define MQTTSUBSCRIBER_H

class MQTTSubscriber {
public:
    static MQTTSubscriber& getInstance();
    virtual ~MQTTSubscriber();
    
    int startSubscriber();
    
    void setQos(unsigned int qos);
    unsigned int getQos() const;
    void setTopic(string topic);
    string getTopic() const;
    void setIpBroker(string ipBroker);
    string getIpBroker() const; 
    
private:
    MQTTSubscriber();
    
    string ipBroker;
    string topic;
    unsigned int qos;
};

#endif /* MQTTSUBSCRIBER_H */

