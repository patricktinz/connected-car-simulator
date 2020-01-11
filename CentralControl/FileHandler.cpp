// Authors: Patrick Tinz and Janik Tinz

#include <iostream>
#include "FileHandler.h"
#include "CentralControl.h"

using namespace std;

void FileHandler::process() 
{
    CentralControl &control = CentralControl::getInstance();
    
    if(control.getTraffic().size() > 0 || control.getAverageSpeed().size() > 0 || control.getTank().size() > 0 || control.getMilage().size() > 0)
    {
        HttpResponse httpResponse(200);
        httpResponse.addToHeader("Content-Type", "text/html");
        string msg =
            "<!DOCTYPE html>"
            "<html>"
                "<head>"
                    "<meta charset=\"UTF-8\" />"
                    "<meta http-equiv=\"refresh\" content=\"1\" />"
                    "<title>Central Control</title>"
                    "<style>"
                        "*{padding: 0em; margin: 0em;}"
                        "h1 {color: white; background-color: rgb(10, 80, 180);}"
                        "body {color:black; background-color: rgb(223, 222, 221); font-family: Verdana, Arial, Helvetica, sans-serif; font-size: 1rem; }"
                        ".listBox {height: 30rem;width: 10rem;margin: 0.5rem;}"
                        "option {text-align: center;}"
                        ".label{vertical-align: top;}"
                        ".box {border: 0.1em solid black; background-color: rgba(243, 253, 152, 0.986); text-align: center; margin: 0.5rem;}"
                        ".button {width: 3rem; margin-right: 4rem;}"
                        "footer{color: white; background-color: grey; text-align: center;}"
                    "</style>"
                "</head>"
                "<body>"
                    "<h1>Sensors";
        msg += " (" + control.getNumberPlate() + ", " + control.getCar() + ")" + "</h1>";
                    msg += "<div class=box><label class=\"label\">Traffic:"
                    "<select class=\"listBox\" name=\"traffic[]\" id=\"traffic\" multiple="" size=\"10\">";
        string tmp;
        
        for(int i = 0; i < control.getTraffic().size(); i++)
        {
            tmp = control.getTraffic()[i];
            tmp = "<option>" + tmp + "</option>";
            msg += tmp;
        }
        msg += "</select>"
            "</label>"

            "<label class=\"label\">Speed (in km/h):"
                "<select class=\"listBox\" name=\"speed[]\" id=\"speed\" multiple="" size=\"10\">";
        for(int i = 0; i < control.getAverageSpeed().size(); i++)
        {
            tmp = to_string((int)control.getAverageSpeed()[i]);
            tmp = "<option>" + tmp + "</option>";
            msg += tmp;
        }
        msg += "</select>"
            "</label>"

            "<label class=\"label\">Tank (in l):"
                "<select class=\"listBox\" name=\"tank[]\" id=\"tank\" multiple="" size=\"10\">";
        for(int i = 0; i < control.getTank().size(); i++)
        {
            tmp = to_string((int)control.getTank()[i]);
            tmp = "<option>" + tmp + "</option>";
            msg += tmp;
        }
        msg += "</select>"
            "</label>"

            "<label class=\"label\">Milage (in km):"
                "<select class=\"listBox\" name=\"milage[]\" id=\"milage\" multiple="" size=\"10\">";
        for(int i = 0; i < control.getMilage().size(); i++)
        {
            tmp = to_string((int)control.getMilage()[i]);
            tmp = "<option>" + tmp + "</option>";
            msg += tmp;
        }
         msg+="</select>"
            "</label>"
            "</div>"   
            "<p>History:</p>"
            "<form action="" method=\"GET\">"
            "<input class=\"button\" type=\"submit\" name=\"history\" size=\"12\" value=\"car\"/>"
            "</form>"
            "<footer>Central Control</footer>"
            "</body>"
            "</html> ";
        httpResponse.addToHeader("Content-Length", to_string(msg.length()));
        httpResponse.sendHeader(socket);
        socket.write_some(
            boost::asio::buffer(msg.c_str(), msg.length()));
    }
    else
    {
        ifstream source(fileName.c_str(), std::ios::binary);
    
        if (source) 
        {
            sendFile(source);
        } 
        else 
        {   // file does not exist
            HttpResponse httpResponse(404);
            httpResponse.addToHeader("Content-Type", "text/html");
            string message = httpResponse.getHTMLMessage("File does not found!");
            httpResponse.addToHeader("Content-Length", to_string(message.length()));
            httpResponse.sendHeader(socket);
            socket.write_some(boost::asio::buffer(message.c_str(), message.length()));
        }
    }
}

string FileHandler::getContentType() 
{
    string type("application/octet-stream");
    auto dot = fileName.rfind('.');
    if (dot < string::npos) 
    {
        string extension = fileName.substr(dot + 1);
        for (size_t i = 0; i < extension.length(); ++i) 
        {
              extension[i] = toupper(extension[i]);
        }
     
        if (extension == "HTML")
          type = "text/html";
        else if (extension == "TXT")
          type = "text/plain; charset=iso-8859-1";
    }
    return type;
}

void FileHandler::send()
{
    ifstream source(fileName.c_str(), std::ios::binary);
    sendFile(source);
}

void FileHandler::sendFile(ifstream &source) 
{
    boost::filesystem::path p(fileName);
    size_t bufsize = boost::filesystem::file_size(p);
    HttpResponse httpResponse(200);
    httpResponse.addToHeader("Content-Type", getContentType());
    httpResponse.addToHeader("Content-Length", to_string(bufsize));
    httpResponse.sendHeader(socket);
    char *const buf = new char[bufsize];
    size_t pos = 0;
    while (source.get(buf[pos++]));   // Read file and save in buf 
    socket.write_some(boost::asio::buffer(buf, bufsize));
    delete[] buf;
}
