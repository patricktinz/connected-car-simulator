# authors Janik Tinz and Patrick Tinz
CPP = g++

OBJ = CentralControl.o Menu.o Drive.o UDPServer.o Server.o HttpConnection.o HttpResponse.o RequestHandler.o FileHandler.o MQTTPublisher.o main.o 

TARGET = control 

FLAGS = -g -std=c++14 -Wall -MMD -fmessage-length=0

LIBS = -pthread -lpthread -lboost_system -lboost_thread -lboost_filesystem -lpaho-mqtt3a -lpaho-mqtt3as -lpaho-mqtt3c -lpaho-mqtt3cs

# name of the program
all: $(TARGET)

%.o : %.cpp
	$(CPP) -c $(FLAGS) -o $@ $<

$(TARGET): $(OBJ)
	$(CPP) -o $(TARGET) $(LIBS) $(OBJ)

clean:
	rm control $(OBJ)
