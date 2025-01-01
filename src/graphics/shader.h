#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#define SHADER_CREATION_ERROR (GLuint)0

typedef struct
{
    GLuint id;
} Shader;

Shader *newShader(const char *vertPath, const char *fragPath);
GLuint createShader(const char *source, GLuint type);
void compileShader(GLuint shaderID);
void linkProgram(Shader *shader, GLuint vertexShader, GLuint fragmentShader);

#endif