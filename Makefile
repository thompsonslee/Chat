CXX      := g++
CXXFLAGS := -std=c++17 -Wall -I/opt/homebrew/include -I.
LDFLAGS  := -L/opt/homebrew/lib
LDLIBS   := -lsodium -lsqlite3

debug: CXXFLAGS += -g -O0 -fsanitize=address,undefined -fno-omit-frame-pointer
debug: LDFLAGS  += -fsanitize=address,undefined
debug: server

SRCS := $(shell find . -name '*.cpp')
OBJS := $(SRCS:.cpp=.o)

server: $(OBJS)
	$(CXX) $(OBJS) -o server $(LDFLAGS) $(LDLIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) server
