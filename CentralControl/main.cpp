// Authors: Patrick Tinz and Janik Tinz

#include <cstdlib>
#include "Menu.h"
#include "Server.h"
#include "MQTTPublisher.h"

using namespace std;

void startHttpServer()
{
    unsigned short port = 9090;
    Server server(port, "web");
    server.run();
}

/*void startMQTTPublisher ()
{
    MQTTPublisher pub;
    pub.startPublisher();
}*/

int main(int argc, char** argv) 
{
    boost::thread t(startHttpServer);
    
    //boost::thread t1(startMQTTPublisher);
    
    Menu menu;
    menu.mainMenu();
    return 0;
}