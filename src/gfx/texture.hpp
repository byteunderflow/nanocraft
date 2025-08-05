#pragma once

#include <glad/glad.h>
#include <iostream>

struct Texture
{
    GLuint handle;
    GLenum unit;

    Texture();
    ~Texture();
    void bind(GLenum unit);
    void unbind() const;
    void load(const char *path, GLenum format) const;
};