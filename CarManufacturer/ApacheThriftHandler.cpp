// Authors: Patrick Tinz and Janik Tinz

#include <iostream>
#include "ApacheThriftHandler.h"
#include "Manufacturer.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <cassert>
#include <exception>
#include <sstream>
#include <map>
#include <set>
#include <string>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include "Data.h"
#include "Drive.h"

using namespace std;

ApacheThriftHandler::ApacheThriftHandler() 
{
}

ApacheThriftHandler::~ApacheThriftHandler() 
{
}

void ApacheThriftHandler::readFile(string carType, string numberPlate)   // insert car data
{  
    Data *dataObject = new Data();
    Manufacturer& manu = Manufacturer::getInstance();
    string timestamp_tmp;
    
    vector<string> traffic;
    vector<string> milage;
    vector<string> tank;
    vector<string> averageSpeed;
    
    data.clear();
    
    string filename = carType + "/server " + manu.getIdServer() + "/" + numberPlate + ".txt";
    ifstream readFile;
    readFile.open(filename, ios::in);

    if (readFile)
    {
        string car_tmp, car_value;
        getline(readFile, car_value, '\n');
        car_tmp = car_value; 

        while (true)
        {
            string tank_tmp, milage_tmp, averageSpeed_tmp;
            string traffic_tmp, value;
            if (readFile.peek() == '#')
            {
                getline(readFile, value, '\n');
                getline(readFile, value, '\n');
                if(readFile.peek() == EOF)
                {
                    break;
                }
                
                timestamp_tmp = value;                  
            }  

            getline(readFile, value, ':');
            
            if(value == "traffic")
            {
                while(readFile.peek() != '\n')
                {
                    getline(readFile, value, ';');
                    traffic_tmp = value;
                    traffic.push_back(traffic_tmp);
                }
            }
            else if(value == "speed")
            {
                while(readFile.peek() != '\n')
                {
                    getline(readFile, value, ';');
                    averageSpeed_tmp = value;
                    averageSpeed.push_back(averageSpeed_tmp);
                }
            }
            else if(value == "tank")
            {
                
                while(readFile.peek() != '\n')
                {
                    getline(readFile, value, ';');
                    tank_tmp = value;
                    tank.push_back(tank_tmp);
                }
            }
            else if(value == "milage")
            {
                while(readFile.peek() != '\n')
                {
                    getline(readFile, value, ';');
                    milage_tmp = value;
                    milage.push_back(milage_tmp);
                }
            }

            getline(readFile, value, '\n');
      
            if (readFile.peek() == '#')
            {  
                if(tank.size() == 0)
                    tank.push_back("");
                if(traffic.size() == 0)
                    traffic.push_back("");
                if(milage.size() == 0)
                    milage.push_back("");
                if(averageSpeed.size() == 0)
                    averageSpeed.push_back("");
                Drive *drive = new Drive(numberPlate, carType, timestamp_tmp, tank, traffic, milage, averageSpeed);
                dataObject->addDrive(drive);

                // Debug
                /*cout << numberPlate << endl;
                cout << car_tmp << endl;
                cout << timestamp_tmp << endl;
                cout << "Tank:" << endl;
                for (int i = 0; i < tank.size(); i++)
                {
                    cout << tank[i] << " ";
                }
                cout << endl;
                cout << "Traffic:" << endl;
                for (int i = 0; i < traffic.size(); i++)
                {
                    cout << traffic[i] << " ";
                }
                cout << endl;
                cout << "Milage:" << endl;
                for (int i = 0; i < milage.size(); i++)
                {
                    cout << milage[i] << " ";
                }
                cout << endl;
                cout << "Speed:" << endl;
                for (int i = 0; i < averageSpeed.size(); i++)
                {
                    cout << averageSpeed[i] << " ";
                }
                cout << endl;*/
                
             
                tank.clear();
                traffic.clear();
                milage.clear();
                averageSpeed.clear();               
            }   
        }
    }
    else
        cout << "Reading " << numberPlate <<  " is not possible! " << endl;
    readFile.close();
    data.push_back(dataObject);
}

void ApacheThriftHandler::ping() 
{
    // Your implementation goes here
    printf("ping\n");
}

void ApacheThriftHandler::calculateResult()
{
    set<string> driveID;
    for(int i = 0; i < data.size(); i++)
    {
        map<string, Drive*> mymap = data[i]->GetDrives();
        map<string, Drive*>::iterator it;
        for(it = mymap.begin(); it != mymap.end(); ++it)
        { 
            driveID.insert(it->first);
        }
    }
    
    Data *dataObject = new Data();
    set<string>::iterator setIter;
    for(setIter = driveID.begin(); setIter != driveID.end(); ++setIter)
    {
        for(int i = 0; i < data.size(); i++)
        {
            if(data.at(i)->GetDrives().count(*setIter) > 0)
            {
                Drive *newDrive = new Drive();
                newDrive = data.at(i)->GetDrives().at(*setIter);
                dataObject->addDrive(newDrive);
                break;
            }
        }
    } 
    data.push_back(dataObject);
}

string ApacheThriftHandler::generateJsonString(string serverID)
{   
    stringstream ss;
    namespace pt = boost::property_tree;
    pt::ptree root;
    //Drive* drive = new Drive();    
    try
    {
        bool start = true;
        int i = 0;
        if(serverID == "0")
        {
            int dataVectorSize = data.size();
            i = dataVectorSize - 1;
        }
        
        if(serverID == "1")
            i = 0;
        
        map<string, Drive*> mymap = data[i]->GetDrives();
        
        map<string, Drive*>::iterator it;
        for(it = mymap.begin(); it != mymap.end(); ++it)
        {   
            Drive* drive = it->second;
            if(start == true)
            {
                string num = drive->getNumberPlate();
                root.put("numberPlate", num);
                string manuNew = drive->getManufacturer();
                root.put("manufacturer", manuNew);
                start = false;
            }  
            
            pt::ptree timestamp_node;   // date with data

            // traffic
            pt::ptree traffic_node;
            for (int i = 0; i < drive->getTraffic().size(); i++)
            {
                // Create an unnamed node containing the value
                pt::ptree data_node;
                string traffic = drive->getTraffic().at(i);
                data_node.put("", traffic);

                // Add this node to the list.
                traffic_node.push_back(std::make_pair("", data_node));
            }
            timestamp_node.add_child("traffic", traffic_node);

            // tank
            pt::ptree tank_node;
            for (int i = 0; i < drive->getTank().size(); i++)
            {
                // Create an unnamed node containing the value
                pt::ptree data_node;
                string tank = drive->getTank().at(i);
                data_node.put("", tank);

                // Add this node to the list.
                tank_node.push_back(std::make_pair("", data_node));
            }
            timestamp_node.add_child("tank", tank_node);

            // milage
            pt::ptree milage_node;
            for (int i = 0; i < drive->getMilage().size(); i++)
            {
                // Create an unnamed node containing the value
                pt::ptree data_node;
                string milage = drive->getMilage().at(i);
                data_node.put("", milage);

                // Add this node to the list.
                milage_node.push_back(std::make_pair("", data_node));
            }
            timestamp_node.add_child("milage", milage_node);

            // speed
            pt::ptree speed_node;
            for (int i = 0; i < drive->getAverageSpeed().size(); i++)
            {
                // Create an unnamed node containing the value
                pt::ptree data_node;
                string speed = drive->getAverageSpeed().at(i); 
                data_node.put("", speed);

                // Add this node to the list.
                speed_node.push_back(std::make_pair("", data_node));
            }
            timestamp_node.add_child("speed", speed_node);

            // Add the new node to the root
            root.add_child(drive->getTimestamp(), timestamp_node);
        }

        pt::write_json(ss, root);   
    }
    catch (std::exception const& e)
    {
        std::cerr << e.what() << std::endl;
    }
    
    string result = ss.str();   
    
    return result;
}

void ApacheThriftHandler::readJson(string jsonString)
{
    try
    {
        string value, numberPlate, manufacturer;
        Data *dataObject = new Data();
        stringstream sNeW;        
        sNeW.str(jsonString);
        
        // Debug
        //cout << sNeW.str() << endl;
        
        boost::property_tree::ptree pt;
        boost::property_tree::read_json(sNeW, pt);

        BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt)
        {   
            value = v.second.data();
            if(v.first == "numberPlate")
            {
                numberPlate = value;
            }
            else if(v.first == "manufacturer")
            {
                manufacturer = value;
            }
            else
            {
                string date = v.first;
                Drive *newDrive = new Drive();
                newDrive->setNumberPlate(numberPlate);
                newDrive->setManufacturer(manufacturer);
                newDrive->setTimestamp(date);
                // traffic
                BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt.get_child(date + ".traffic"))
                {
                    newDrive->addTraffic(v.second.data());
                }
                // tank
                BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt.get_child(date + ".tank"))
                {
                    newDrive->addTank(v.second.data());
                }
                // milage
                BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt.get_child(date + ".milage"))
                {
                    newDrive->addMilage(v.second.data());
                }
                // speed
                BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt.get_child(date + ".speed"))
                {
                    newDrive->addSpeed(v.second.data());
                }
                dataObject->addDrive(newDrive);
                
            }
        } 
        data.push_back(dataObject);
    }
    catch (std::exception const& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void ApacheThriftHandler::carStatus(Result& _return, const string& carType, const string& numberPlate, const string& server) 
{
    Manufacturer& manu = Manufacturer::getInstance();
    printf("carStatus\n");

    string finalJson;
    if(server == "0")
    {
        readFile(carType, numberPlate);
        for(int i = 0; i < manu.getServerIP().size(); i++)
        {
            if(i+1 == atoi(manu.getIdServer().c_str()))
                continue;
            apacheThriftClient(carType, numberPlate, i);
        }
        calculateResult();
        finalJson = generateJsonString("0"); 
    }
    else if(server == "1")
    {
        readFile(carType, numberPlate);
        finalJson = generateJsonString("1"); 
    }
    

    data.clear();

    // Debug
    //cout << finalJson << endl;

    _return.result = finalJson;
}

void ApacheThriftHandler::apacheThriftClient(string carType, string numberPlate, int serverID)
{
    try{
    Manufacturer& manu = Manufacturer::getInstance();
    string ip = manu.getServerIP().at(serverID);
    int port = manu.getServerPort().at(serverID);
    shared_ptr<TSocket> socket(new TSocket(ip, port));
    shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
    CalcClient calc(protocol);
    socket->setConnTimeout(2000);
    transport->open();

    Result myResult;

    calc.carStatus(myResult, carType, numberPlate, "1");

    string jsonData = myResult.result;

    readJson(jsonData);

    transport->close(); 
    } catch (TTransportException e) {
            
        } 
}