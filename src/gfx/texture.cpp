#include "texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture()
{
    glGenTextures(1, &handle);
}

Texture::~Texture()
{
    glDeleteTextures(1, &handle);
}

void Texture::bind() const
{
    glBindTexture(GL_TEXTURE_2D, handle);
}

void Texture::unbind() const
{
    glBindTexture(GL_TEXTURE_2D, handle);
}

void Texture::load(const char *path) const
{
    int width;
    int height;
    int nchannels;

    stbi_uc *data = stbi_load(path, &width, &height, &nchannels, 0);
    if (!data)
    {
        std::cerr << "Unable to load texture " << path << std::endl;
        exit(EXIT_FAILURE);
    }

    // Note: Change pixel format to GL_RGBA when dealing with transparency
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
}