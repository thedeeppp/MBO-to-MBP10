CXX = g++
CXXFLAGS = -std=c++17 -O3 -Wall
TARGET = reconstruction_deep

$(TARGET): reconstruction_deep.cpp
	$(CXX) $(CXXFLAGS) reconstruction_deep.cpp -o $(TARGET)

clean:
	rm -f $(TARGET)
