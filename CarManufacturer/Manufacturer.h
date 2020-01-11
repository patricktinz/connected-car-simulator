// Authors: Patrick Tinz and Janik Tinz

#include <map>
#include <string>
#include "Drive.h"

using namespace std;

#ifndef MANUFACTURER_H
#define MANUFACTURER_H

class Manufacturer {
public:
    static Manufacturer& getInstance();
   
    virtual ~Manufacturer();

    void setIdServer(string);
    string getIdServer();
    void setDrives(map<string, Drive*> drives);
    map<string, Drive*> getDrives() const;
    void addDrives(string key, Drive* drive);
    
    void setServerPort(vector<int> serverPort);
    vector<int> getServerPort() const;
    void setServerIP(vector<string> serverIP);
    vector<string> getServerIP() const;
    
    void addServerIP(string);
    void addServerPort(int);
    
private:
    Manufacturer();
    string idServer;
    map<string, Drive*> drives;
    
    vector<string> serverIPThrift;
    vector<int> serverPortThrift;
};

#endif /* MANUFACTURER_H */

