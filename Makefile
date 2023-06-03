CC=gcc
# CCFLAGS_FINAL=-g -Wall -Wextra -Wno-unused-parameter -Wno-implicit-fallthrough -pedantic
CCFLAGS=-Wall -std=c99 -pedantic -D _DEFAULT_SOURCE
SOURCES=$(wildcard ./*.c)
SOURCES_BMP=$(wildcard bmpReader/*.c)
BIN_DIR=./bin
BIN_FILE=./bin/main
BIN_FILE_BMP=./bin/bmp

all:
	@mkdir -p $(BIN_DIR)
	@$(CC) $(CCFLAGS) $(SOURCES) -o $(BIN_FILE)
	@$(CC) $(CCFLAGS) $(SOURCES_BMP) -o $(BIN_FILE_BMP)

clean:
	@rm -rf $(BIN_DIR)

PHONY: clean all