CXX=clang++

all: main.out

main.out: main.cpp
	$(CXX) $(shell wx-config --cxxflags --libs) $< -o $@

clean:
	rm -f main.out

re: clean all
