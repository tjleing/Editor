all:
	g++ -g -lncurses -std=c++11 -Wall -o main main.cpp buffer.cpp editor.cpp
b:
	g++ -g -Wall -o main main.cpp buffer.cpp editor.cpp -lncurses -std=c++11 
