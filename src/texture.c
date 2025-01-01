#include <texture.h>
#include <stb_image.h>
#include <glad/glad.h>
#include <allocator.h>

Texture *newTextureFromImage(const char *path)
{
    Texture *texture = ALLOCATE(Texture, 1);
    int width, height, nrChannels;

    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);

    if (data)
    {
        glGenTextures(1, &texture->id);
        glBindTexture(GL_TEXTURE_2D, texture->id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        if (nrChannels == 3)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        else if (nrChannels == 4)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }
        else
        {
            stbi_image_free(data);
            return NULL;
        }

        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);

        texture->width = width;
        texture->height = height;
    }
    else
    {
        printf("Error to load texture: %s\n", path);
        return NULL;
    }

    return texture;
}

void bindTexture(Texture *texture)
{
    glBindTexture(GL_TEXTURE_2D, texture->id);
}

void unbindTextures()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
