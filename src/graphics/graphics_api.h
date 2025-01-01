#ifndef GRAPHICS_API_H
#define GRAPHICS_API_H

#include <service.h>
#include <stdbool.h>
#include <flags.h>
#include <window.h>
#include <renderer.h>

typedef struct GLFWwindow GLFWwindow;

typedef struct
{
    Service service;
    FlagFunction isWindowClosed;
    Window *window;
    Renderer2D *renderer;
} GraphicsAPI;

bool isWindowClosed(void *self);
GraphicsAPI *newGraphicsAPI(vec2s size, const char *title, vec4s clearColor);
void initGraphicsAPI(void *self);
void updateGraphicsAPI(void *self, float dt);
void shutdownGraphicsAPI(void *self);
void setupCallbacks(GraphicsAPI *api);

#endif