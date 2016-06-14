	
Consumer=./util/* ./*.h ./*.cpp 

all:consumer
	
consumer:
	g++  -g -O2 -std=c++11 -I/usr/include -L/usr/lib/x86_64-linux-gnu  -o consumer $(Consumer) -lboost_thread -lboost_system -lboost_chrono -lboost_regex -Wl,-rpath /usr/local/lib/ -L/usr/local/lib -lndn-cpp -lpthread -lprotobuf -lsqlite3 -lcrypto
clean:
	rm consumer
