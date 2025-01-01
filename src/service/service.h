#ifndef SERVICE_H
#define SERVICE_H

#include <stdint.h>

typedef void (*initFn)(void *self);
typedef void (*updateFn)(void *self, float dt);
typedef void (*shutdownFn)(void *self);
#define AS_SERVICE_PTR(value) (Service *)value
#define CAST_API(type, value) type *api = (type *)value

typedef enum
{
    SERVICE_TYPE_GRAPHICS,
    SERVICE_TYPE_OBJECT_POOL,
    SERVICE_TYPE_MAX,
} ServiceType;

typedef struct
{
    ServiceType type;
    initFn init;
    updateFn update;
    shutdownFn shutdown;
} Service;

typedef struct
{
    Service **services;
} ServiceSet;

void initServiceSet(ServiceSet *set);
void setServiceFromArray(ServiceSet *set, Service *service);

#endif