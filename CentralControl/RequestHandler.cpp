// Authors: Patrick Tinz and Janik Tinz

#include <iostream>
#include "RequestHandler.h"
#include "CentralControl.h"
#include "FileHandler.h"

using namespace std;

void RequestHandler::process() 
{
    size_t pos = request.find('=');
    string key = request.substr(0, pos);
    string val = request.substr(pos + 1);
    
    //cout << "KEY=" << key << " VAL=" << val << '\n';
    if (key == "history" && val == "car") 
    {
        CentralControl &control = CentralControl::getInstance();
        string fileName = control.getNumberPlate();
        fileName = "cars/" + fileName + ".txt";
        FileHandler dh(socket, fileName);
        dh.send();
            
    }
    else 
    {
        HttpResponse httpResponse(400);
        httpResponse.addToHeader("Content-Type", "text/html");
        string msg =
            httpResponse.getHTMLMessage("unknown query!");
        httpResponse.addToHeader("Content-Length",
                                 to_string(msg.length()));
        httpResponse.sendHeader(socket);
        socket.write_some(
            boost::asio::buffer(msg.c_str(), msg.length()));
    }
}
