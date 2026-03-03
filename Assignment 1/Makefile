CXX = g++
CXXFLAGS = -I/opt/homebrew/include -std=c++17
LDFLAGS= -L/opt/homebrew/lib -lsfml-graphics -lsfml-window -lsfml-system



TARGET = hermites_cube

SOURCES = $(wildcard *.cpp)

OBJECTS = $(SOURCES:.cpp=.o)


all: $(TARGET)


$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $^ $(LDFLAGS)


%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


run: all
	./$(TARGET)


clean:
	rm -f $(TARGET) $(OBJECTS)

.PHONY: all run clean
