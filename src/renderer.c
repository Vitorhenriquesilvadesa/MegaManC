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

    renderer->lines = ALLOCATE(Line, 32);
    renderer->lineCount = 0;
    renderer->lineCapacity = 32;

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
    Entity **entities = scene->entities;
    Entity **tilemap = scene->tilemap;

    for (uint32_t i = 0; i < scene->brickCount; i++)
    {
        if (isEntityOnScreen(tilemap[i], renderer->camera))
        {
            renderEntity(tilemap[i], renderer->camera);
        }
        // renderWireframe(entities[i], renderer->camera);
        // renderCollider(entities[i], renderer->camera);
    }

    for (uint32_t i = 0; i < scene->entityCount; i++)
    {
        renderCollider(entities[i], renderer->camera);

        if (!entities[i]->isVisible)
        {
            continue;
        }

        if (isEntityOnScreen(entities[i], renderer->camera))
        {
            renderEntity(entities[i], renderer->camera);
        }

        // renderWireframe(entities[i], renderer->camera);
    }

    for (size_t i = 0; i < renderer->lineCount; i++)
    {
        Line line = renderer->lines[i];
        renderLine(renderer, line);
    }

    renderer->lineCount = 0;
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

void renderCollider(Entity *entity, Camera2D *camera)
{
    static Shader *colliderShader = NULL;

    if (!colliderShader)
    {
        colliderShader = newShader("../assets/shaders/collider.vert", "../assets/shaders/collider.frag");
    }

    bindShader(colliderShader);

    mat4s transformation = GLMS_MAT4_IDENTITY_INIT;

    vec2s pos = entity->transform.position;
    vec2s min = entity->collider.bound.min;
    vec2s max = entity->collider.bound.max;

    transformation = glms_translate(transformation, (vec3s){pos.x, pos.y, 0.0f});
    transformation = glms_scale(transformation, (vec3s){max.x - min.x, max.y - min.y, 0.0f});

    shaderSetMat4(colliderShader, "projection", cameraGetProjectionMatrix(camera));
    shaderSetMat4(colliderShader, "view", cameraGetViewMatrix(camera));
    shaderSetMat4(colliderShader, "model", transformation);

    bindMesh(quad);
    drawMeshWireframe(quad);
}

void renderLine(Renderer2D *renderer, Line line)
{
    static Shader *lineShader = NULL;

    if (!lineShader)
    {
        lineShader = newShader("../assets/shaders/line.vert", "../assets/shaders/line.frag");
    }

    bindShader(lineShader);

    shaderSetMat4(lineShader, "projection", cameraGetProjectionMatrix(renderer->camera));
    shaderSetMat4(lineShader, "view", cameraGetViewMatrix(renderer->camera));

    vec2s finalPos = (vec2s){(line.start.x + line.end.x) / 2.0, (line.start.y + line.end.y) / 2.0};

    float angle = atan2(line.end.y - line.start.y, line.end.x - line.start.x);

    mat4s transformation = GLMS_MAT4_IDENTITY_INIT;

    transformation = glms_translate(transformation, (vec3s){finalPos.x, finalPos.y, 0.0f});

    transformation = glms_rotate_z(transformation, angle);

    float length = glms_vec2_distance(line.start, line.end);
    transformation = glms_scale(transformation, (vec3s){length, 1.0f, 1.0f});

    shaderSetMat4(lineShader, "model", transformation);

    bindMesh(quad);
    drawMesh(quad);

    unbindTextures();
    unbindMeshes();
    unbindShaders();
}

void drawLine(Renderer2D *renderer, Ray ray, float distance)
{
    if (renderer->lineCount >= renderer->lineCapacity - 1)
    {
        renderer->lines = REALLOCATE(Line, renderer->lines, renderer->lineCapacity * 2);
    }

    vec2s direction = glms_vec2_normalize(ray.direction);
    vec2s end = (vec2s){ray.origin.x + direction.x * distance, ray.origin.y + direction.y * distance};
    renderer->lines[renderer->lineCount++] = (Line){.start = ray.origin, .end = end};
}
