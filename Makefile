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

SOURCE_FILES := $(wildcard src/*.cpp)
OBJECT_FILES := $(patsubst src/%.cpp,dist/%.o,$(SOURCE_FILES))

CC := g++
CFLAGS := -std=c++11 -Iinclude
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

dist/%.o: src/%.cpp
	$(CC) -O3 -c -o $@ $< $(CFLAGS)