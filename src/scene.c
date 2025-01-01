#include <scene.h>
#include <allocator.h>

Scene *newScene()
{
    Scene *scene = ALLOCATE(Scene, 1);
    scene->entities = ALLOCATE(Entity *, ARRAY_MIN_SIZE);
    scene->entityCapacity = ARRAY_MIN_SIZE;
    scene->entityCount = 0;
}