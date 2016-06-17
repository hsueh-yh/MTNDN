LIBDIR=-L./ffmpeg/lib
INCLUDE=-I./ffmpeg/include
LIB=-lavcodec -lavutil -lpostproc

all:decoder
decoder:
	g++ $(LIBDIR) $(INCLUDE) $(LIB) tdll.h h264decoder.h h264decoder.cpp main.cpp -o decoder -ldl 
clean:
	rm decoder
