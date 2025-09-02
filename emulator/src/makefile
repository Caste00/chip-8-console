TARGET := eseguibile
SRC := $(wildcard *.cpp)
CXX := g++
CXXFLAGS := -std=c++17 -Wall -O2
LDFLAGS := -lSDL2

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)
