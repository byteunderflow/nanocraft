#pragma once

#include <glad/glad.h>

struct EBO
{
    GLuint handle;

    EBO();
    ~EBO();
    void fill(const GLuint *indices, const GLsizeiptr size) const;
    void bind() const;
    void unbind() const;
};