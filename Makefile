CC = clang
CXX = clang++
CXXFLAGS = -std=c++20 -g -Wall -Wextra -Wpedantic -Wstrict-aliasing
CXXFLAGS += -Ilib/glad/include -Ilib/glfw/include -Ilib/glm -Ilib/stb -Ilib/imgui
LDFLAGS = -fuse-ld=lld lib/glad/glad.o lib/glfw/build/src/libglfw3.a \
	lib/imgui/imgui.o lib/imgui/imgui_draw.o lib/imgui/imgui_tables.o lib/imgui/imgui_widgets.o lib/imgui/imgui_impl_glfw.o lib/imgui/imgui_impl_opengl3.o \
	-lgdi32

SRC = $(wildcard src/*.cpp) $(wildcard src/**/*.cpp)
OBJ = $(SRC:.cpp=.o)
BIN = bin
EXE = $(BIN)\nanocraft.exe

.PHONY: all dirs libs compile run clean

all: dirs libs compile

dirs:
	mkdir $(BIN)

libs:
	cd lib\glad && $(CC) -c src\glad.c -o glad.o -Iinclude

	cd lib\glfw && cmake -G "MinGW Makefiles" -B build -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF -DBUILD_SHARED_LIBS=OFF
	cd lib\glfw && cmake --build build

	cd lib\imgui && $(CXX) -c imgui.cpp imgui_draw.cpp imgui_tables.cpp imgui_widgets.cpp backends/imgui_impl_glfw.cpp backends/imgui_impl_opengl3.cpp \
		-I. -Ibackends -I../glfw/include

compile: $(OBJ)
	$(CXX) -o $(EXE) $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

run:
	$(EXE)

clean:
	del /q /f $(subst /,\,$(OBJ))
	del /q lib\imgui\*.o
	rmdir /s /q $(BIN)