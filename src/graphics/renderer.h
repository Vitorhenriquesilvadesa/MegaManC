#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#define CGLM_USE_STRUCT
#include <cglm/struct.h>

#include <shader.h>
#include <mesh.h>

typedef struct
{
    vec2s position;
    float zoom;
} Camera2D;

typedef struct
{
    Mesh *mesh;
    Shader *shader;
    Camera2D camera;
} Renderer2D;

Renderer2D *newRenderer();

#endif