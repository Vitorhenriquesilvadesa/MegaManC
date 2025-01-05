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

    Shader *tilemapShader = newShader("../assets/shaders/tilemap.vert",
                                      "../assets/shaders/tilemap.frag");

    api->shaders[SHADER_TYPE_SPRITE] = spriteShader;
    api->shaders[SHADER_TYPE_TILEMAP] = tilemapShader;
}

void updateGraphicsAPI(void *self, float dt)
{
    CAST_API(GraphicsAPI, self);

    static int elapsedTime = 0;
    static float *values = NULL;
    static int fpsCap = 0;

    if (!values)
    {
        fpsCap = (int)getGameInstanceFPS();
        values = ALLOCATE(float, fpsCap);
    }

    values[elapsedTime % fpsCap] = 1.0f / dt;
    elapsedTime++;

    if (elapsedTime >= fpsCap)
    {
        elapsedTime = 0;
        float avg = 0.0f;

        for (int i = 0; i < fpsCap; i++)
        {
            avg += values[i];
        }

        avg /= fpsCap;

        char buffer[64];
        sprintf(buffer, "Mega Man | FPS: %d", (int)avg);

        glfwSetWindowTitle(api->window->nativeWindow, buffer);
    }

    glClear(GL_COLOR_BUFFER_BIT);
    render(api->renderer, getGameInstanceActiveScene());
    glfwSwapBuffers(api->window->nativeWindow);
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
    glfwSetWindowFocusCallback(api->window->nativeWindow, onWindowFocus);
}

Shader *getShader(GraphicsAPI *graphics, ShaderType type)
{
    return graphics->shaders[type];
}
