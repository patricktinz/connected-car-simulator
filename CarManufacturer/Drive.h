// Authors: Patrick Tinz and Janik Tinz

#include <string>
#include <vector>

using namespace std;

#ifndef DRIVE_H
#define DRIVE_H

class Drive {
public:
    Drive();
    Drive(string, string, string, vector<string>, vector<string>, vector<string>, vector<string> );
    virtual ~Drive();
    
    void addTraffic(string traffic);
    void addSpeed(string speed);
    void addTank(string tank);
    void addMilage(string milage);
    
    void clearTraffic();
    void clearSpeed();
    void clearTank();
    void clearMilage();
    
    void setAverageSpeed(vector<string> averageSpeed);
    vector<string> getAverageSpeed();
    void setTank(vector<string> tank);
    vector<string> getTank();
    void setMilage(vector<string> milage);
    vector<string> getMilage();
    void setTraffic(vector<string> traffic);
    vector<string> getTraffic();
    void setTimestamp(string timestamp);
    string getTimestamp();
    void setNumberPlate(string numberPlate);
    string getNumberPlate();
    void setManufacturer(string manufacturer);
    string getManufacturer();
    
private:
    string manufacturer;
    string numberPlate;
    string timestamp;
    vector<string> traffic;
    vector<string> milage;
    vector<string> tank;
    vector<string> averageSpeed;
};

#endif /* DRIVE_H */

