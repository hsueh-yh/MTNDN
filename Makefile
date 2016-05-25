all:consumer publisher
	
consumer:
	g++  -g -O2 -std=gnu++11  -lboost_regex -lboost_system  -o consumer Consumer.h Consumer.cpp C_main.cpp -Wl,-rpath /usr/local/lib/ -L/usr/local/lib -lndn-cpp -lpthread -lprotobuf -lsqlite3 -lcrypto

publisher:
	g++  -g -O2 -std=gnu++11  -lboost_regex -lboost_system  -o publisher Publisher.h Publisher.cpp P_main.cpp -Wl,-rpath /usr/local/lib/ -L/usr/local/lib -lndn-cpp -lpthread -lprotobuf -lsqlite3 -lcrypto

clean:
	rm consumer publisher
