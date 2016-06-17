LIBDIR=-L./ffmpeg/lib
INCLUDE=-I./ffmpeg/include
LIB=-lavcodec -lavutil -lpostproc

SOURCEFILE=tdll.h decoder.h decoder.cpp

all:decoder.o

decoder.o:
	g++ -c $(INCLUDE) $(SOURCEFILE)
#decoder:
#	g++ $(LIBDIR) $(INCLUDE) $(LIB) tdll.h h264decoder.h h264decoder.cpp main.cpp -o decoder -ldl 
clean:
	rm decoder
