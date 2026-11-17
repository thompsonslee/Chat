CXX      := g++
CXXFLAGS := -std=c++17 -Wall -I/opt/homebrew/include -I.
LDFLAGS  := -L/opt/homebrew/lib
LDLIBS   := -lsodium

SRCS := $(shell find . -name '*.cpp')
OBJS := $(SRCS:.cpp=.o)

server: $(OBJS)
	$(CXX) $(OBJS) -o server $(LDFLAGS) $(LDLIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) server
