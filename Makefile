CFLAGS = -I ./include
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: walk

walk: walk.cpp jgarcia2.cpp
	g++ $(CFLAGS) walk.cpp libggfonts.a jgarcia2.cpp -Wall -Wextra $(LFLAGS) -owalk

clean:
	rm -f walk
	rm -f *.o

