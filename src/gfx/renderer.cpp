#include "renderer.hpp"

void Renderer::init()
{
    glEnable(GL_DEPTH_TEST);

    // Setup vertex shader
    Shader vs(GL_VERTEX_SHADER);
    vs.compile("res/shaders/default.vs");

    // Setup fragment shader
    Shader fs(GL_FRAGMENT_SHADER);
    fs.compile("res/shaders/default.fs");

    // Setup shader program
    program.attach(vs);
    program.attach(fs);
    program.link();
    program.bind();

    // Setup texture atlas
    atlas.bind();
    atlas.load("res/textures/atlas.png");
    program.uniform("atlas", atlas.texture);
}

void Renderer::renderBlock(float x, float y, float z, Block block)
{
    const BlockType type = getBlockType(block);
    if (type == BlockType::AIR)
        return;
    
    // const bool isSolid = getBlockFlag(block, SOLID);
    // const bool isTransparent = getBlockFlag(block, TRANSPARENT);

    // Setup model matrix
    model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
    program.uniform("model", model);

    // Compute texture coordinates for each block face
    TextureCoordinates front;
    atlas.compute(type, BlockFace::FRONT, front);
    TextureCoordinates back;
    atlas.compute(type, BlockFace::BACK, back);
    TextureCoordinates left;
    atlas.compute(type, BlockFace::LEFT, left);
    TextureCoordinates right;
    atlas.compute(type, BlockFace::RIGHT, right);
    TextureCoordinates top;
    atlas.compute(type, BlockFace::TOP, top);
    TextureCoordinates bottom;
    atlas.compute(type, BlockFace::BOTTOM, bottom);

    vao.bind();

    const float vertices[] = {
        // Front face
        -0.5f, -0.5f, 0.5f, front.u1, front.v1, // Bottom left
        0.5f, -0.5f, 0.5f, front.u2, front.v1,  // Bottom right
        0.5f, 0.5f, 0.5f, front.u2, front.v2,   // Top right
        0.5f, 0.5f, 0.5f, front.u2, front.v2,   // Top right
        -0.5f, 0.5f, 0.5f, front.u1, front.v2,  // Top left
        -0.5f, -0.5f, 0.5f, front.u1, front.v1, // Bottom left

        // Back face
        -0.5f, -0.5f, -0.5f, back.u1, back.v1, // Bottom left
        0.5f, -0.5f, -0.5f, back.u2, back.v1,  // Bottom right
        0.5f, 0.5f, -0.5f, back.u2, back.v2,   // Top right
        0.5f, 0.5f, -0.5f, back.u2, back.v2,   // Top right
        -0.5f, 0.5f, -0.5f, back.u1, back.v2,  // Top left
        -0.5f, -0.5f, -0.5f, back.u1, back.v1, // Bottom left

        // Left face
        -0.5f, 0.5f, 0.5f, left.u2, left.v2,   // Top right
        -0.5f, 0.5f, -0.5f, left.u1, left.v2,  // Top left
        -0.5f, -0.5f, -0.5f, left.u1, left.v1, // Bottom left
        -0.5f, -0.5f, -0.5f, left.u1, left.v1, // Bottom left
        -0.5f, -0.5f, 0.5f, left.u2, left.v1,  // Bottom right
        -0.5f, 0.5f, 0.5f, left.u2, left.v2,   // Top right

        // Right face
        0.5f, 0.5f, 0.5f, right.u1, right.v2,   // Top left
        0.5f, 0.5f, -0.5f, right.u2, right.v2,  // Top right
        0.5f, -0.5f, -0.5f, right.u2, right.v1, // Bottom right
        0.5f, -0.5f, -0.5f, right.u2, right.v1, // Bottom right
        0.5f, -0.5f, 0.5f, right.u1, right.v1,  // Bottom left
        0.5f, 0.5f, 0.5f, right.u1, right.v2,   // Top left

        // Top face
        -0.5f, 0.5f, -0.5f, top.u1, top.v2, // Top left
        0.5f, 0.5f, -0.5f, top.u2, top.v2,  // Top right
        0.5f, 0.5f, 0.5f, top.u2, top.v1,   // Bottom right
        0.5f, 0.5f, 0.5f, top.u2, top.v1,   // Bottom right
        -0.5f, 0.5f, 0.5f, top.u1, top.v1,  // Bottom left
        -0.5f, 0.5f, -0.5f, top.u1, top.v2, // Top left

        // Bottom face
        -0.5f, -0.5f, -0.5f, bottom.u1, bottom.v2, // Top left
        0.5f, -0.5f, -0.5f, bottom.u2, bottom.v2,  // Top right
        0.5f, -0.5f, 0.5f, bottom.u2, bottom.v1,   // Bottom right
        0.5f, -0.5f, 0.5f, bottom.u2, bottom.v1,   // Bottom right
        -0.5f, -0.5f, 0.5f, bottom.u1, bottom.v1,  // Bottom left
        -0.5f, -0.5f, -0.5f, bottom.u1, bottom.v2  // Top left
    };

    vbo.bind();
    vbo.fill(vertices, sizeof(vertices));

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Renderer::renderChunk(Chunk &chunk)
{
    for (int x = 0; x < CHUNK_X; ++x)
    {
        for (int y = 0; y < CHUNK_Y; ++y)
        {
            for (int z = 0; z < CHUNK_Z; ++z)
            {
                const Block block = chunk.getBlock(x, y, z);
                const float bx = static_cast<float>(chunk.x + x);
                const float by = static_cast<float>(y);
                const float bz = static_cast<float>(chunk.z + z);
                renderBlock(bx, by, bz, block);
            }
        }
    }
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

    // Setup view matrix
    program.uniform("view", world.player.view());

    // Setup projection matrix
    projection = glm::perspective(glm::radians(settings.fov), (float)width / height, settings.near, settings.far);
    program.uniform("projection", projection);

    glViewport(0, 0, width, height);
    glClearColor(0.5f, 0.85f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, settings.wireframe ? GL_LINE : GL_FILL);

    renderWorld(world);
}