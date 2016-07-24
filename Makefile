CPP = g++
LD = g++
CPPFLAGS = -O3 -std=c++11
LDFLAGS =
TARGET = hw3
OBJS = main.o
OS = $(shell uname)

ifeq ($(OS),Darwin)
	LIB =  -framework OpenGL -framework Cocoa -lGLEW -lglfw3
else
	LIB =  -lGL -lGLEW -lglfw
endif

default: $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) $(LIB) -o $(TARGET)

main.o: main.cpp
	$(CPP) -c $(CPPFLAGS) main.cpp

clean:
	rm -f *.o $(TARGET)
