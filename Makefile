all:parser.so
	
parser.so:native/main.cpp

	cd native && g++ main.cpp -std=c++0x -shared -fpic -o ../parser.so
