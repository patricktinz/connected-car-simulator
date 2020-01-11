// Authors: Patrick Tinz and Janik Tinz

#include "Sensor.h"
#include <iostream>
#include <cstdlib>
#include <string>

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

int main(int argc, char** argv) 
{
    string ipServer;
    string sensType;
    unsigned int choice;
    
    cout << "IP-Server: ";
    cin >> ipServer;
    
    while(choice != 1 && choice != 2 && choice != 3 && choice != 4)
    {
        cout << "#### Sensor type ####" << endl;
        cout << "(1) Traffic" << endl;
        cout << "(2) Speed" << endl;
        cout << "(3) Tank" << endl;
        cout << "(4) Milage" << endl;
        check_input(choice);
        
        switch(choice)
        {
            case 1:
                sensType = "traffic";
                break;
            case 2:
                sensType = "speed";
                break;
            case 3:
                sensType = "tank";
                break;
            case 4:
                sensType = "milage";
                break;
            default: 
                cout << "!!!Invalid input!!!" << endl;
                break;
        }      
    }
    
    Sensor *sensor = new Sensor(ipServer, sensType);
    sensor->receivedUDP();

    return 0;
}

