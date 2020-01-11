// Authors: Patrick Tinz and Janik Tinz

#include "Drive.h"

Drive::Drive() 
{
}

Drive::Drive(string timestamp, vector<float> tank, vector<string> traffic, vector<float> milage, vector<float> averageSpeed) 
{
    this->timestamp = timestamp;
    this->tank = tank;
    this->traffic = traffic;
    this->milage = milage;
    this->averageSpeed = averageSpeed;
}

Drive::~Drive() 
{
}

void Drive::setAverageSpeed(vector<float> averageSpeed) {
    this->averageSpeed = averageSpeed;
}

vector<float> Drive::getAverageSpeed() const {
    return averageSpeed;
}

void Drive::setTank(vector<float> tank) {
    this->tank = tank;
}

vector<float> Drive::getTank() const {
    return tank;
}

void Drive::setMilage(vector<float> milage) {
    this->milage = milage;
}

vector<float> Drive::getMilage() const {
    return milage;
}

void Drive::setTraffic(vector<string> traffic) {
    this->traffic = traffic;
}

vector<string> Drive::getTraffic() const {
    return traffic;
}

void Drive::setTimestamp(string timestamp) {
    this->timestamp = timestamp;
}

string Drive::getTimestamp() const {
    return timestamp;
}
