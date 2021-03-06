CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wextra -Wall -Wno-sign-compare -pedantic
NAME = solution

all: big_int.o
	$(CXX) $(CXXFLAGS) big_int.o main.cpp -o $(NAME)
big_int.o: big_int.hpp
	$(CXX) $(CXXFLAGS) -c big_int.cpp
clean:
	rm -f *.o $(NAME)