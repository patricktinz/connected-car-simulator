// Authors: Patrick Tinz and Janik Tinz

#include "Drive.h"

using namespace std;

Drive::Drive() 
{
}

Drive::Drive(string numberPlate, string car, string timestamp, vector<string> tank, vector<string> traffic, vector<string> milage, vector<string> speed)
{
    this->numberPlate = numberPlate;
    this->manufacturer = car;
    this->timestamp = timestamp;
    this->tank = tank;
    this->traffic = traffic;
    this->milage = milage;
    this->averageSpeed = speed;
}

Drive::~Drive() 
{
}

void Drive::addTraffic(string trafficNEW)
{
    traffic.push_back(trafficNEW);
}

void Drive::addSpeed(string speed)
{
    averageSpeed.push_back(speed);
}

void Drive::addTank(string tankNEW)
{
    tank.push_back(tankNEW);
}

void Drive::addMilage(string milageNEW)
{
    milage.push_back(milageNEW);
}

void Drive::clearTraffic()
{
    traffic.clear();
}

void Drive::clearSpeed()
{
    averageSpeed.clear();
}

void Drive::clearTank()
{
    tank.clear();
}

void Drive::clearMilage()
{
    milage.clear();
}

void Drive::setAverageSpeed(vector<string> averageSpeed) {
    this->averageSpeed = averageSpeed;
}

vector<string> Drive::getAverageSpeed(){
    return this->averageSpeed;
}

void Drive::setTank(vector<string> tank) {
    this->tank = tank;
}

vector<string> Drive::getTank(){
    return this->tank;
}

void Drive::setMilage(vector<string> milage) {
    this->milage = milage;
}

vector<string> Drive::getMilage(){
    return this->milage;
}

void Drive::setTraffic(vector<string> traffic) {
    this->traffic = traffic;
}

vector<string> Drive::getTraffic(){
    return this->traffic;
}

void Drive::setTimestamp(string timestamp) {
    this->timestamp = timestamp;
}

string Drive::getTimestamp(){
    return this->timestamp;
}

void Drive::setNumberPlate(string numberPlate) {
    this->numberPlate = numberPlate;
}

string Drive::getNumberPlate(){
    return this->numberPlate;
}

void Drive::setManufacturer(string manufacturer) {
    this->manufacturer = manufacturer;
}

string Drive::getManufacturer(){
    return this->manufacturer;
}