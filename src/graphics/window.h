#ifndef WINDOW_H
#define WINDOW_H

#define CGLM_USE_STRUCT
#include <cglm/struct.h>

typedef struct GLFWwindow GLFWwindow;

typedef struct
{
    vec2s size;
    const char *title;
    vec4s clearColor;
    GLFWwindow *nativeWindow;
} Window;

void initWindow(Window *window, vec2s size, const char *title, vec4s clearColor);
void onWindowResize(GLFWwindow *window, int width, int height);

void onWindowFocus(GLFWwindow *window, int focused);

#endif