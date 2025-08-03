#include "renderer.hpp"

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::init()
{
    glEnable(GL_DEPTH_TEST);
    // DEBUG
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    texture.bind(GL_TEXTURE0);
    texture.load("res/textures/grass.png", GL_RGBA);

    Shader vs(GL_VERTEX_SHADER);
    vs.compile("res/shaders/default.vs");

    Shader fs(GL_FRAGMENT_SHADER);
    fs.compile("res/shaders/default.fs");

    program.attach(&vs);
    program.attach(&fs);
    program.link();

    vao.bind();

    GLfloat vertices[] = {
        // Back face
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // Bottom left
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  // Bottom right
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   // Top right
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   // Top right
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,  // Top left
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // Bottom left

        // Front face
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // Bottom left
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,  // Bottom right
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,   // Top right
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,   // Top right
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,  // Top left
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // Bottom left

        // Left face
        -0.5f, 0.5f, 0.5f, 1.0f, 1.0f,   // Top right
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,  // Top left
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // Bottom left
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // Bottom left
        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f,  // Bottom right
        -0.5f, 0.5f, 0.5f, 1.0f, 1.0f,   // Top right

        // Right face
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f,   // Top left
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,  // Top right
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // Bottom right
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // Bottom right
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  // Bottom left
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f,   // Top left

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f};

    vbo.bind();
    vbo.fill(vertices, sizeof(vertices));

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    program.bind();
    program.uniform("tex", &texture);
}

void Renderer::render(int width, int height) const
{
    if (width <= 0 && height <= 0)
        return;

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);

    program.uniform("model", model);
    program.uniform("view", view);
    program.uniform("projection", projection);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}