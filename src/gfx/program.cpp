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

void Program::uniform(const char *name, const glm::vec1 vec1) const
{
    glUniform1f(glGetUniformLocation(handle, name), vec1.x);
}

void Program::uniform(const char *name, const glm::vec2 vec2) const
{
    glUniform2f(glGetUniformLocation(handle, name), vec2.x, vec2.y);
}

void Program::uniform(const char *name, const glm::vec3 vec3) const
{
    glUniform3f(glGetUniformLocation(handle, name), vec3.x, vec3.y, vec3.z);
}

void Program::uniform(const char *name, const glm::vec4 vec4) const
{
    glUniform4f(glGetUniformLocation(handle, name), vec4.x, vec4.y, vec4.z, vec4.w);
}