
BIN_TARGET = str_tests
SRC_FILES  = tests.cpp
CXXFLAGS  += -std=c++11 -O3 -Wall -Wextra -Weffc++ -Wshadow -pedantic

all:
	$(CXX) $(CXXFLAGS) $(SRC_FILES) -o $(BIN_TARGET)

clean:
	rm -f *.o
	rm -f $(BIN_TARGET)

