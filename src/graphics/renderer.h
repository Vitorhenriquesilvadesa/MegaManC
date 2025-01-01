#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#define CGLM_USE_STRUCT
#include <cglm/struct.h>

#include <shader.h>
#include <mesh.h>
#include <texture.h>
#include <entity.h>
#include <scene.h>

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
    Camera2D *camera;
} Renderer2D;

mat4s cameraGetProjectionMatrix(Camera2D *camera);
mat4s cameraGetViewMatrix(Camera2D *camera);

Camera2D *newCamera(OrthographicFrustum2D frustum);
Renderer2D *newRenderer();

void render(Renderer2D *renderer, Scene *scene);
void renderEntity(Entity *entity, Camera2D *camera);

#endif