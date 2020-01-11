// Authors: Patrick Tinz and Janik Tinz

#include <string>
#include <map>
#include "Insurant.h"

using namespace std;

#ifndef INSURANCE_H
#define INSURANCE_H

class Insurance {
public:
    Insurance();
    virtual ~Insurance();
    
    void apacheThriftClient(string, string);
    void print(string);
    
    void setNameInsurance(string);
    string getNameInsurance() const;
    void setInsurant(map<string, Insurant*> insurant);
    map<string, Insurant*> getInsurant() const;
    void deleteInsurant(string numberPlate);
    void addInsurant(Insurant*);
    
private:
    string nameInsurance;
    map<string, Insurant*> insurant;
};

#endif /* INSURANCE_H */

