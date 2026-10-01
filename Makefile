CXX = g++
CXXFLAGS = -std=c++20 -Wall -O2 -Iinclude -Iinclude/imgui
LDFLAGS = -lglfw -lGL -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

# Detect platform
ifeq ($(OS),Windows_NT)
    LDFLAGS += -lopengl32 -lgdi32
    RM = del
else
    RM = rm -f
endif

IMGUI_SRC = $(wildcard src/imgui/*.cpp)

SRCS = $(wildcard *.cpp)
TARGET = sr

all: $(TARGET)

$(TARGET): $(SRCS) $(IMGUI_SRC)
	$(CXX) -g $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) $(TARGET)
