#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#define CGLM_USE_STRUCT
#include <cglm/struct.h>

#include <shader.h>
#include <mesh.h>
#include <texture.h>

typedef struct
{
    float left;
    float right;
    float top;
    float bottom;
} OrthographicFrustum2D;

typedef struct
{
    vec2s position;
    float zoom;
    OrthographicFrustum2D frustum;
} Camera2D;

typedef struct
{
    Texture *texture;
    vec2s position;
    vec2s scale;
    Shader *shader;
} Entity;

typedef struct
{
    Camera2D *camera;
    Entity *renderers;
} Renderer2D;

mat4s cameraGetProjectionMatrix(Camera2D *camera);
mat4s cameraGetViewMatrix(Camera2D *camera);
mat4s entityGetTransformationMatrix(Entity *sprite);

Camera2D *newCamera(OrthographicFrustum2D frustum);
Renderer2D *newRenderer();
Entity *newSpriteRenderer(vec2s position, vec2s scale, Shader *shader, Texture *texture);

void render(Renderer2D *renderer);
void renderSprite(Entity *sprite, Camera2D *camera);

#endif