#pragma once

#include <glad/glad.h>

struct VBO
{
    GLuint handle;

    VBO();
    ~VBO();
    void fill(const float *data, const GLsizeiptr size) const;
    void bind() const;
    void unbind() const;
};