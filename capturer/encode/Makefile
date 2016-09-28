
CC = g++

INCLUDE = -I./include -I/usr/include/SDL

LIBDIR = -L./lib

LIB = -lavutil -lavcodec -lavformat -lavdevice -lswscale  -lSDL -lpthread

PARAM = -g -O2 -std=c++11

TARGET = encoder

OBJECT = encoder.o

all:$(TARGET)

$(TARGET): $(OBJECT)
	$(CC) $(PARAM) -o $(TARGET) encoder.cpp $(LIBDIR) $(LIB)
	

clean:
	rm *.o *.gch encoder *~
