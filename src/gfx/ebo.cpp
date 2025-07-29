#include "ebo.hpp"

EBO::EBO()
{
    glGenBuffers(1, &handle);
}

EBO::~EBO()
{
    glDeleteBuffers(1, &handle);
}

void EBO::fill(const GLuint *indices, const GLsizeiptr size) const
{
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}

void EBO::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
}

void EBO::unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}