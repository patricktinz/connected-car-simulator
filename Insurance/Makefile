# authors Janik Tinz and Patrick Tinz

CPP = g++
LIBS = -pthread -lpthread -lboost_system -lboost_thread -lboost_filesystem -lthrift
FLAGS = -g -std=c++14 -Wall -MMD -fmessage-length=0
GENSRC = gen-cpp/Calc.cpp gen-cpp/calc_constants.cpp gen-cpp/calc_types.cpp
CLIENTSRC = Insurance.cpp Insurant.cpp Menu.cpp main.cpp
CLIENT = insurance

$(GENSRC): calc.thrift
	thrift --gen cpp calc.thrift

$(CLIENT): $(SRC) $(CLIENTSRC) $(GENSRC)
	$(CPP) $(FLAGS) $(LIBS) $(GENSRC) $(CLIENTSRC) -o $(CLIENT)

all: $(CLIENT)

clean:
	rm -rf $(GENSRC) $(CLIENT)