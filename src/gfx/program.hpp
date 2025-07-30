#pragma once

#include "shader.hpp"
#include <glm/glm.hpp>

class Program
{
public:
    Program();
    ~Program();
    void attach(Shader *shader) const;
    void link() const;
    void bind() const;
    void unbind() const;
    void uniform(const char *name, const glm::vec1 vec1) const;
    void uniform(const char *name, const glm::vec2 vec2) const;
    void uniform(const char *name, const glm::vec3 vec3) const;
    void uniform(const char *name, const glm::vec4 vec4) const;

private:
    GLuint handle;
};