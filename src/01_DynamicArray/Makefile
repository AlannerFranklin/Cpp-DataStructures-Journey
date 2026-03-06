CXX = g++
CXXFLAGS = -std=c++17 -Wall -g

TARGET = main
SRCS = main.cpp
HEADERS = MyVector.hpp

all: $(TARGET)

$(TARGET): $(SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)
