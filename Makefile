CFLAGS_LINUX = -fcompare-debug-second -pipe -ldl -lpthread -lm -O0 -ggdb -Wall -Wno-unused-function -Wno-unused-command-line-argument -Wno-reorder -std=c++17
CFLAGS_WINDOWS = -Wl,-subsystem,windows -w -std=c++17
LIBS = `pkg-config gtkmm-3.0 --cflags --libs`
CC_LINUX = ccache g++
CC_WINDOWS = x86_64-w64-mingw32-g++
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
WIN_PKG_CONFIG_PATH = /home/leopoldkucinski/quasi-msys2/root/mingw64/lib/pkgconfig:/home/leopoldkucinski/quasi-msys2/root/mingw64/share/pkgconfig
WIN_PKG_SYSROOT_DIR = /home/leopoldkucinski/quasi-msys2/root

%.o: %.cpp
	$(CC_LINUX) $(CFLAGS_LINUX) -c -o $@ $< $(LIBS)

all: default
default: $(OBJECTS)
	$(CC_LINUX) $(CFLAGS_LINUX) -o yacg $^ $(LIBS)
	rm -rf *.o

.ONESHELL:
SHELL = /usr/bin/bash
windows:
	export PKG_CONFIG_PATH="$(WIN_PKG_CONFIG_PATH)"; export PKG_CONFIG_SYSROOT_DIR="$(WIN_PKG_SYSROOT_DIR)"; $(CC_WINDOWS) $(CFLAGS_WINDOWS) *.cpp -o yacg $(LIBS)

clean:
	rm -rf *.o
	rm yacg
	rm yacg.exe
