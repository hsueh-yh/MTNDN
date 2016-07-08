CC = g++


LIB = -lboost_thread -lboost_system -lboost_chrono -lboost_regex -lndn-cpp -lpthread -lprotobuf -lsqlite3 -lcrypto

PARAM = -g -O2 -std=c++11

TARGET = publisher

OBJECTS = utils.o object.o face-wrapper.o publisher.o	



all:main

main: $(OBJECTS)
	$(CC) $(PARAM) -o $(TARGET) main.cpp $(OBJECTS) $(LIB)

utils.o:
	$(CC) $(PARAM) -c face-wrapper.h utils.h utils.cpp

object.o:
	$(CC) $(PARAM) -c utils.h object.h object.cpp

face-wrapper.o:
	$(CC) $(PARAM) -c face-wrapper.h face-wrapper.cpp utils.h object.h

publisher.o:
	$(CC) $(PARAM) -c utils.h object.h face-wrapper.h publisher.h publisher.cpp



cleangch:
	rm *.gch
	
clean:
	rm publisher *.gch
	
cleanall:
	rm *.o *.gch publisher *~

