CC=gcc
CCFLAGS=-Wall -std=c99 -pedantic -D_DEFAULT_SOURCE -lm
SOURCES=$(wildcard src/*.c)
BIN_DIR=./bin
BIN_FILE=./bin/main

all:
	@mkdir -p $(BIN_DIR)
	@$(CC) $(SOURCES) -o $(BIN_FILE) $(CCFLAGS)

clean:
	@rm -rf $(BIN_DIR)

PHONY: clean all