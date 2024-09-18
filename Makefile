all: drawBmp

drawBmp: main.cpp
	g++ --std=c++11 -o drawBmp main.cpp

rebuild:
	$(MAKE) clean
	$(MAKE) all

clean:
	rm drawBmp