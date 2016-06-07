	
Consumer=./util/* ./*.h ./*.cpp 

all:consumer
	
consumer:
	g++  -g -O2 -std=gnu++11  -lboost_regex -lboost_system  -o consumer $(Consumer) -Wl,-rpath /usr/local/lib/ -L/usr/local/lib -lndn-cpp -lpthread -lprotobuf -lsqlite3 -lcrypto


clean:
	rm consumer
