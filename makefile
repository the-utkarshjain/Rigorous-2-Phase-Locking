all: readfile lockmanager main out

readfile: readfile.cpp
	g++ -Wall -std=c++11 -c readfile.cpp

lockmanager: lockmanager.cpp
	g++ -Wall -std=c++11 -c lockmanager.cpp

main: main.cpp
	g++ -Wall -std=c++11 -c main.cpp

out: readfile.o main.o
	g++ lockmanager.o readfile.o main.o -o out -lm