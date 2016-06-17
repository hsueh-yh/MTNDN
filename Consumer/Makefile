
CC = g++

INCLUDE = -I./ffmpeg/include

LIB = -lboost_thread -lboost_system -lboost_chrono -lboost_regex -lndn-cpp -lpthread -lprotobuf -lsqlite3 -lcrypto

PARAM = -g -O2 -std=c++11

TARGET = consumer

OBJECTS = decoder.o utils.o object.o face-wrapper.o frame-buffer.o pipeliner.o player.o consumer.o

all:main
	
#consumer:  utils object face buffer pipeliner player
#	$(CC) $(PARAM) -o $(TARGET) $(INCLUDE) $(Consumer) $(LIB)

main: $(OBJECTS)
	$(CC) $(PARAM) -o $(TARGET) main.cpp $(OBJECTS) $(LIB) -ldl

decoder.o:
	$(CC) $(PARAM) -c tdll.h decoder.h decoder.cpp $(INCLUDE)

utils.o:
	$(CC) $(PARAM) -c utils.h utils.cpp face-wrapper.h

object.o:
	$(CC) $(PARAM) -c object.h object.cpp utils.h

face-wrapper.o:
	$(CC) $(PARAM) -c face-wrapper.h face-wrapper.cpp utils.h object.h

frame-buffer.o:
	$(CC) $(PARAM) -c frame-buffer.h frame-buffer.cpp

pipeliner.o:
	$(CC) $(PARAM) -c pipeliner.h pipeliner.cpp frame-buffer.h

player.o:
	$(CC) $(PARAM) -c player.h player.cpp frame-buffer.h

consumer:
	$(CC) $(PARAM) -c utils.h object.h face-wrapper.h frame-buffer.h pipeliner.h player.h consumer.h consumer.cpp

#$(Decoder):
#	cd Decoder; $(MAKE)

clean:
	rm consume
