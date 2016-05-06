# cs335 project
# to compile your project, type make and press enter

all: upstream

upstream: upstream.cpp
	g++ upstream.cpp ppm.cpp log.cpp jonathanD.cpp johnH.cpp quyN.cpp game.cpp kevinJ.cpp io.cpp collision.cpp -Wall -o upstream libggfonts.a -lopenal -lalut -lX11 -lGL -lGLU -lrt -pthread -lm

clean:
	rm -f upstream
	rm -f *.o

