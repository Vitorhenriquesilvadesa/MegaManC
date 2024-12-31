#ifndef GRAPHICS_API_H
#define GRAPHICS_API_H

#include <service.h>
#include <stdbool.h>
#include <flags.h>

typedef struct GLFWwindow GLFWwindow;

typedef struct
{
    Service service;
    FlagFunction isWindowClosed;
    GLFWwindow *window;
    int width;
    int height;
    const char *title;
} GraphicsAPI;

bool isWindowClosed(void *self);
GraphicsAPI *newGraphicsAPI(int width, int height, const char *title);
void initGraphicsAPI(void *self);
void updateGraphicsAPI(void *self);
void shutdownGraphicsAPI(void *self);

#endif