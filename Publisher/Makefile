Publisher=./*.h ./*.cpp ./util/*

all:publisher

publisher:
	g++  -g -O2 -std=gnu++11  -lboost_regex -lboost_system  -o publisher $(Publisher) -Wl,-rpath /usr/local/lib/ -L/usr/local/lib -lndn-cpp -lpthread -lprotobuf -lsqlite3 -lcrypto

clean:
	rm publisher
