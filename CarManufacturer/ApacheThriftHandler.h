// Authors: Patrick Tinz and Janik Tinz

#include "gen-cpp/Calc.h"
#include "Data.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <string>
#include <vector>

using namespace std;
using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

#ifndef APACHETHRIFTHANDLER_H
#define APACHETHRIFTHANDLER_H

class ApacheThriftHandler : virtual public CalcIf {
public:
    ApacheThriftHandler();
    virtual ~ApacheThriftHandler();
    
    void readFile(string, string); 
    
    void ping();
    
    void calculateResult();
    string generateJsonString(string);
    void readJson(string);
    
    void carStatus(Result& _return, const string&, const string&, const string&);
    
    void apacheThriftClient(string, string, int);
    
    void setServerPort(vector<int> serverPort);
    vector<int> getServerPort() const;
    void setServerIP(vector<string> serverIP);
    vector<string> getServerIP() const;
    
    void addServerIP(string);
    void addServerPort(int);
    
private:
    vector<Data*> data;
};

#endif /* APACHETHRIFTHANDLER_H */

