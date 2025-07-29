#include "vbo.hpp"

VBO::VBO()
{
    glGenBuffers(1, &handle);
}

VBO::~VBO()
{
    glDeleteBuffers(1, &handle);
}

void VBO::fill(const float *data, const GLsizeiptr size) const
{
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
}

void VBO::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, handle);
}

void VBO::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}