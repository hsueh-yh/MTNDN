all:consumer publisher
	
consumer:
	g++  -g -O2 -std=gnu++11  -lboost_regex -lboost_system  -o consumer consumer.cpp -Wl,-rpath /usr/local/lib/ -L/usr/local/lib -lndn-cpp -lpthread -lprotobuf -lsqlite3 -lcrypto

publisher:
	g++  -g -O2 -std=gnu++11  -lboost_regex -lboost_system  -o publisher publisher.cpp -Wl,-rpath /usr/local/lib/ -L/usr/local/lib -lndn-cpp -lpthread -lprotobuf -lsqlite3 -lcrypto
