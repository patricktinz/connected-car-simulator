// Authors: Patrick Tinz and Janik Tinz

#include <string>

using namespace std;

#ifndef SENSOR_H
#define SENSOR_H

class Sensor {
public:
    Sensor();
    Sensor(string, string);
    virtual ~Sensor();
    
    void print(string);
    void writeLogFiles(string);
    int sendUDP(string);
    void receivedUDP();
  
private:
    string type;
    string value;
    string ipAddressServer;
};

#endif /* SENSOR_H */

