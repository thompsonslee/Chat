make
CXX      := g++
CXXFLAGS := -std=c++17 -Wall -I/opt/homebrew/include -I.
LDFLAGS  := -L/opt/homebrew/lib
LDLIBS   := -lsodium -lsqlite3

BUILD := build

debug: CXXFLAGS += -g -O0 -fsanitize=address,undefined -fno-omit-frame-pointer
debug: LDFLAGS  += -fsanitize=address,undefined
debug: server

SRCS := $(shell find . -name '*.cpp' -not -path './$(BUILD)/*')
OBJS := $(SRCS:./%.cpp=$(BUILD)/%.o)

server: $(OBJS)
	$(CXX) $(OBJS) -o server $(LDFLAGS) $(LDLIBS)

$(BUILD)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD) server
