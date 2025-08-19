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

void Texture::bind(GLenum unit)
{
    this->unit = unit;
    glActiveTexture(unit);
    glBindTexture(GL_TEXTURE_2D, handle);
}

void Texture::unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::load(const char *path, GLenum format) const
{
    stbi_set_flip_vertically_on_load(true);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width;
    int height;
    int channels;
    stbi_uc *data = stbi_load(path, &width, &height, &channels, STBI_rgb_alpha);
    if (!data)
    {
        std::cerr << "Unable to load texture " << path << ": " << stbi_failure_reason() << std::endl;
        exit(EXIT_FAILURE);
    }

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
}