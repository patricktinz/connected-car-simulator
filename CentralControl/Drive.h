// Authors: Patrick Tinz and Janik Tinz

#include <vector>
#include <string>
using namespace std;

#ifndef DRIVE_H
#define DRIVE_H

class Drive {
public:
    Drive();
    Drive(string, vector<float>, vector<string>, vector<float>, vector<float>);
    virtual ~Drive();
    
    void setAverageSpeed(vector<float> averageSpeed);
    vector<float> getAverageSpeed() const;
    void setTank(vector<float> tank);
    vector<float> getTank() const;
    void setMilage(vector<float> milage);
    vector<float> getMilage() const;
    void setTraffic(vector<string> traffic);
    vector<string> getTraffic() const;
    void setTimestamp(string timestamp);
    string getTimestamp() const;
private:
    vector<string> traffic;
    vector<float> milage;
    vector<float> tank;
    vector<float> averageSpeed;
    string timestamp;
};

#endif /* DRIVE_H */

