#pragma once

#include "atlas.hpp"
#include "vao.hpp"
#include "vbo.hpp"
#include "program.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Renderer
{
    Atlas atlas;
    Program program;
    VAO vao;
    VBO vbo;

    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
    
    struct Settings
    {
        float fov = 60.0f;
        float near = 0.1f;
        float far = 100.0f;
    };
    Settings settings;

    void init();
    void renderBlock(float x, float y, float z, BlockType type);
    void renderChunk();
    void render(int width, int height);
};