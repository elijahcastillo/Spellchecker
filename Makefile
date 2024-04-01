CC = g++
CXXFLAGS = -std=c++17
SRCS = $(wildcard src/*.cpp)
HEADERS = $(wildcard src/include/*.h)
TARGET = spellcheck

$(TARGET): $(SRCS) $(HEADERS)
	$(CC) $(CXXFLAGS) $(SRCS) -o $(TARGET) -O2

