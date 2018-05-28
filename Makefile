HEAD := $(wildcard include/*.h)
SRC := $(wildcard src/*.cxx)
OBJ := $(patsubst src/%.cxx,tmp/%.o,$(SRC))
CXXFLAGS := -Iinclude $(shell root-config --cflags) -g -std=c++11
LDFLAGS := $(shell root-config --libs)
TARGET := libTrendMaker.so

all: main $(TARGET)

clean:
	-rm main
	-rm $(TARGET)
	-rm $(OBJ)

tmp/%.o: src/%.cxx $(HEAD)
	g++ -c $(CXXFLAGS) -fPIC -o $@ $<

tmp/main.o: main.cxx $(HEAD)
	g++ -c $(CXXFLAGS) -fPIC -o $@ $<

$(TARGET): $(OBJ)
	g++ -o $@ -shared $(LDFLAGS) $^
	
main: tmp/main.o $(TARGET) $(HEAD)
	g++ -o $@ tmp/main.o $(TARGET) $(LDFLAGS)
