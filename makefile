CXX=g++
FLAGS=-std=c++14
SOURCE=main.cpp
TARGET=projekt

all: $(TARGET)
	./$(TARGET)

$(TARGET): $(SOURCE)
	$(CXX) $(FLAGS) $(SOURCE) -o $(TARGET)

clean:
	rm -f *.txt
	rm -f $(TARGET)
