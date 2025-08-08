#include "renderer.hpp"

void ChunkMesh::addFace(uint32 x, uint32 y, uint32 z, Blocks::Type type, Blocks::Face face)
{
    constexpr float32 width = 1.0f / 16.0f;
    constexpr float32 height = 1.0f / 16.0f;
    const uint8 row = static_cast<uint8>(type) - 1;
    const uint8 column = static_cast<uint8>(face);
    const float32 u1 = column * width;
    const float32 v2 = 1.0f - row * height;
    const float32 u2 = (column + 1) * width;
    const float32 v1 = v2 - height;

    const GLuint index = static_cast<GLuint>(vertices.size());

    Vertex vertex1 = VERTICES[face][0];
    vertex1.x += x;
    vertex1.y += y;
    vertex1.z += z;
    vertex1.u = u1;
    vertex1.v = v2;

    Vertex vertex2 = VERTICES[face][1];
    vertex2.x += x;
    vertex2.y += y;
    vertex2.z += z;
    vertex2.u = u2;
    vertex2.v = v2;

    Vertex vertex3 = VERTICES[face][2];
    vertex3.x += x;
    vertex3.y += y;
    vertex3.z += z;
    vertex3.u = u2;
    vertex3.v = v1;

    Vertex vertex4 = VERTICES[face][3];
    vertex4.x += x;
    vertex4.y += y;
    vertex4.z += z;
    vertex4.u = u1;
    vertex4.v = v1;

    vertices.push_back(vertex1);
    vertices.push_back(vertex2);
    vertices.push_back(vertex3);
    vertices.push_back(vertex4);

    indices.push_back(index + 0);
    indices.push_back(index + 1);
    indices.push_back(index + 2);
    indices.push_back(index + 2);
    indices.push_back(index + 3);
    indices.push_back(index + 0);
}

void Renderer::init()
{
    glEnable(GL_DEPTH_TEST);

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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    ebo.bind();
}

void Renderer::renderChunk(Chunk &chunk)
{
    // Todo: Ony rebuild chunk mesh if required
    ChunkMesh mesh;
    for (uint32 x = 0; x < Chunk::CHUNK_X; ++x)
    {
        for (uint32 y = 0; y < Chunk::CHUNK_Y; ++y)
        {
            for (uint32 z = 0; z < Chunk::CHUNK_Z; ++z)
            {
                const Blocks::Block block = chunk.getBlock(x, y, z);
                const Blocks::Type type = Blocks::getType(block);
                if (type == Blocks::Type::AIR)
                    continue;

                // Optimization: Face culling (render only visible faces)
                if (z + 1 == Chunk::CHUNK_Z || chunk.getBlock(x, y, z + 1) & Blocks::MASK_TRANSPARENT)
                    mesh.addFace(x, y, z, type, Blocks::Face::FRONT);
                if (z == 0 || chunk.getBlock(x, y, z - 1) & Blocks::MASK_TRANSPARENT)
                    mesh.addFace(x, y, z, type, Blocks::Face::BACK);
                if (x == 0 || chunk.getBlock(x - 1, y, z) & Blocks::MASK_TRANSPARENT)
                    mesh.addFace(x, y, z, type, Blocks::Face::LEFT);
                if (x + 1 == Chunk::CHUNK_X || chunk.getBlock(x + 1, y, z) & Blocks::MASK_TRANSPARENT)
                    mesh.addFace(x, y, z, type, Blocks::Face::RIGHT);
                if (y + 1 == Chunk::CHUNK_Y || chunk.getBlock(x, y + 1, z) & Blocks::MASK_TRANSPARENT)
                    mesh.addFace(x, y, z, type, Blocks::Face::TOP);
                if (y == 0 || chunk.getBlock(x, y - 1, z) & Blocks::MASK_TRANSPARENT)
                    mesh.addFace(x, y, z, type, Blocks::Face::BOTTOM);
            }
        }
    }

    const glm::mat4 model =
        glm::translate(glm::mat4(1.0f), glm::vec3(static_cast<float32>(chunk.position.x), 0.0f, static_cast<float32>(chunk.position.z)));
    program.uniform("model", model);

    vbo.fill(mesh.vertices.data(), mesh.vertices.size() * sizeof(Vertex));
    ebo.fill(mesh.indices.data(), mesh.indices.size() * sizeof(GLuint));

    glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
}

void Renderer::renderWorld(World &world)
{
    for (const std::unique_ptr<Chunk> &chunk : world.chunks)
    {
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

    const glm::mat4 view =
        glm::lookAt(world.player.position, world.player.position + world.player.direction, world.player.up);
    program.uniform("view", view);

    const glm::mat4 projection =
        glm::perspective(glm::radians(settings.fov), (float)width / height, settings.near, settings.far);
    program.uniform("projection", projection);

    renderWorld(world);
}