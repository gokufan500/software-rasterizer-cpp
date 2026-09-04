CXX = g++
CXXFLAGS = -std=c++20 -Wall -O2 -Iinclude -Iinclude/imgui
LDFLAGS = -lglfw -lGL -ldl -lpthread -lvulkan -lX11 -lXxf86vm -lXrandr -lXi

# Detect platform
ifeq ($(OS),Windows_NT)
    LDFLAGS += -lopengl32 -lgdi32
    RM = del
else
    RM = rm -f
endif


SRCS = $(wildcard *.cpp)
TARGET = sr

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) -g $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) $(TARGET)
