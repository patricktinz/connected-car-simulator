// Authors: Patrick Tinz and Janik Tinz

#include <string>

using namespace std;

#ifndef CARSIMULATOR_H
#define CARSIMULATOR_H

class CarSimulator {
public:
    CarSimulator();
    virtual ~CarSimulator();
    void sendUDP(string, string, const int);
    void send(string, const int);
    void startSimulator(int);  
    void startSimulator();
    void check_input(unsigned int &);
    
    void setIpMilage(string ipMilage);
    string getIpMilage() const;
    void setIpTank(string ipTank);
    string getIpTank() const;
    void setIpSpeed(string ipSpeed);
    string getIpSpeed() const;
    void setIpTraffic(string ipTraffic);
    string getIpTraffic() const;

private:
    string ipTraffic;
    string ipSpeed;
    string ipTank;
    string ipMilage;
};

#endif /* CARSIMULATOR_H */

