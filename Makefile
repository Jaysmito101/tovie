SOURCE_FILES := $(wildcard src/*.cpp)
OBJECT_FILES := $(patsubst src/%.cpp,dist/%.o,$(SOURCE_FILES))

bin/tovie: $(OBJECT_FILES) bin
	g++ -ldl -o bin/toviec $(OBJECT_FILES)

bin:
	mkdir bin

dist:
	mkdir dist

dist/%.o: src/%.cpp dist
	g++ -std=c++11 -Iinclude -c -o $@ $<