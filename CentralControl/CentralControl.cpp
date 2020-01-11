// Authors: Patrick Tinz and Janik Tinz

#include "CentralControl.h"
#include "Drive.h"
#include "UDPServer.h"
#include "Parameter.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <time.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

using namespace std;

CentralControl::CentralControl() 
{   
}

CentralControl::~CentralControl() 
{
}

CentralControl& CentralControl::getInstance()
{
    static CentralControl instance;
    return instance;
}

string CentralControl::generateDate()
{
    time_t timestamp_tmp;
    tm *now;
    timestamp_tmp = time(0);
    now = localtime(&timestamp_tmp);

    string time = to_string(now->tm_mday) + '.' + to_string(now->tm_mon+1) + '.' + to_string(now->tm_year+1900);
    return time;
}

void CentralControl::startTimer()
{
    tstartTraffic = time(0);
    tstartSpeed = time(0);
    tstartTank = time(0);
    tstartMilage = time(0);
}

void CentralControl::receivedUDP()
{
    // Cancel with ctrl+c
    cout << "Central Control wait for data on port (UDP) " << SERVER_PORT << " ... (Cancel with ctrl+c)\n";
    try
    {
        boost::asio::io_service io_service;
        UDPServer s(io_service, SERVER_PORT);
        io_service.run();      
    }
    catch (std::exception& e)
    {
      std::cerr << "Exception: " << e.what() << "\n";
    }
}

void CentralControl::readFile(string file)   // insert car data
{  
    string timestamp_tmp;
    
    vector<string> traffic;
    vector<float> milage;
    vector<float> tank;
    vector<float> averageSpeed;
    
    drives.clear();
    
    file = "cars/" + file + ".txt";
    ifstream readFile;
    readFile.open(file, ios::in);

    if (readFile)
    {
        string car_tmp, car_value;
        getline(readFile, car_value, '\n');
        car_tmp = car_value;

        car = car_tmp; 

        /*while (true)
        {
            float tank_tmp, milage_tmp, averageSpeed_tmp;
            string traffic_tmp, value;
            if (readFile.peek() == '#')
            {
                getline(readFile, value, '\n');
                getline(readFile, value, '\n');
                if(readFile.peek() == EOF)
                {
                    break;
                }
                
                timestamp_tmp = value;                  
            }  

            getline(readFile, value, ':');
            
            if(value == "traffic")
            {
                while(readFile.peek() != '\n')
                {
                    getline(readFile, value, ';');
                    traffic_tmp = value;
                    traffic.push_back(traffic_tmp);
                }
            }
            else if(value == "speed")
            {
                while(readFile.peek() != '\n')
                {
                    getline(readFile, value, ';');
                    averageSpeed_tmp = stof(value);
                    averageSpeed.push_back(averageSpeed_tmp);
                }
            }
            else if(value == "tank")
            {
                
                while(readFile.peek() != '\n')
                {
                    getline(readFile, value, ';');
                    tank_tmp = stof(value);
                    tank.push_back(tank_tmp);
                }
            }
            else if(value == "milage")
            {
                while(readFile.peek() != '\n')
                {
                    getline(readFile, value, ';');
                    milage_tmp = stof(value);
                    milage.push_back(milage_tmp);
                }
            }

            getline(readFile, value, '\n');
      
            if (readFile.peek() == '#')
            {  
                Drive *drive = new Drive(timestamp_tmp, tank, traffic, milage, averageSpeed);
                drives.push_back(drive);

                tank.clear();
                traffic.clear();
                milage.clear();
                averageSpeed.clear();
                
            }   
        }*/
    }
    else
        cout << "Reading " << file <<  " is not possible! " << endl;
    readFile.close();
}

void CentralControl::writeInFile(string file)
{
    int size = drives.size()-1;
    Drive *drive;    
    drive = drives[size];
    file = "cars/" + file + ".txt";
    
    ifstream OpenFile(file);
    if (OpenFile.good()) {        
        OpenFile.close();
    }
    else
    {
        ofstream WriteInFile; 
	WriteInFile.open(file, ios::out);
        
        WriteInFile << car << '\n';
        WriteInFile << '#' << '\n';
        
	WriteInFile.close();
    }
    
    ofstream WriteInFile; 
	WriteInFile.open(file, ios::out | ios::app); 
        
        WriteInFile << drive->getTimestamp() << '\n';
        
        if(drive->getTraffic().size() > 0)
            WriteInFile << "traffic:";
	for (unsigned int i = 0; i < drive->getTraffic().size(); i++)
	{      
            if(i ==  drive->getTraffic().size()-1)
                WriteInFile << drive->getTraffic()[i] << ";" << '\n';
            else
                WriteInFile << drive->getTraffic()[i] << ";";                                                 
	}
        
        if(drive->getAverageSpeed().size() > 0)
            WriteInFile << "speed:";
        for (unsigned int i = 0; i < drive->getAverageSpeed().size(); i++)
	{
            if(i ==  drive->getAverageSpeed().size()-1)
                WriteInFile << drive->getAverageSpeed()[i] << ";" << '\n';
            else
                WriteInFile << drive->getAverageSpeed()[i] << ";";                                       
	}
        
        if(drive->getTank().size() > 0)
            WriteInFile << "tank:";
        for (unsigned int i = 0; i < drive->getTank().size(); i++)
	{
            if(i ==  drive->getTank().size()-1)
                WriteInFile << drive->getTank()[i] << ";" << '\n';
            else
                WriteInFile << drive->getTank()[i] << ";";                                       
	}
        
        if(drive->getMilage().size() > 0)
            WriteInFile << "milage:";
        for (unsigned int i = 0; i < drive->getMilage().size(); i++)
	{
            if(i ==  drive->getMilage().size()-1)
                WriteInFile << drive->getMilage()[i] << ";" << '\n';
            else
                WriteInFile << drive->getMilage()[i] << ";";                                       
	}
        WriteInFile << '#' << '\n';
    WriteInFile.close();  
}

void CentralControl::addDrive(Drive* drive)
{
    drives.push_back(drive);
}

void CentralControl::addTraffic(string trafficNEW)
{
    traffic.push_back(trafficNEW);
}

void CentralControl::addSpeed(float speed)
{
    averageSpeed.push_back(speed);
}
void CentralControl::addTank(float tankNEW)
{
    tank.push_back(tankNEW);
}
void CentralControl::addMilage(float milageNEW)
{
    milage.push_back(milageNEW);
}

void CentralControl::clearTraffic()
{
    traffic.clear();
}

void CentralControl::clearSpeed()
{
    averageSpeed.clear();
}
void CentralControl::clearTank()
{
    tank.clear();
}
void CentralControl::clearMilage()
{
    milage.clear();
}

void CentralControl::setNumberPlate(string numberPlate) {
    this->numberPlate = numberPlate;
}

string CentralControl::getNumberPlate() const {
    return numberPlate;
}

void CentralControl::setCar(string car) {
    this->car = car;
}

string CentralControl::getCar() const {
    return car;
}

void CentralControl::setDrives(vector<Drive*> drives) {
    this->drives = drives;
}

vector<Drive*> CentralControl::getDrives() const {
    return drives;
}

void CentralControl::setTstartMilage(time_t tstartMilage) {
    this->tstartMilage = tstartMilage;
}

time_t CentralControl::getTstartMilage() const {
    return tstartMilage;
}

void CentralControl::setTstartTank(time_t tstartTank) {
    this->tstartTank = tstartTank;
}

time_t CentralControl::getTstartTank() const {
    return tstartTank;
}

void CentralControl::setTstartSpeed(time_t tstartSpeed) {
    this->tstartSpeed = tstartSpeed;
}

time_t CentralControl::getTstartSpeed() const {
    return tstartSpeed;
}

void CentralControl::setTstartTraffic(time_t tstartTraffic) {
    this->tstartTraffic = tstartTraffic;
}

time_t CentralControl::getTstartTraffic() const {
    return tstartTraffic;
}

void CentralControl::setAverageSpeed(vector<float> averageSpeed) {
    this->averageSpeed = averageSpeed;
}

vector<float> CentralControl::getAverageSpeed() const {
    return averageSpeed;
}

void CentralControl::setTank(vector<float> tank) {
    this->tank = tank;
}

vector<float> CentralControl::getTank() const {
    return tank;
}

void CentralControl::setMilage(vector<float> milage) {
    this->milage = milage;
}

vector<float> CentralControl::getMilage() const {
    return milage;
}

void CentralControl::setTraffic(vector<string> traffic) {
    this->traffic = traffic;
}

vector<string> CentralControl::getTraffic() const {
    return traffic;
}

