HEAD := $(wildcard include/*.h)
SRC := $(wildcard src/*.cxx)
OBJ := $(patsubst src/%.cxx,tmp/%.o,$(SRC))
CXXFLAGS := -Iinclude $(shell root-config --cflags) -g
LDFLAGS := $(shell root-config --libs)
TARGET := libTrendMaker.so

all: main $(TARGET)

clean:
	-rm main
	-rm $(TARGET)
	-rm $(OBJ)

tmp/%.o: src/%.cxx $(HEAD)
	g++ -c $(CXXFLAGS) -fPIC -o $@ $<

$(TARGET): $(OBJ)
	g++ -o $@ -shared $(LDFLAGS) $^
	
main: main.cxx $(TARGET) $(HEAD)
	g++ -o $@ $(TARGET) $(CXXFLAGS) $(LDFLAGS) $<
