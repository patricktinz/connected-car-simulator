// Authors: Patrick Tinz and Janik Tinz

#include "CentralControl.h"
#include <vector>
using namespace std;

#ifndef MENU_H
#define MENU_H

class Menu {
public:
    Menu();
    virtual ~Menu();
    int mainMenu();
    void performanceTest();
    bool numberPlateExists(string);
    void saveCars();
    void writeSettingsMQTTInFile(string, string, unsigned int);
    void submenuSettingsMQTT();
    void check_input(unsigned int &);
       
private:
    vector<string> cars;
};

#endif /* MENU_H */

