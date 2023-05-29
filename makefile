FLAGS=`pkg-config --libs --cflags raylib` -std=c++20
SRC=src
BIN=bin

all: ${SRC}/main.cpp
	g++ ${SRC}/main.cpp -o ${BIN}/main ${FLAGS} && ./bin/main