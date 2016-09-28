
CC = g++

INCLUDE = -I./include -I/usr/include/SDL

LIBDIR = -L./lib

LIB = -lavutil -lavcodec -lavformat -lavdevice -lswscale  -lSDL -lpthread

PARAM = -g -O2 -std=c++11

TARGET = cap

OBJECT = capturer.o encoder.o

all:$(TARGET)

$(TARGET): $(OBJECT)
	$(CC) $(PARAM) -o $(TARGET) main.cpp $(LIBDIR) $(LIB)
	
capturer.o:capturer.hpp
	$(CC) $(PARAM) -c capturer.hpp

encoder.o: encoder.hpp
	$(CC) $(PARAM) -c encoder.hpp


cleangch:
	rm *.gch
	
clean:
	rm encoder *.gch
	
cleanall:
	rm *.o *.gch encoder *~
