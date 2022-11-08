CFLAGS_LINUX = -pipe -ldl -lpthread -lm -O0 -ggdb -Wall -Wno-unused-function -Wno-unused-command-line-argument -Wno-reorder -std=c++17
CFLAGS_WINDOWS = -Wl,-subsystem,windows -ggdb -w -std=c++17
LIBS = `pkg-config gtkmm-3.0 --cflags --libs`
CC_LINUX = ccache clangg++
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
