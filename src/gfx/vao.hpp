#pragma once

#include <glad/glad.h>

struct VAO
{
    GLuint handle;

    VAO();
    ~VAO();
    void bind() const;
    void unbind() const;
};