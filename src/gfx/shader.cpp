#include "shader.hpp"

Shader::Shader(GLenum type)
{
    handle = glCreateShader(type);
}

Shader::~Shader()
{
    glDeleteShader(handle);
}

bool Shader::compile(const char *path) const
{
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file)
    {
        std::cerr << "Unable to open file " << path << std::endl;
        return false;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::unique_ptr<char[]> buffer(new char[size + 1]);
    if (!file.read(buffer.get(), size))
    {
        std::cerr << "Unable to read file " << path << std::endl;
        return false;
    }

    buffer[size] = 0;

    const GLchar *source = buffer.get();
    glShaderSource(handle, 1, &source, nullptr);
    glCompileShader(handle);

    int success;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char log[512];
        glGetShaderInfoLog(handle, sizeof(log), nullptr, log);
        std::cerr << "Unable to compile file " << path << ": " << log;
        return false;
    }

    return true;
}