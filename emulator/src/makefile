TARGET := eseguibile
SRC := *.cpp
CXX := g++
CXXFLAGS := -std=c++17 -Wall -O2
LDFLAGS := -lSDL2

check: build run clean

build:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

run: 
	./$(TARGET)

clean:
	rm -f $(TARGET)