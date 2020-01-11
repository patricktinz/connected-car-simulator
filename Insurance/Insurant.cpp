// Authors: Patrick Tinz and Janik Tinz

#include "Insurant.h"

Insurant::Insurant() {
}

Insurant::~Insurant() {
}

void Insurant::SetCarType(string carType) {
    this->carType = carType;
}

string Insurant::GetCarType() const {
    return carType;
}

void Insurant::SetNumberPlate(string numberPlate) {
    this->numberPlate = numberPlate;
}

string Insurant::GetNumberPlate() const {
    return numberPlate;
}

