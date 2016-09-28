
CC = g++

INCLUDE = -I./include -I/usr/include/SDL

LIBDIR = -L./lib

LIB = -lavutil -lavcodec -lavformat -lavdevice -lswscale  -lSDL -lpthread

PARAM = -g -O2 -std=c++11

TARGET = capturer

OBJECT = capturer.o

all:$(TARGET)

$(TARGET): $(OBJECT)
	$(CC) $(PARAM) -o $(TARGET) main.cpp $(LIBDIR) $(LIB)
	
$(OBJECT):capturer.hpp
	$(CC) $(PARAM) -c capturer.hpp


clean:
	rm *.o *.gch $(TARGET) *~
	
cleanall:clean
	rm $(TARGET) *.yuv
