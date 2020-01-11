// Authors: Patrick Tinz and Janik Tinz

#include <string>
#include <vector>
#include "Insurance.h"

using namespace std;

#ifndef MENU_H
#define MENU_H

class Menu {
public:
    static Menu& getInstance();
    virtual ~Menu();
    
    void check_input(unsigned int &choice);
    void submenuStartInsurance();
    void submenuAddInsurance();
    void submenuEditInsurance();
    void submenuDeleteInsurance();
    void submenuSettings();
    void mainMenu();
    void setServerPort(vector<int> serverPort);
    vector<int> getServerPort() const;
    void setServerIP(vector<string> serverIP);
    vector<string> getServerIP() const;
    
private:
    Menu();
    vector<Insurance*> insurances;
    vector<string> serverIP;
    vector <int> serverPort;
};

#endif /* MENU_H */

