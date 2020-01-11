# authors Janik Tinz and Patrick Tinz

CPP = g++
LIBS = -pthread -lpthread -lboost_system -lboost_thread -lboost_filesystem -lpaho-mqtt3a -lpaho-mqtt3as -lpaho-mqtt3c -lpaho-mqtt3cs -lthrift
FLAGS = -g -std=c++14 -Wall -MMD -fmessage-length=0
GENSRC = gen-cpp/Calc.cpp gen-cpp/calc_constants.cpp gen-cpp/calc_types.cpp
TARGETSRC = Manufacturer.cpp MQTTSubscriber.cpp ApacheThriftHandler.cpp Data.cpp Drive.cpp main.cpp
TARGET = manufacturer

$(GENSRC): calc.thrift
	thrift --gen cpp calc.thrift

$(TARGET): $(SRC) $(TARGETSRC) $(GENSRC)
	$(CPP) $(FLAGS) $(LIBS) $(GENSRC) $(TARGETSRC) -o $(TARGET)

all: $(TARGET)

clean:
	rm -rf $(GENSRC) $(TARGET)