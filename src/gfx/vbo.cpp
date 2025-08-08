#include "vbo.hpp"

VBO::VBO()
{
    glGenBuffers(1, &handle);
}

VBO::~VBO()
{
    glDeleteBuffers(1, &handle);
}

void VBO::fill(const void *data, const GLsizeiptr size) const
{
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void VBO::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, handle);
}

void VBO::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}