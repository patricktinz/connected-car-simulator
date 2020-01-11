// Authors: Patrick Tinz and Janik Tinz

#include "CarSimulator.h"
#include <iostream>
#include <unistd.h>

using namespace std;

void check_input(unsigned int &choice)
{
    cout << "Entry > ";
    cin >> choice;
    while (!cin)   // if choice is no number
    {
        try
        {
            throw "- Wrong input! Enter a number -";
        }
        catch (const char* str)
        {
            cout << str << endl;
        }
        cin.clear();
        cin.ignore();

        cout << "Entry > ";
        cin >> choice;
    }
}

void submenuSettings(CarSimulator *car)
{
    while(true)
    {
        system("clear");
        unsigned int input = 5;
        string ipTraffic;
        string ipSpeed;
        string ipTank;
        string ipMilage;
        
        cout << "##################################" << endl;
        cout << "             Settings             " << endl;
        cout << "##################################" << endl;
        cout << "(1) Edit IP-address sensor traffic" << endl;
        cout << "(2) Edit IP-address sensor speed" << endl;
        cout << "(3) Edit IP-address sensor tank" << endl;
        cout << "(4) Edit IP-address sensor milage" << endl;
        cout << "(5) Back" << endl;
        check_input(input);
        
        switch(input)
        {
            case 1:
                cout << "IP-address sensor traffic: " << endl;
                cin >> ipTraffic;
                car->setIpTraffic(ipTraffic);
                break;
            case 2:
                cout << "IP-address sensor speed: " << endl;
                cin >> ipSpeed;
                car->setIpSpeed(ipSpeed);
                break;
            case 3:
                cout << "IP-address sensor tank: " << endl;
                cin >> ipTank;
                car->setIpTank(ipTank);
                break;
            case 4:
                cout << "IP-address sensor milage: " << endl;
                cin >> ipMilage;
                car->setIpMilage(ipMilage); 
                break;
            case 5: 
                return;
            default:
                cout << "!!!Invalid input!!!" << endl;
                break;
        }
    }
}

int generateRandomKilometers()
{
    srand((unsigned)time(0));
    return((rand()%2000)+10);
}

int main(int argc, char** argv) {
    
    unsigned int input = 4;
    int kilometers;
    CarSimulator *car = new CarSimulator();
    
    while(true)
    {
        system("clear");
        cout << "###################" << endl;
        cout << "   Car Simulator   " << endl;
        cout << "###################" << endl;
        cout << "(1) Start" << endl;
        cout << "(2) Test" << endl;
        cout << "(3) Settings" << endl;
        cout << "(4) Exit" << endl;
        check_input(input);
        cout << endl;
        
        switch(input)
        {
            case 1: 
                system("clear");
                kilometers = generateRandomKilometers();
                car->startSimulator(kilometers);  // start simulation
                break;
            case 2:
                car->startSimulator();
                break;
            case 3:
                submenuSettings(car);
                break;
            case 4:
                delete car;
                return 0;
                break;
            default:
                cout << "!!!Invalid input!!!" << endl;
                break;
        }
    } 
    return 0;
}

