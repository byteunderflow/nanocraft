#pragma once

#include "vao.hpp"
#include "vbo.hpp"
#include "ebo.hpp"
#include "texture.hpp"
#include "program.hpp"
#include "camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera;
class Window;

class Renderer
{
public:
    Renderer(const Window *window) : window(window) {};
    ~Renderer() = default;
    void init();
    void render() const;

private:
    const Window *window;
    Program program;
    VAO vao;
    VBO vbo;
    EBO ebo;
    Texture texture;
};