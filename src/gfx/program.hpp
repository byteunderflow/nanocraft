#pragma once

#include "shader.hpp"
#include "texture.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Program
{
    GLuint handle;

    Program();
    ~Program();
    void attach(Shader &shader) const;
    void detach(Shader &shader) const;
    void link() const;
    void bind() const;
    void unbind() const;
    void uniform(const char *name, const GLint value) const;
    void uniform(const char *name, const glm::vec1 &value) const;
    void uniform(const char *name, const glm::vec2 &value) const;
    void uniform(const char *name, const glm::vec3 &value) const;
    void uniform(const char *name, const glm::vec4 &value) const;
    void uniform(const char *name, const glm::mat4 &value) const;
    void uniform(const char *name, const Texture &value) const;
};