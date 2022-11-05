CFLAGS_LINUX = -pipe -ldl -lpthread -lm -O0 -ggdb -Wall -Wno-unused-command-line-argument -Wno-reorder -std=c++17
CFLAGS_WINDOWS = -Wl,-subsystem,windows -ggdb -Wall -std=c++17
LIBS = 
CC_LINUX = ccache clang++
CC_WINDOWS = g++
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

%.o: %.h
	$(CC_LINUX) $(CFLAGS_LINUX) -c -o $@ $< $(LIBS)

all: default
default: $(OBJECTS)
	

windows:
	$(CC_WINDOWS) $(CFLAGS_WINDOWS) *.cpp -o yacg $(LIBS)

clean:
	rm -rf *.o
	rm yacg
