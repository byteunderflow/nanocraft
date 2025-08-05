#pragma once

#include <glad/glad.h>
#include <iostream>
#include <fstream>

struct Shader
{
    GLuint handle;

    Shader(GLenum type);
    ~Shader();
    void compile(const char *path) const;
};