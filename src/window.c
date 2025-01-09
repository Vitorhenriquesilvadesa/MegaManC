#include <window.h>
#include <glad/glad.h>
#include <sizes.h>
#include <game.h>

void initWindow(Window *window, vec2s size, const char *title, vec4s clearColor)
{
    window->clearColor = clearColor;
    window->size = size;
    window->title = title;
}

void onWindowResize(GLFWwindow *window, int width, int height)
{
    float aspectRatio = (float)SCREEN_WIDTH / SCREEN_HEIGHT;

    int newWidth = width;
    int newHeight = height;

    if (width / (float)height > aspectRatio)
    {
        newWidth = (int)(height * aspectRatio);
    }
    else
    {
        newHeight = (int)(width / aspectRatio);
    }

    glViewport((width - newWidth) / 2, (height - newHeight) / 2, newWidth, newHeight);
}

void onWindowFocus(GLFWwindow *window, int focused)
{
    // setGameEnableUpdates(focused);
}
