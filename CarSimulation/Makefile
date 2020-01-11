# authors Janik Tinz and Patrick Tinz
CPP = g++

OBJ = CarSimulator.o main.o  

TARGET = simulator

FLAGS = -g -std=c++14 -Wall -MMD -fmessage-length=0

LIBS = -pthread -lpthread -lboost_system -lboost_thread

# name of the program
all: $(TARGET)

%.o : %.cpp
	$(CPP) -c $(FLAGS) -o $@ $<

$(TARGET): $(OBJ)
	$(CPP) -o $(TARGET) $(LIBS) $(OBJ)

clean:
	rm -f simulator $(OBJ)