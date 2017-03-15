all:
	g++ -g -std=c++11 -Wall -o main main.cpp buffer.cpp editor.cpp -lncurses
b:
	$(CXX) -g -std=c++11 -Wall -o main main.cpp buffer.cpp editor.cpp -lncurses
clean:
	rm main
