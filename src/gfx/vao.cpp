#include "vao.hpp"

VAO::VAO()
{
    glGenVertexArrays(1, &handle);
}

VAO::~VAO()
{
    glDeleteVertexArrays(1, &handle);
}

void VAO::bind() const
{
    glBindVertexArray(handle);
}

void VAO::unbind() const
{
    glBindVertexArray(0);
}