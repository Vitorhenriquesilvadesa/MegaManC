#include <ray_cast.h>
#include <entity.h>
#include <game.h>
#include <object_pool_api.h>
#include <graphics_api.h>
#include <math.h>
#include <float.h>

bool rayIntersectsAABB(vec2s rayOrigin, vec2s rayDir, Entity *entity, float distance)
{
    AABB aabb = entity->collider;

    vec2s aabbMin = {aabb.bound.min.x + entity->transform.position.x, aabb.bound.min.y + entity->transform.position.y};
    vec2s aabbMax = {aabb.bound.max.x + entity->transform.position.x, aabb.bound.max.y + entity->transform.position.y};

    rayDir = glms_vec2_normalize(rayDir);

    float tmin, tmax, tymin, tymax;

    if (rayDir.x != 0.0f)
    {
        tmin = (aabbMin.x - rayOrigin.x) / rayDir.x;
        tmax = (aabbMax.x - rayOrigin.x) / rayDir.x;

        if (tmin > tmax)
        {
            float temp = tmin;
            tmin = tmax;
            tmax = temp;
        }
    }
    else
    {
        tmin = rayOrigin.x < aabbMin.x || rayOrigin.x > aabbMax.x ? FLT_MAX : 0;
        tmax = tmin;
    }

    if (rayDir.y != 0.0f)
    {
        tymin = (aabbMin.y - rayOrigin.y) / rayDir.y;
        tymax = (aabbMax.y - rayOrigin.y) / rayDir.y;

        if (tymin > tymax)
        {
            float temp = tymin;
            tymin = tymax;
            tymax = temp;
        }
    }
    else
    {
        tymin = rayOrigin.y < aabbMin.y || rayOrigin.y > aabbMax.y ? FLT_MAX : 0;
        tymax = tymin;
    }

    if (tmin > tymax || tymin > tmax)
    {
        return false;
    }

    tmin = fmax(tmin, tymin);
    tmax = fmin(tmax, tymax);

    return tmax >= 0 && tmin <= distance;
}

bool raycastHit(Ray ray, float distance)
{
    GraphicsAPI *graphics = (GraphicsAPI *)getGameInstanceService(SERVICE_TYPE_GRAPHICS);
    Renderer2D *renderer = graphics->renderer;

    drawLine(renderer, ray, distance);

    Scene *scene = getGameInstanceActiveScene();

    if (!scene)
    {
        return false;
    }

    uint32_t count = 0;

    for (uint32_t i = 0; i < scene->entityCount; i++)
    {
        Entity *entity = scene->entities[i];
        if (!entity || entity->type != ENTITY_TYPE_BRICK && entity->type != ENTITY_TYPE_COLLIDER)
            continue;

        if (rayIntersectsAABB(ray.origin, ray.direction, entity, distance))
        {
            return true;
        }
    }

    return false;
}
