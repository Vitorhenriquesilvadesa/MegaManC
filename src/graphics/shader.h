#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#define CGLM_USE_STRUCT
#include <cglm/struct.h>

#define SHADER_CREATION_ERROR (GLuint)0

typedef struct
{
    GLuint id;
} Shader;

typedef enum
{
    SHADER_TYPE_SPRITE,
    SHADER_TYPE_MAX,
} ShaderType;

Shader *newShader(const char *vertPath, const char *fragPath);
GLuint createShader(const char *source, GLuint type);
void compileShader(GLuint shaderID);
void linkProgram(Shader *shader, GLuint vertexShader, GLuint fragmentShader);

void shaderSetInt(Shader *shader, const char *name, int value);
void shaderSetMat4(Shader *shader, const char *name, mat4s value);

void bindShader(Shader *shader);
void unbindShaders();

#endif