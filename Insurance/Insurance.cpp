// Authors: Patrick Tinz and Janik Tinz

#include "Insurance.h"
#include <iostream>

#include "gen-cpp/Calc.h"
#include "Menu.h"

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <cassert>
#include <exception>
#include <sstream>


using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;


Insurance::Insurance() {
}

Insurance::~Insurance() {
}

void Insurance::apacheThriftClient(string carType, string numberPlate)
{
    Menu &menu = Menu::getInstance();
    string jsonData = "No manufacturer server available!";
    for(int i = 0; i < menu.getServerIP().size(); i++)
    {
        jsonData = "No manufacturer server available!";
        try{
        shared_ptr<TSocket> socket(new TSocket(menu.getServerIP().at(i), menu.getServerPort().at(i)));
        shared_ptr<TTransport> transport(new TBufferedTransport(socket));
        shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
        CalcClient calc(protocol);
        socket->setConnTimeout(10000);
        transport->open();
        
        Result myResult;

        calc.carStatus(myResult, carType, numberPlate, "0");
        
        jsonData = myResult.result;
        
        transport->close();  
        
        if(jsonData != "No manufacturer server available!")
            break;
        } catch (TTransportException e) {
            continue;
        } catch (TException e) {
            continue;
        }  
    }
    
    print(jsonData);
    
    cout << "Press -Enter- to continue..." << endl;
    cin.ignore();
    cin.ignore();
}

 void Insurance::print(string jsonData)
 {
     cout << jsonData << endl;
 }

void Insurance::setNameInsurance(string nameInsurance) {
    this->nameInsurance = nameInsurance;
}

string Insurance::getNameInsurance() const {
    return nameInsurance;
}

void Insurance::setInsurant(map<string, Insurant*> insurant) {
    this->insurant = insurant;
}

map<string, Insurant*> Insurance::getInsurant() const {
    return insurant;
}

void Insurance::deleteInsurant(string numberPlate)
{
    insurant.erase(numberPlate);
}

void Insurance::addInsurant(Insurant* insurantNew)
{
    insurant.insert(pair<string, Insurant*>(insurantNew->GetNumberPlate(), insurantNew));
}