// Authors: Patrick Tinz and Janik Tinz

#include "Data.h"

Data::Data() 
{
}

Data::~Data() 
{
}

void Data::SetDrives(map<string, Drive*> drives) 
{
    this->drives = drives;
}

map<string, Drive*> Data::GetDrives() const 
{
    return this->drives;
}

void Data::addDrive(Drive* driveNew)
{
    drives.insert(pair<string, Drive*>(driveNew->getTimestamp(), driveNew));
}

