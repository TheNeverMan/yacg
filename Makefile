all: main.cpp
	ccache g++ -pipe -O0 -ggdb -Wall -Wno-reorder -std=c++17 *.cpp -o yacg `pkg-config gtkmm-3.0 --cflags --libs`
windows: main.cpp
	g++ -Wl,-subsystem,windows -ggdb -Wall -std=c++17 *.cpp -o yacg `pkg-config gtkmm-3.0 --cflags --libs`
clean:
	$(RM) yacg
