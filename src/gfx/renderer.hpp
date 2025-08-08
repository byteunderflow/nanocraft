#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.hpp"
#include "vao.hpp"
#include "vbo.hpp"
#include "ebo.hpp"
#include "program.hpp"

#include "../game/world.hpp"

struct Vertex
{
    GLfloat x;
    GLfloat y;
    GLfloat z;
    GLfloat u;
    GLfloat v;
};

constexpr const Vertex VERTICES[6][4] = {
    {
        {-0.5f, 0.5f, 0.5f, 0.0f, 0.0f}, // Top left
        {0.5f, 0.5f, 0.5f, 0.0f, 0.0f},  // Top right
        {0.5f, -0.5f, 0.5f, 0.0f, 0.0f}, // Bottom right
        {-0.5f, -0.5f, 0.5f, 0.0f, 0.0f} // Bottom left
    },

    {
        {-0.5f, 0.5f, -0.5f, 0.0f, 0.0f}, // Top left
        {0.5f, 0.5f, -0.5f, 0.0f, 0.0f},  // Top right
        {0.5f, -0.5f, -0.5f, 0.0f, 0.0f}, // Bottom right
        {-0.5f, -0.5f, -0.5f, 0.0f, 0.0f} // Bottom left
    },

    {
        {-0.5f, 0.5f, -0.5f, 0.0f, 0.0f}, // Top left
        {-0.5f, 0.5f, 0.5f, 0.0f, 0.0f},  // Top right
        {-0.5f, -0.5f, 0.5f, 0.0f, 0.0f}, // Bottom right
        {-0.5f, -0.5f, -0.5f, 0.0f, 0.0f} // Bottom left
    },

    {
        {0.5f, 0.5f, 0.5f, 0.0f, 0.0f},   // Top left
        {0.5f, 0.5f, -0.5f, 0.0f, 0.0f},  // Top right
        {0.5f, -0.5f, -0.5f, 0.0f, 0.0f}, // Bottom right
        {0.5f, -0.5f, 0.5f, 0.0f, 0.0f}   // Bottom left
    },

    {
        {-0.5f, 0.5f, -0.5f, 0.0f, 0.0f}, // Top left
        {0.5f, 0.5f, -0.5f, 0.0f, 0.0f},  // Top right
        {0.5f, 0.5f, 0.5f, 0.0f, 0.0f},   // Bottom right
        {-0.5f, 0.5f, 0.5f, 0.0f, 0.0f}   // Bottom left
    },

    {
        {-0.5f, -0.5f, -0.5f, 0.0f, 0.0f}, // Top left
        {0.5f, -0.5f, -0.5f, 0.0f, 0.0f},  // Top right
        {0.5f, -0.5f, 0.5f, 0.0f, 0.0f},   // Bottom right
        {-0.5f, -0.5f, 0.5f, 0.0f, 0.0f}   // Bottom left
    }};

struct ChunkMesh
{
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    void addFace(uint32 x, uint32 y, uint32 z, Blocks::Type type, Blocks::Face face);
};

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
        float far = 100.0f;
    };
    Settings settings;

    void init();
    void renderChunk(Chunk &chunk);
    void renderWorld(World &world);
    void render(int width, int height, World &world);
};