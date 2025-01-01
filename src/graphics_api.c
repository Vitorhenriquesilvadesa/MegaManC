#include <allocator.h>
#include <graphics_api.h>
#include <game.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

bool isWindowClosed(void *self)
{
    CAST_API(GraphicsAPI, self);
    return glfwWindowShouldClose(api->window->nativeWindow);
}

GraphicsAPI *newGraphicsAPI(vec2s size, const char *title, vec4s clearColor)
{
    GraphicsAPI *api = ALLOCATE(GraphicsAPI, 1);
    Window *window = ALLOCATE(Window, 1);

    initWindow(window, size, title, clearColor);

    api->service.type = SERVICE_TYPE_GRAPHICS;
    api->service.init = initGraphicsAPI;
    api->service.update = updateGraphicsAPI;
    api->service.shutdown = shutdownGraphicsAPI;
    api->window = window;
    api->isWindowClosed = isWindowClosed;

    return api;
}

void initGraphicsAPI(void *self)
{
    CAST_API(GraphicsAPI, self);

    glfwInit();

    Window *window = api->window;
    api->window->nativeWindow = glfwCreateWindow(window->size.x, window->size.y, window->title, NULL, NULL);
    glfwMakeContextCurrent(api->window->nativeWindow);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    Renderer2D *renderer = newRenderer();
    api->renderer = renderer;
}

void updateGraphicsAPI(void *self)
{
    CAST_API(GraphicsAPI, self);

    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(api->renderer->shader->id);
    glBindVertexArray(api->renderer->mesh->id);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(api->window->nativeWindow);
    glfwPollEvents();
}

void shutdownGraphicsAPI(void *self)
{
    CAST_API(GraphicsAPI, self);

    glfwDestroyWindow(api->window->nativeWindow);
    glfwTerminate();
}

void setupCallbacks(GraphicsAPI *api)
{
    glfwSetFramebufferSizeCallback(api->window->nativeWindow, onWindowResize);
}
