#pragma once

#include "vao.hpp"
#include "vbo.hpp"
#include "ebo.hpp"
#include "texture.hpp"
#include "program.hpp"

class Renderer
{
public:
    Renderer();
    ~Renderer();
    void init() const;
    void render() const;

private:
    Program program;
    VAO vao;
    VBO vbo;
    EBO ebo;
    Texture texture;
};