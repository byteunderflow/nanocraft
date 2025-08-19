#include "renderer.hpp"

void Renderer::init()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Shader vs(GL_VERTEX_SHADER);
    vs.compile("res/shaders/default.vs");

    Shader fs(GL_FRAGMENT_SHADER);
    fs.compile("res/shaders/default.fs");

    program.attach(vs);
    program.attach(fs);
    program.link();
    program.bind();

    atlas.bind(GL_TEXTURE0);
    atlas.load("res/textures/atlas.png", GL_RGBA);
    program.uniform("atlas", atlas);

    vao.bind();

    vbo.bind();

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Chunks::Vertex), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Chunks::Vertex), (void *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    ebo.bind();
}

void Renderer::renderChunk(Chunks::Chunk &chunk)
{
    chunk.render();

    const glm::mat4 model = glm::translate(
        glm::mat4(1.0f),
        glm::vec3(
            static_cast<float32>(chunk.position.x) * static_cast<float32>(Chunks::Chunk::SIZE_X),
            0.0f,
            static_cast<float32>(chunk.position.z) * static_cast<float32>(Chunks::Chunk::SIZE_Z)));
    program.uniform("model", model);

    vbo.fill(chunk.mesh->vertices.data(), chunk.mesh->vertices.size() * sizeof(Chunks::Vertex));
    ebo.fill(chunk.mesh->indices.data(), chunk.mesh->indices.size() * sizeof(GLuint));

    glDrawElements(GL_TRIANGLES, chunk.mesh->indices.size(), GL_UNSIGNED_INT, 0);
}

void Renderer::renderWorld(World &world)
{
    for (const auto &[position, chunk] : world.chunks)
    {
        if (chunk)
            renderChunk(*chunk);
    }
}

void Renderer::render(int width, int height, World &world)
{
    if (width <= 0 || height <= 0)
    {
        return;
    }

    glViewport(0, 0, width, height);
    glClearColor(0.5f, 0.85f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, settings.wireframe ? GL_LINE : GL_FILL);

    const glm::mat4 view = glm::lookAt(
        world.player->position,
        world.player->position + world.player->direction,
        world.player->up);
    program.uniform("view", view);

    const glm::mat4 projection = glm::perspective(
        glm::radians(settings.fov),
        (float)width / height,
        settings.near,
        settings.far);
    program.uniform("projection", projection);

    renderWorld(world);
}