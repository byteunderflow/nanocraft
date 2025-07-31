#include "renderer.hpp"

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::init() const
{
    // DEBUG
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    texture.load("res/textures/textures.png");
    texture.bind();

    Shader vs(GL_VERTEX_SHADER);
    vs.compile("res/shaders/default.vs");

    Shader fs(GL_FRAGMENT_SHADER);
    fs.compile("res/shaders/default.fs");

    program.attach(&vs);
    program.attach(&fs);
    program.link();

    vao.bind();

    // pos (vec3) | texture coords (vec2)
    GLfloat vertices[] = {
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f};

    vbo.bind();
    vbo.fill(vertices, sizeof(vertices));

    GLuint indices[] = {
        0, 1, 3,
        1, 2, 3};

    ebo.bind();
    ebo.fill(indices, sizeof(indices));

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    program.bind();
}

void Renderer::render() const
{
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}