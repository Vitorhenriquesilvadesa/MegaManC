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
    glClearColor(window->clearColor.r, window->clearColor.g, window->clearColor.b, window->clearColor.a);

    setupCallbacks(api);

    int width, height;

    glfwGetWindowSize(api->window->nativeWindow, &width, &height);
    onWindowResize(api->window->nativeWindow, width, height);

    Renderer2D *renderer = newRenderer();
    api->renderer = renderer;

    Shader *spriteShader = newShader("../assets/shaders/default.vert",
                                     "../assets/shaders/default.frag");

    api->shaders[SHADER_TYPE_SPRITE] = spriteShader;
}

void updateGraphicsAPI(void *self, float dt)
{
    CAST_API(GraphicsAPI, self);

    static float elapsedTime = 0.0f;

    elapsedTime += dt;

    if (elapsedTime >= 1.0f)
    {
        elapsedTime = 0.0f;
        char buffer[32];

        sprintf(buffer, "Mega Man | FPS: %d", (int)(1.0f / dt));

        glfwSetWindowTitle(api->window->nativeWindow, buffer);
    }

    glClear(GL_COLOR_BUFFER_BIT);

    render(api->renderer, getGameInstanceActiveScene());

    glfwSwapBuffers(api->window->nativeWindow);
    glfwPollEvents();
}

void shutdownGraphicsAPI(void *self)
{
    CAST_API(GraphicsAPI, self);

    glfwDestroyWindow(api->window->nativeWindow);
    glfwTerminate();

    FREE(api->renderer);
    FREE(api->window);
}

void setupCallbacks(GraphicsAPI *api)
{
    glfwSetFramebufferSizeCallback(api->window->nativeWindow, onWindowResize);
}

Shader *getShader(GraphicsAPI *graphics, ShaderType type)
{
    return graphics->shaders[type];
}
