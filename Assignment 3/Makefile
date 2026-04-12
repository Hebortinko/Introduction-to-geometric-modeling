CXX = g++
CXXFLAGS = -I/opt/homebrew/include -I../ -Iexternal/eigen -std=c++20
LDFLAGS = -L/opt/homebrew/lib -lsfml-graphics -lsfml-window -lsfml-system

TARGET = hermite_spline

SOURCES = $(wildcard *.cpp) \
          $(wildcard App/*.cpp) \
          $(wildcard HermiteSpline/*.cpp) \
          $(wildcard Math/*.cpp) \
          $(wildcard UI/*.cpp)

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
