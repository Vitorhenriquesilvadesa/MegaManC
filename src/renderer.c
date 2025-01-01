#include <renderer.h>
#include <allocator.h>
#include <mesh.h>

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

    Mesh *mesh = newMesh(vertices, texCoords, indices, 6);

    Shader *shader = newShader("C:/Github/CDev/MegaMan/assets/shaders/default.vert",
                               "C:/Github/CDev/MegaMan/assets/shaders/default.frag");

    renderer->mesh = mesh;
    renderer->shader = shader;

    return renderer;
}
