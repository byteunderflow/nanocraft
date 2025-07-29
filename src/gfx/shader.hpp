#pragma once

#include <glad/glad.h>
#include <iostream>
#include <fstream>

class Shader
{
public:
    GLuint handle;

    Shader(GLenum type);
    ~Shader();
    bool compile(const char *path) const;
};