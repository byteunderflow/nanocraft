#pragma once

#include "vao.hpp"
#include "vbo.hpp"
#include "ebo.hpp"
#include "texture.hpp"
#include "program.hpp"
#include "camera.hpp"

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Renderer
{
public:
    Renderer();
    ~Renderer();
    void init();
    void render(int width, int height) const;

private:
    Program program;
    VAO vao;
    VBO vbo;
    EBO ebo;
    Texture texture;
};