all: main.cpp
	clang++ -ggdb -Wall -Wno-reorder -std=c++17 *.cpp -o miniyacg `pkg-config gtkmm-3.0 --cflags --libs`
clean:
	$(RM) miniyacg
