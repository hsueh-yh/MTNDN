
CC = g++

PARAM = "-fno-stack-protector"

LIBDIR = -L/usr/local/lib

LIB=-lavcodec -lavutil -lpostproc

SRC=decoder.cpp

TARGET=decoder

OBJECT = decoder.o

all:$(OBJECT) $(TARGET)

$(TARGET):$(OBJECT)
	$(CC) $(PARAM) $(LIBDIR) $(LIB) $(OBJECT) main.cpp -o $(TARGET) -ldl 

$(OBJECT):
	$(CC) -c $(PARAM) $(INCLUDE) $(SRC)


clean:
	rm $(TARGET) $(OBJECT) *.gch 
