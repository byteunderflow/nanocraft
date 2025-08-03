#include "program.hpp"

Program::Program()
{
    handle = glCreateProgram();
}

Program::~Program()
{
    glDeleteProgram(handle);
}

void Program::attach(Shader *shader) const
{
    glAttachShader(handle, shader->handle);
}

void Program::detach(Shader *shader) const
{
    glDetachShader(handle, shader->handle);
}

void Program::link() const
{
    glLinkProgram(handle);

    int success;
    glGetProgramiv(handle, GL_LINK_STATUS, &success);
    if (!success)
    {
        char log[512];
        glGetProgramInfoLog(handle, sizeof(log), nullptr, log);
        std::cerr << "Unable to link program: " << log;
        exit(EXIT_FAILURE);
    }
}

void Program::bind() const
{
    glUseProgram(handle);
}

void Program::unbind() const
{
    glUseProgram(0);
}

void Program::uniform(const char *name, const GLint value) const
{
    glUniform1i(glGetUniformLocation(handle, name), value);
}

void Program::uniform(const char *name, const glm::vec1 value) const
{
    glUniform1f(glGetUniformLocation(handle, name), value.x);
}

void Program::uniform(const char *name, const glm::vec2 value) const
{
    glUniform2f(glGetUniformLocation(handle, name), value.x, value.y);
}

void Program::uniform(const char *name, const glm::vec3 value) const
{
    glUniform3f(glGetUniformLocation(handle, name), value.x, value.y, value.z);
}

void Program::uniform(const char *name, const glm::vec4 value) const
{
    glUniform4f(glGetUniformLocation(handle, name), value.x, value.y, value.z, value.w);
}

void Program::uniform(const char *name, const glm::mat4 value) const
{
    glUniformMatrix4fv(glGetUniformLocation(handle, name), 1, GL_FALSE, glm::value_ptr(value));
}

void Program::uniform(const char *name, const Texture *value) const
{
    // Note: Translate texture unit to texture index
    uniform(name, value->unit - GL_TEXTURE0);
}