CFLAGS_LINUX = -pipe -O0 -ggdb -Wall -Wno-reorder -std=c++17
CFLAGS_WINDOWS = -Wl,-subsystem,windows -ggdb -Wall -std=c++17
LIBS = `pkg-config gtkmm-3.0 --cflags --libs`
CC = ccache g++
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

%.o: %.cpp
	$(CC) $(CFLAGS_LINUX) -c -o $@ $< $(LIBS)

all: default
default: $(OBJECTS)
	g++ $(CFLAGS_LINUX) -o yacg $^ $(LIBS)

clean:
	rm -rf *.o
	rm yacg
