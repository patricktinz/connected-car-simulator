// Authors: Patrick Tinz and Janik Tinz

#include <string>

using namespace std;

#ifndef MQTTPUBLISHER_H
#define MQTTPUBLISHER_H

class MQTTPublisher {
public:
    //static MQTTPublisher& getInstance();
    MQTTPublisher();
    virtual ~MQTTPublisher();
    
    void startPublisher(string);
    //static void onConnect(void* context, MQTTAsync_successData* response);
    //void onSend(void* context, MQTTAsync_successData* response);
    //void onDisconnect(void* context, MQTTAsync_successData* response);
    //void connlost(void *context, char *cause);  
    void setQos(unsigned int qos);
    unsigned int getQos() const;
    void setTopic(string topic);
    string getTopic() const;
    void setIpBroker(string ipBroker);
    string getIpBroker() const;
    
private:
    string ipBroker;
    string topic;
    unsigned int qos;
};

#endif /* MQTTPUBLISHER_H */

