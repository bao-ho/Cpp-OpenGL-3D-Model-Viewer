#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <GL/glew.h>
#include <string>

MyTexture::MyTexture(const char* path, unsigned int slot)
    : textureImageFilePath(path), textureSlot(slot)
{
    int width, height, nChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* textureImageData = stbi_load(path, &width, &height, &nChannels, 0);
    unsigned int texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, texture);
    if (std::strstr(path, ".png") != NULL)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureImageData);
    else if (std::strstr(path, ".jpg") != NULL)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureImageData);
    
    stbi_image_free(textureImageData);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void MyTexture::bindToShader(unsigned int shader, const char* uniform)
{
    glUniform1i(glGetUniformLocation(shader, uniform), this->textureSlot);
}
