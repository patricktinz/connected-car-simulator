// Authors: Patrick Tinz and Janik Tinz

#include <string>

using namespace std;

#ifndef INSURANT_H
#define INSURANT_H

class Insurant {
public:
    Insurant();
    virtual ~Insurant();
    void SetCarType(string carType);
    string GetCarType() const;
    void SetNumberPlate(string numberPlate);
    string GetNumberPlate() const;
    
private:
    string numberPlate;
    string carType;
};

#endif /* INSURANT_H */

