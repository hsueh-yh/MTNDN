
CC = g++

LIB = -lboost_system -lboost_regex -lndn-cpp -lpthread -lsqlite3 -lcrypto

PARAM = -g -O2 -std=c++11

TARGET = publisher

OBJECTS = publisher.o	


all:$(TARGET)
	
$(TARGET): $(OBJECTS)
	$(CC) $(PARAM) -o $(TARGET) main.cpp $(OBJECTS) $(LIB)

$(OBJECTS):
	$(CC) $(PARAM) -c common.h defines.h frame-data.h publisher.h publisher.cpp


cleangch:
	rm *.gch
	
clean:
	rm publisher *.gch
	
cleanall:
	rm *.o *.gch publisher *~
