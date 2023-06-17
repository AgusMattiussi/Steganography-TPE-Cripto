CC=gcc
# CCFLAGS_FINAL=-g -Wall -Wextra -Wno-unused-parameter -Wno-implicit-fallthrough -pedantic
CCFLAGS=-Wall -std=c99 -pedantic -D _DEFAULT_SOURCE -lm
SOURCES=$(wildcard ./*.c)
SOURCES_BMP=$(wildcard bmpReader/*.c)
SOURCES_SHADOWGEN=$(wildcard shadowgen/*.c)
SOURCES_STEGANOGRAPHY=$(wildcard steganography/*.c)
BIN_DIR=./bin
BIN_FILE=./bin/main

all:
	@mkdir -p $(BIN_DIR)
	@$(CC) $(SOURCES) $(SOURCES_BMP) $(SOURCES_SHADOWGEN) $(SOURCES_STEGANOGRAPHY) -o $(BIN_FILE) $(CCFLAGS)

clean:
	@rm -rf $(BIN_DIR)

PHONY: clean all