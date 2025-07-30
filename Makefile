CC = clang
CXXC = clang++
CXXFLAGS = -std=c++20 -g -Wall -Wextra -Wpedantic -Wstrict-aliasing
CXXFLAGS += -Ilib/glad/include -Ilib/glfw/include -Ilib/glm -Ilib/stb
LDFLAGS = lib/glad/glad.o lib/glfw/build/src/libglfw3.a -lgdi32

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

compile: $(OBJ)
	$(CXXC) -o $(EXE) $^ $(LDFLAGS)

%.o: %.cpp
	$(CXXC) -c $< -o $@ $(CXXFLAGS)

run:
	$(EXE)

clean:
	del /q /f $(subst /,\,$(OBJ))
	rmdir /s /q $(BIN)