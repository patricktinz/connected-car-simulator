// Authors: Patrick Tinz and Janik Tinz

#include "Menu.h"

int main(int argc, char** argv) 
{
    Menu &menu = Menu::getInstance();
    menu.mainMenu();
    return 0;
}

