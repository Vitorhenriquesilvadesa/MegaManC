#include <window.h>
#include <glad/glad.h>

void initWindow(Window *window, vec2s size, const char *title, vec4s clearColor)
{
    window->clearColor = clearColor;
    window->size = size;
    window->title = title;
}

void onWindowResize(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}
