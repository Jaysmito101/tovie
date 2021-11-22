ifeq ($(OS),Windows_NT)     # is Windows_NT on XP, 2000, 7, Vista, 10...
    SYSTEM := Windows
else
    SYSTEM := $(shell uname) # Linux etc.
endif

ifeq ($(SYSTEM), Windows)
	BINARY := tovie.exe
else
	BINARY := tovie
endif

SOURCE_FILES := $(wildcard src/*.cpp)
OBJECT_FILES := $(patsubst src/%.cpp,dist/%.o,$(SOURCE_FILES))

CC := g++
CFLAGS := -std=c++11 -Iinclude
LDFLAGS := -ldl

all: bin dist bin/tovie

bin/tovie: $(OBJECT_FILES)
	$(CC) $(LDFLAGS) -o bin/toviec $(OBJECT_FILES)

bin:
	mkdir bin

dist:
	mkdir dist

dist/%.o: src/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<