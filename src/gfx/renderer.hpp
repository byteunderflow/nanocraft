#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.hpp"
#include "vao.hpp"
#include "vbo.hpp"
#include "ebo.hpp"
#include "program.hpp"

#include "../game/world.hpp"

struct Renderer
{
    Texture atlas;
    Program program;
    VAO vao;
    VBO vbo;
    EBO ebo;

    struct Settings
    {
        bool wireframe = false;
        float fov = 60.0f;
        float near = 0.1f;
        float far = 1000.0f;
    };
    Settings settings;

    void init();
    void renderChunk(Chunks::Chunk &chunk);
    void renderWorld(World &world);
    void render(int width, int height, World &world);
};