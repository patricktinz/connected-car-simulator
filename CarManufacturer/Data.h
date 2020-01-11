// Authors: Patrick Tinz and Janik Tinz

#include "Drive.h"
#include <string>
#include <map>

using namespace std;

#ifndef DATA_H
#define DATA_H

class Data {
public:
    Data();   
    virtual ~Data();
    void SetDrives(map<string, Drive*> drives);
    map<string, Drive*> GetDrives() const;
    void addDrive(Drive*);
    
private:
    map<string, Drive*> drives;
    
};

#endif /* DATA_H */

