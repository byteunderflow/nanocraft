#pragma once

#include <glad/glad.h>
#include <iostream>

class Texture
{
public:
    Texture();
    ~Texture();
    void bind() const;
    void unbind() const;
    void load(const char *path) const;

private:
    GLuint handle;
};