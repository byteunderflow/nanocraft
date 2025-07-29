#pragma once

#include <glad/glad.h>

class VBO
{
public:
    VBO();
    ~VBO();
    void fill(const float *data, const GLsizeiptr size) const;
    void bind() const;
    void unbind() const;
private:
    GLuint handle;
};