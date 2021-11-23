ifeq ($(OS),Windows_NT)
    SYSTEM := Windows
else
    SYSTEM := $(shell uname) # Linux etc.
endif

ifeq ($(SYSTEM), Windows)
	BINARY := bin/tovie.exe
else
	BINARY := bin/tovie
endif

SOURCE_FILES := $(wildcard src/*.cpp) $(wildcard src/*.c)
OBJECT_FILES := $(patsubst src/%,dist/%.o,$(SOURCE_FILES))

CC := g++
C := gcc
CFLAGS := -Iinclude
LDFLAGS :=
ifneq ($(SYSTEM), Windows)
	LDFLAGS += -ldl
endif

build: bin dist $(BINARY)

clean:
	rm -rf dist bin

$(BINARY): $(OBJECT_FILES)
	$(CC) -o $(BINARY) $(OBJECT_FILES) $(LDFLAGS)

bin dist:
	mkdir -p bin dist

dist/%.c.o: src/%.c
	$(C) -c -o $@ $< $(CFLAGS)

dist/%.cpp.o: src/%.cpp
	$(CC) -c -o $@ $< $(CFLAGS) -std=c++11