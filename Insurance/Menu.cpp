// Authors: Patrick Tinz and Janik Tinz

#include "Menu.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include "Insurance.h"

using namespace std;

Menu::Menu() 
{
    ifstream inputIP("ip-server.txt");
    string lineIP;
    while(!inputIP.eof())
    {
        getline(inputIP, lineIP);
        serverIP.push_back(lineIP); 
        
        if(inputIP.peek() == '\n')
            break;
    }
    
    ifstream inputPort("port-server.txt");
    string linePort;
    while(!inputPort.eof())
    {
        getline(inputPort, linePort);
        int port = atoi(linePort.c_str());
        serverPort.push_back(port); 
        
        if(inputPort.peek() == '\n')
            break;
    }
}

Menu::~Menu() 
{
    ofstream outputIP("ip-server.txt");
    for(int i = 0; i < serverIP.size(); i++)
    {
        outputIP << serverIP[i];
        outputIP.put('\n');
    }
    
    ofstream outputPort("port-server.txt");
    for(int i = 0; i < serverIP.size(); i++)
    {
        outputPort << serverPort[i];
        outputPort.put('\n');
    }
}

Menu& Menu::getInstance()
{
    static Menu instance;
    return instance;
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

void Menu::submenuStartInsurance()
{
    Insurance *insurance = new Insurance();
    unsigned int input = 2;
    system("clear");
    cout << "#####################" << endl;
    cout << "   Start Insurance   " << endl;
    cout << "#####################" << endl;
    cout << "Choose an insurance: " << endl;
    
    int i;
    for(i = 0; i < insurances.size(); i++)
    {
        cout << "(" << i+1 << ") " << insurances.at(i)->getNameInsurance() << endl;
    }
    
    cout << "(" << i+1 << ") " << "Back" << endl;
    check_input(input);
    cout << endl;
    
    if(input < insurances.size()+1)
    {
        unsigned int choice = 0;
        cout << "Choose an insurant: " << endl;
        
        map<unsigned int,Insurant*> actualInsurant;   // all insurants
        unsigned int j = 0;
        for(map<string, Insurant*>::iterator it = insurances.at(input-1)->getInsurant().begin(); it !=insurances.at(input-1)->getInsurant().end(); ++it)
        {
            cout << "(" << ++j << ") " << it->first << endl;
            actualInsurant.insert(pair<unsigned int, Insurant*>(j, it->second));           
        }
        check_input(choice);
        cout << endl;
        insurance->apacheThriftClient(actualInsurant.at(choice)->GetCarType(), actualInsurant.at(choice)->GetNumberPlate());
    }
}

void Menu::submenuAddInsurance()
{
    string name;
    cout << "Enter name of the insurance: " << endl;
    cin >> name;
    Insurance *insur = new Insurance();
    insur->setNameInsurance(name);
    insurances.push_back(insur);
}

void Menu::submenuEditInsurance()
{
    unsigned int input = 2;
    system("clear");
    cout << "###################" << endl;
    cout << "   Edit Insurance   " << endl;
    cout << "###################" << endl;
    cout << "Choose an insurance to edit: " << endl;
    
    int i;
    for(i = 0; i < insurances.size(); i++)
    {
        cout << "(" << i+1 << ") " << insurances.at(i)->getNameInsurance() << endl;
    }
    cout << "(" << i+1 << ") " << "Back" << endl;
    check_input(input);
    cout << endl;
    
    if(input < insurances.size()+1)
    {
        unsigned int choice = 3;
        bool whileValue = true;
        while(whileValue){
            system("clear");
            cout << "########################" << endl;
            cout << "   Add/ Edit Insurant   " << endl;
            cout << "########################" << endl;
            cout << "(1) Add insurant" << endl;
            cout << "(2) Edit insurant" << endl;
            cout << "(3) Delete insurant " << endl;
            cout << "(4) Back" << endl;
            check_input(choice);
            
            switch (choice)
            {
                case 1:
                {
                    string numberPlate, manufacturer;
                    cout << "Add manufacturer: " << endl;
                    cin >> manufacturer;

                    cout << "Add number plate (e.g.: XX-XX-1234): " << endl;
                    cin >> numberPlate;
                    Insurant* insurant = new Insurant();
                    insurant->SetCarType(manufacturer);
                    insurant->SetNumberPlate(numberPlate);
                    insurances.at(input-1)->addInsurant(insurant);
                }
                    break;
                case 2:
                {
                    try{
                        string numberPlate;
                        cout << "Enter the number plate: " << endl;
                        cin >> numberPlate;

                        string manufacturerNew;
                        string carType = insurances.at(input-1)->getInsurant().at(numberPlate)->GetCarType();
                        cout << "Edit manufacturer (actual: " << carType << "): " << endl;
                        cin >> manufacturerNew;
                        insurances.at(input-1)->getInsurant().at(numberPlate)->SetCarType(manufacturerNew);                       

                        string numberPlateNew;
                        string numberPlateOld = insurances.at(input-1)->getInsurant().at(numberPlate)->GetNumberPlate();
                        cout << "Edit numberplate (actual: " << numberPlateOld << "): " << endl;
                        cin >> numberPlateNew;
                        insurances.at(input-1)->getInsurant().at(numberPlate)->SetNumberPlate(numberPlateNew);
                    }
                    catch(...)
                    {
                        cout << "Wrong number plate!!!" << endl;
                    }
                    cout << "Press -Enter- to continue..." << endl;
                    cin.ignore();
                    cin.ignore();
                }
                    break;
                case 3:
                {
                    try{
                        string numberPlate;
                        cout << "Enter the number plate: " << endl;
                        cin >> numberPlate;

                        insurances.at(input-1)->deleteInsurant(numberPlate);
                    }
                    catch(...)
                    {
                        cout << "Wrong number plate!!!" << endl;
                    }
                    cout << "Press -Enter- to continue..." << endl;
                    cin.ignore();
                    cin.ignore();
                }
                    break;
                case 4:
                    whileValue = false;
                    break;
                default:
                    cout << "!!!Invalid input!!!" << endl;
                    break;
            }
        }
    }
    else
    {
        return;
    }
}

void Menu::submenuDeleteInsurance()
{
    unsigned int input = 2;
    system("clear");
    cout << "#####################" << endl;
    cout << "   Delete insurance   " << endl;
    cout << "#####################" << endl;
    cout << "Choose an insurance: " << endl;
    
    int i;
    for(i = 0; i < insurances.size(); i++)
    {
        cout << "(" << i+1 << ") " << insurances.at(i)->getNameInsurance() << endl;
    }
    
    check_input(input);
    cout << endl;
    
    if(input < insurances.size()+1)
    {       
        string name = insurances.at(input-1)->getNameInsurance();
        int index = ((int)input) - 2;
        insurances.erase(insurances.begin()+index);          
        cout << "Delete insurance " <<  name << " successful. " << endl;
    }
    else
        cout << "!!!Invalid input!!!" << endl;  
    
    cout << "Press -Enter- to continue..." << endl;
    cin.ignore();
    cin.ignore();
}

void Menu::submenuSettings()
{
    unsigned int input = 2;
    system("clear");
    cout << "##############" << endl;
    cout << "   Settings   " << endl;
    cout << "##############" << endl;
    cout << "(1) Add server" << endl;
    cout << "(2) Edit server" << endl;
    cout << "(3) Back" << endl;
    check_input(input);
    cout << endl;
    
    switch(input)
    {
        case 1:
            {
                string ip; 
                int port;
                
                cout << "IP-Address server: " << endl;
                cin >> ip;
                serverIP.push_back(ip);
                cout << "Port: " << endl;
                cin >> port;
                serverPort.push_back(port);
            }
            break;
        case 2:
            {
                unsigned int input;
                do{
                    cout << "Edit server: " << endl;
                    cout << "(0) Back" << endl;
                    
                    for(int i = 0; i < serverIP.size(); i++)
                    {
                        cout << "(" << i+1 << ") " << serverIP.at(i) << ", " << serverPort.at(i) << endl;                       
                    }
                    check_input(input);
                    
                    if(input != 0)
                    {
                        int value;
                        string tmp = "";
                        
                        cout << "IP-Server: ";
			cin.ignore();
			getline(cin, tmp);
			if (tmp != "")
                            serverIP.at(input-1) = tmp;
                        
                        cout << "Port-Server: ";
			cin >> value;                       
                        serverPort.at(input-1) = value;
                        
                    }

                }while(input != 0);
            }
            break;
        case 3:
            return;
        default:
            cout << "!!!Invalid input!!!" << endl;
            break;
    }
}

void Menu::mainMenu() 
{
    unsigned int input = 5;
    
    while(true)
    {
        system("clear");
        cout << "###################" << endl;
        cout << "   Car Insurance   " << endl;
        cout << "###################" << endl;
        cout << "(1) Start" << endl;
        cout << "(2) Add insurance" << endl;
        cout << "(3) Edit insurance" << endl;
        cout << "(4) Delete insurance" << endl;
        cout << "(5) Settings" << endl;
        cout << "(6) Exit" << endl;
        check_input(input);
        cout << endl;
        
        switch(input)
        {
            case 1: 
                submenuStartInsurance();              
                break;
            case 2:
                submenuAddInsurance();
                break;
            case 3:
                submenuEditInsurance();
                break;
            case 4:
                submenuDeleteInsurance();
                break;
            case 5:
                submenuSettings();
                break;
            case 6:
                return;
            default:
                cout << "!!!Invalid input!!!" << endl;
                break;
        }
    }    
}

void Menu::setServerPort(vector<int> serverPort) {
    this->serverPort = serverPort;
}

vector<int> Menu::getServerPort() const {
    return serverPort;
}

void Menu::setServerIP(vector<string> serverIP) {
    this->serverIP = serverIP;
}

vector<string> Menu::getServerIP() const {
    return serverIP;
}