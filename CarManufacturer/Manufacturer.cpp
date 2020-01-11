// Authors: Patrick Tinz and Janik Tinz

#include <iostream>
#include "Manufacturer.h"

using namespace std;

Manufacturer::Manufacturer() {
}

Manufacturer::~Manufacturer() {
}

Manufacturer& Manufacturer::getInstance()
{
    static Manufacturer instance;
    return instance;
}

void Manufacturer::setIdServer(string id) {
    this->idServer = id;
}

string Manufacturer::getIdServer() {
    return this->idServer;
}

void Manufacturer::setDrives(map<string, Drive*> drives) {
    this->drives = drives;
}

map<string, Drive*> Manufacturer::getDrives() const {
    return drives;
}

void Manufacturer::addDrives(string key, Drive* drive)
{
    drives.insert(pair<string, Drive*>(key, drive));
}

void Manufacturer::setServerPort(vector<int> serverPort) {
    this->serverPortThrift = serverPort;
}

vector<int> Manufacturer::getServerPort() const {
    return serverPortThrift;
}

void Manufacturer::setServerIP(vector<string> serverIP) {
    this->serverIPThrift = serverIP;
}

vector<string> Manufacturer::getServerIP() const {
    return serverIPThrift;
}

void Manufacturer::addServerIP(string ip)
{
    serverIPThrift.push_back(ip);
}
    
void Manufacturer::addServerPort(int port)
{
    serverPortThrift.push_back(port);
}
