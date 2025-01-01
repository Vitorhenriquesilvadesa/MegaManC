#include <renderer.h>
#include <allocator.h>
#include <mesh.h>
#include <sprite.h>

static Mesh *quad = NULL;

mat4s cameraGetProjectionMatrix(Camera2D *camera)
{
    OrthographicFrustum2D frustum = camera->frustum;
    mat4s projectionMatrix = glms_ortho(frustum.left, frustum.right, frustum.bottom, frustum.top, -1.0f, 1.0f);

    return projectionMatrix;
}

mat4s cameraGetViewMatrix(Camera2D *camera)
{
    mat4s view = GLMS_MAT4_IDENTITY_INIT;
    vec3s negatedPosition = {-camera->position.x, -camera->position.y};

    view = glms_translate(view, negatedPosition);
    return view;
}

Camera2D *newCamera(OrthographicFrustum2D frustum)
{
    Camera2D *camera = ALLOCATE(Camera2D, 1);

    camera->frustum = frustum;
    camera->position = (vec2s){0.0f, 0.0f};
    camera->zoom = 100.0f;

    return camera;
}

Renderer2D *newRenderer()
{
    Renderer2D *renderer = ALLOCATE(Renderer2D, 1);

    vec2s vertices[4] = {
        {-0.5f, -0.5f},
        {0.5f, -0.5f},
        {0.5f, 0.5f},
        {-0.5f, 0.5f},
    };

    vec2s texCoords[4] = {
        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f},
    };

    GLuint indices[6] = {
        0,
        1,
        2,
        0,
        2,
        3,
    };

    quad = newMesh(vertices, texCoords, indices, 6);

    const float HALF_VIEWPORT_WIDTH = 256.0f / 2.0f;
    const float HALF_VIEWPORT_HEIGHT = 224.0f / 2.0f;

    OrthographicFrustum2D frustum = {
        -HALF_VIEWPORT_WIDTH, HALF_VIEWPORT_WIDTH,
        -HALF_VIEWPORT_HEIGHT, HALF_VIEWPORT_HEIGHT};

    Camera2D *camera = newCamera(frustum);

    renderer->camera = camera;
    return renderer;
}

void render(Renderer2D *renderer, Scene *scene)
{
    for (uint32_t i = 0; i < scene->entityCount; i++)
    {
        renderEntity(scene->entities[i], renderer->camera);
        renderWireframe(scene->entities[i], renderer->camera);
    }
}

void renderEntity(Entity *entity, Camera2D *camera)
{
    Shader *shader = entity->renderer->shader;

    glActiveTexture(GL_TEXTURE0);
    bindTexture(entity->renderer->currentAnimation->texture);
    bindShader(shader);

    shaderSetInt(shader, "frameCount", entity->renderer->currentAnimation->frameCount);
    shaderSetInt(shader, "currentFrame", entity->renderer->currentAnimation->currentFrame);
    shaderSetMat4(shader, "projection", cameraGetProjectionMatrix(camera));
    shaderSetMat4(shader, "view", cameraGetViewMatrix(camera));
    shaderSetMat4(shader, "model", entityGetTransformationMatrix(entity));
    shaderSetInt(shader, "albedo", 0);

    bindMesh(quad);
    drawMesh(quad);

    unbindTextures();
    unbindMeshes();
    unbindShaders();
}

void renderWireframe(Entity *entity, Camera2D *camera)
{
    static Shader *wireframe = NULL;

    if (!wireframe)
    {
        wireframe = newShader("../assets/shaders/wireframe.vert", "../assets/shaders/wireframe.frag");
    }

    bindShader(wireframe);

    shaderSetMat4(wireframe, "projection", cameraGetProjectionMatrix(camera));
    shaderSetMat4(wireframe, "view", cameraGetViewMatrix(camera));
    shaderSetMat4(wireframe, "model", entityGetTransformationMatrix(entity));

    bindMesh(quad);
    drawMeshWireframe(quad);
}
