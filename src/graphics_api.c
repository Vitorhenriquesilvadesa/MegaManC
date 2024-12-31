#include <allocator.h>
#include <graphics_api.h>
#include <game.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

bool isWindowClosed(void *self)
{
    CAST_API(GraphicsAPI, self);
    return glfwWindowShouldClose(api->window);
}

GraphicsAPI *newGraphicsAPI(int width, int height, const char *title)
{
    GraphicsAPI *api = ALLOCATE(GraphicsAPI, 1);
    api->width = width;
    api->height = height;
    api->title = title;
    api->service.type = SERVICE_TYPE_GRAPHICS;
    api->service.init = initGraphicsAPI;
    api->service.update = updateGraphicsAPI;
    api->service.shutdown = shutdownGraphicsAPI;
    api->isWindowClosed = isWindowClosed;

    return api;
}

void initGraphicsAPI(void *self)
{
    CAST_API(GraphicsAPI, self);

    glfwInit();

    api->window = glfwCreateWindow(api->width, api->height, api->title, NULL, NULL);
    glfwMakeContextCurrent(api->window);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void updateGraphicsAPI(void *self)
{
    CAST_API(GraphicsAPI, self);

    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(api->window);
    glfwPollEvents();
}

void shutdownGraphicsAPI(void *self)
{
    CAST_API(GraphicsAPI, self);

    glfwDestroyWindow(api->window);
    glfwTerminate();
}
