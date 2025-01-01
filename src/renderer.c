#include <renderer.h>
#include <allocator.h>
#include <mesh.h>

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

mat4s entityGetTransformationMatrix(Entity *sprite)
{
    vec3s position = {sprite->position.x, sprite->position.y, 0.0f};
    vec3s scale = {sprite->scale.x, sprite->scale.y, 1.0};

    mat4s transformation = GLMS_MAT4_IDENTITY_INIT;
    transformation = glms_translate(transformation, position);
    transformation = glms_scale(transformation, scale);

    return transformation;
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

    Shader *shader = newShader("C:/Github/CDev/MegaMan/assets/shaders/default.vert",
                               "C:/Github/CDev/MegaMan/assets/shaders/default.frag");

    Texture *texture = newTextureFromImage("../assets/sprites/megaman/idle.png");

    Entity *spriteRenderer = newSpriteRenderer((vec2s){0.0f, 0.0f}, (vec2s){256.0f, 64.0f}, shader, texture);

    const float HALF_VIEWPORT_WIDTH = 256.0f / 2.0f;
    const float HALF_VIEWPORT_HEIGHT = 224.0f / 2.0f;

    OrthographicFrustum2D frustum = {
        -HALF_VIEWPORT_WIDTH, HALF_VIEWPORT_WIDTH,
        -HALF_VIEWPORT_HEIGHT, HALF_VIEWPORT_HEIGHT};

    Camera2D *camera = newCamera(frustum);

    renderer->camera = camera;
    renderer->renderers = spriteRenderer;

    return renderer;
}

Entity *newSpriteRenderer(vec2s position, vec2s scale, Shader *shader, Texture *texture)
{
    Entity *renderer = ALLOCATE(Entity, 1);

    renderer->position = position;
    renderer->scale = scale;
    renderer->shader = shader;
    renderer->texture = texture;

    return renderer;
}

void render(Renderer2D *renderer)
{
    for (uint32_t i = 0; i < 1; i++)
    {
        renderSprite(&renderer->renderers[i], renderer->camera);
    }
}

void renderSprite(Entity *sprite, Camera2D *camera)
{
    glActiveTexture(GL_TEXTURE0);
    bindTexture(sprite->texture);
    bindShader(sprite->shader);
    shaderSetMat4(sprite->shader, "projection", cameraGetProjectionMatrix(camera));
    shaderSetMat4(sprite->shader, "view", entityGetTransformationMatrix(sprite));
    shaderSetMat4(sprite->shader, "model", cameraGetViewMatrix(camera));

    shaderSetInt(sprite->shader, "albedo", 0);
    bindMesh(quad);
    drawMesh(quad);
}
