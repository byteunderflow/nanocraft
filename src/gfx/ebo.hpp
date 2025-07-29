#pragma once

#include <glad/glad.h>

class EBO
{
public:
    EBO();
    ~EBO();
    void fill(const GLuint *indices, const GLsizeiptr size) const;
    void bind() const;
    void unbind() const;
private:
    GLuint handle;
};