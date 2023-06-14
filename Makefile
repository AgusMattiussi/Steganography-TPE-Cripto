CC=gcc
# CCFLAGS_FINAL=-g -Wall -Wextra -Wno-unused-parameter -Wno-implicit-fallthrough -pedantic
CCFLAGS=-Wall -std=c99 -pedantic -D _DEFAULT_SOURCE -lm
SOURCES=$(wildcard ./*.c)
SOURCES_BMP=$(wildcard bmpReader/*.c)
SOURCES_SHADOWGEN=$(wildcard shadowgen/*.c)
BIN_DIR=./bin
BIN_FILE=./bin/main
BIN_FILE_BMP=./bin/bmp

all:
	@mkdir -p $(BIN_DIR)
	@$(CC) $(SOURCES) $(SOURCES_BMP) $(SOURCES_SHADOWGEN) -o $(BIN_FILE) $(CCFLAGS)

clean:
	@rm -rf $(BIN_DIR)

PHONY: clean all