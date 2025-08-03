#pragma once

#include <glad/glad.h>
#include <iostream>

class Texture
{
public:
    GLenum unit;

    Texture();
    ~Texture();
    void bind(GLenum unit);
    void unbind() const;
    void load(const char *path, GLenum format) const;

private:
    GLuint handle;
};