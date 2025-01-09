#include <shader.h>
#include <allocator.h>
#include <stdio.h>
#include <files.h>

Shader *newShader(const char *vertPath, const char *fragPath)
{
    Shader *shader = ALLOCATE(Shader, 1);

    shader->id = glCreateProgram();

    const char *vertSource = readFile(vertPath);
    const char *fragSource = readFile(fragPath);

    if (!vertSource || !fragSource)
    {
        fprintf(stderr, "Failed to read shader files.\n");
        FREE((void *)vertSource);
        FREE((void *)fragSource);
        FREE(shader);
        return NULL;
    }

    GLuint vertexShader = createShader(vertSource, GL_VERTEX_SHADER);
    GLuint fragmentShader = createShader(fragSource, GL_FRAGMENT_SHADER);

    FREE((void *)vertSource);
    FREE((void *)fragSource);

    if (vertexShader == SHADER_CREATION_ERROR)
    {
        fprintf(stderr, "ERROR IN VERTEX SHADER\n");
        FREE(shader);
        return NULL;
    }

    if (fragmentShader == SHADER_CREATION_ERROR)
    {
        fprintf(stderr, "ERROR IN FRAGMENT SHADER\n");
        glDeleteShader(vertexShader);
        FREE(shader);
        return NULL;
    }

    glAttachShader(shader->id, vertexShader);
    glAttachShader(shader->id, fragmentShader);

    linkProgram(shader, vertexShader, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shader;
}

GLuint createShader(const char *source, GLuint type)
{
    GLuint shader = glCreateShader(type);

    if (!shader)
    {
        printf("Failed to create shader\n");
        return SHADER_CREATION_ERROR;
    }

    glShaderSource(shader, 1, &source, NULL);

    compileShader(shader);

    return shader;
}

void compileShader(GLuint shaderID)
{
    glCompileShader(shaderID);

    GLint success;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        char log[512];
        glGetShaderInfoLog(shaderID, sizeof(log), NULL, log);
        fprintf(stderr, "Shader compilation failed:\n\t%s\n", log);
        glDeleteShader(shaderID);
    }
}

void linkProgram(Shader *shader, GLuint vertexShader, GLuint fragmentShader)
{
    glLinkProgram(shader->id);

    GLint success;
    glGetProgramiv(shader->id, GL_LINK_STATUS, &success);
    if (!success)
    {
        char log[512];
        glGetProgramInfoLog(shader->id, sizeof(log), NULL, log);
        fprintf(stderr, "Shader program linking failed:\n%s\n", log);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteProgram(shader->id);
        FREE(shader);
    }
}

void shaderSetInt(Shader *shader, const char *name, int value)
{
    GLuint location = glGetUniformLocation(shader->id, name);
    glUniform1i(location, value);
}

void shaderSetVec2(Shader *shader, const char *name, vec2s value)
{
    GLuint location = glGetUniformLocation(shader->id, name);
    glUniform2fv(location, 1, (const GLfloat *)value.raw);
}

void shaderSetMat4(Shader *shader, const char *name, mat4s value)
{
    GLuint location = glGetUniformLocation(shader->id, name);
    glUniformMatrix4fv(location, 1, false, (const GLfloat *)value.raw);
}

void bindShader(Shader *shader)
{
    glUseProgram(shader->id);
}

void unbindShaders()
{
    glUseProgram(0);
}
