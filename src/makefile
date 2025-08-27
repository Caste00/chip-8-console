TARGET := eseguibile
SRC := main.cpp cpu.cpp
CXX := g++
CXXFLAGS := -std=c++17 -Wall -O2

check: build run clean

build:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: 
	./$(TARGET)

clean:
	rm -f $(TARGET)