CXX = g++
	CXXFLAGS = -I/opt/homebrew/include -std=c++17
LDFLAGS= -L/opt/homebrew/lib -lsfml-graphics -lsfml-window -lsfml-system

TARGET = hermites_cube

all:
	$(CXX) main.cpp -o $(TARGET) $(CXXFLAGS) $(LDFLAGS)

run:
	./$(TARGET)

clean:
	rm -f $(TARGET)


