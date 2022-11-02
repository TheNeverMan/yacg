CFLAGS_LINUX = -pipe -O0 -ggdb -Wall -Wno-reorder -std=c++17
CFLAGS_WINDOWS = -Wl,-subsystem,windows -ggdb -Wall -std=c++17
LIBS = `pkg-config gtkmm-3.0 --cflags --libs`
CC_LINUX = ccache g++
CC_WINDOWS = g++
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

%.o: %.cpp
	$(CC_LINUX) $(CFLAGS_LINUX) -c -o $@ $< $(LIBS)

all: default
default: $(OBJECTS)
	$(CC_LINUX) $(CFLAGS_LINUX) -o yacg $^ $(LIBS)
	rm -rf *.o

windows:
	$(CC_WINDOWS) $(CFLAGS_WINDOWS) *.cpp -o yacg $(LIBS)

clean:
	rm -rf *.o
	rm yacg
