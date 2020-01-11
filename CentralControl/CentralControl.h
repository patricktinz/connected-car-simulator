// Authors: Patrick Tinz and Janik Tinz

#include "Drive.h"
#include <vector>
#include <string>
#include <map>
#include <boost/asio.hpp>
#include <time.h>

using boost::asio::ip::udp;
using namespace std;

#ifndef CENTRALCONTROL_H
#define CENTRALCONTROL_H

class CentralControl {
public:
    static CentralControl& getInstance();
    virtual ~CentralControl();

    string generateDate();
    void startTimer();
    void receivedUDP();
    void readFile(string);
    void writeInFile(string);
    void addDrive(Drive*);
    
    void addTraffic(string traffic);
    void addSpeed(float speed);
    void addTank(float tank);
    void addMilage(float milage);
    
    void clearTraffic();
    void clearSpeed();
    void clearTank();
    void clearMilage();
    
    void setNumberPlate(string numberPlate);
    string getNumberPlate() const;
    void setCar(string car);
    string getCar() const;
    void setDrives(vector<Drive*> drives);
    vector<Drive*> getDrives() const;
    void setTstartMilage(time_t tstartMilage);
    time_t getTstartMilage() const;
    void setTstartTank(time_t tstartTank);
    time_t getTstartTank() const;
    void setTstartSpeed(time_t tstartSpeed);
    time_t getTstartSpeed() const;
    void setTstartTraffic(time_t tstartTraffic);
    time_t getTstartTraffic() const;
    
    void setAverageSpeed(vector<float> averageSpeed);
    vector<float> getAverageSpeed() const;
    void setTank(vector<float> tank);
    vector<float> getTank() const;
    void setMilage(vector<float> milage);
    vector<float> getMilage() const;
    void setTraffic(vector<string> traffic);
    vector<string> getTraffic() const;   

private:
    CentralControl();
    vector<Drive*> drives;
    string car;
    string numberPlate;
    time_t tstartTraffic;
    time_t tstartSpeed;
    time_t tstartTank;
    time_t tstartMilage;
    
    vector<string> traffic;
    vector<float> milage;
    vector<float> tank;
    vector<float> averageSpeed;
};

#endif /* CENTRALCONTROL_H */

