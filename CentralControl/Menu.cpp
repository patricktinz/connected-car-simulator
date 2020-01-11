// Authors: Patrick Tinz and Janik Tinz

#include "Menu.h"
#include "CentralControl.h"
#include "UDPServer.h"
#include "Parameter.h"
#include "MQTTPublisher.h"
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <fstream>

using namespace std;

Menu::Menu() {
    ifstream input("numberPlate.txt");
    for (string line; getline(input, line);)
    {
        cars.push_back(line);  
    }
}

Menu::~Menu() 
{   
}

void Menu::performanceTest()
{
    time_t input;
    cout << "Test duration (sec): " << endl;
    cin >> input;
    // Cancel with ctrl+c
    try
    {   
        boost::asio::io_service io_service;
        UDPServer s(io_service, SERVER_PORT);
        
        // test
        s.setTest(true);
        s.setEndTimeTest(input);
        
        io_service.run();
    }
    catch (std::exception& e)
    {
      std::cerr << "Exception: " << e.what() << "\n";
    }
}

void Menu::saveCars()
{
    ofstream output("numberPlate.txt");
    for(int i=0; i<cars.size(); i++){
        output << cars[i];
        output.put('\n');
    }
}

bool Menu::numberPlateExists(string numberPlate)
{
    vector<string>::iterator it;
    it = find(cars.begin(), cars.end(), numberPlate);
    if(it != cars.end())
    {
        return true;
    }else
    {
        return false;
    }
}

void Menu::writeSettingsMQTTInFile(string ipBroker, string topic, unsigned int qos)
{
    ofstream output("settingsMQTT.txt");
    
    output << ipBroker;
    output.put('\n');
    
    output << topic;
    output.put('\n');
    
    output << qos;
    output.put('\n');
}

void Menu::submenuSettingsMQTT()
{
    string ipBroker = "127.0.0.1";
    string topic = "data/sensor";
    unsigned int qos = 3;
    while(true)
    {
        system("clear");
        unsigned int input = 2;    
        
        cout << "############################" << endl;
        cout << "        Settings MQTT       " << endl;
        cout << "############################" << endl;
        cout << "(1) Edit IP-address Broker" << endl;
        cout << "(2) Edit topic" << endl;
        cout << "(3) Edit QoS" << endl;
        cout << "(4) Back" << endl;
        check_input(input);
        
        switch(input)
        {
            case 1:
                cout << "IP-address Broker: " << endl;
                cin >> ipBroker;
                ipBroker = "tcp://" + ipBroker + ":1883";
                
                break;
            case 2:
                cout << "Topic: " << endl;
                cin >> topic;
                break;
            case 3:
            {               
                while(qos > 2)
                {
                    cout << "### Edit QoS ###" << endl;
                    cout << "(0) QoS = 0: Fire and forget" << endl;
                    cout << "(1) QoS = 1: Message is received least once" << endl;
                    cout << "(2) QoS = 2: Message is received exactly once" << endl;
                    check_input(qos);                    
                }
                break; 
            }
            case 4: 
                writeSettingsMQTTInFile(ipBroker, topic, qos);
                return;
            default:
                cout << "!!!Invalid input!!!" << endl;
                break;
        }
    }
}

int Menu::mainMenu()
{
    unsigned int input = 3;
    string numberPlate = "default";
    string carType;
    
    CentralControl &centralControl = CentralControl::getInstance();
    centralControl.setNumberPlate(numberPlate);
    
    
    
    while(true)
    {
        system("clear");
        cout << "################################################" << endl;
        cout << "                 Central Control                " << endl;
        cout << "################################################" << endl;
        cout << "(1) Start Central Control" << endl;
        cout << "(2) Performance Test" << endl;
        cout << "(3) MQTT Settings" << endl;
        cout << "(4) Exit" << endl;
        check_input(input);
        
        switch(input)
        {
            case 1:
            {
                unsigned int choice = 0;
                
                for(int i=0; i < cars.size(); i++)
                {
                    cout << "(" << i+1 << ") " << cars[i] << endl;
                    if(i == cars.size() -1)
                    {
                       cout << "(" << i+2 << ") " << "New Number Plate " << endl;
                    }
                }
                
                if(cars.size() > 0){
                    check_input(choice);
                }
                
                if(choice-1 < cars.size())
                {
                    numberPlate = cars[choice-1];
                }else{
                    system("clear");
                    cout << "New Number Plate: " << endl;
                    cin >> numberPlate;
                }
                
                centralControl.setNumberPlate(numberPlate);
                
                if(numberPlateExists(numberPlate) == false)
                {
                    cout << "Car Type: " << endl;
                    cin >> carType;
                    centralControl.setCar(carType);
                    cars.push_back(numberPlate);
                }
                
                centralControl.readFile(numberPlate);
                saveCars();
                centralControl.receivedUDP();
                centralControl.writeInFile(numberPlate);
            }
                break;
            case 2:
                performanceTest();
                break;
            case 3:
                submenuSettingsMQTT();
                break;
            case 4:
                return 0;
                break;
            default:
                cout << "!!!Invalid input!!!" << endl;
                break;
        }
    }
    return 0;
}

void Menu::check_input(unsigned int &choice)
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