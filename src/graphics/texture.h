#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

typedef struct
{
    GLuint id;
    int width;
    int height;
} Texture;

Texture *newTextureFromImage(const char *path);
void bindTexture(Texture *texture);
void unbindTextures();

#endif