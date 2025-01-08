#include <test_scene.h>
#include <megaman/megaman.h>
#include <enemies/metal/metal.h>
#include <sizes.h>
#include <game.h>
#include <graphics_api.h>
#include <audio_api.h>
#include <enemies/spawner.h>
#include <allocator.h>

Scene *getTestScene()
{
    TilemapColliderMask mask;
    TilemapData *tilemap = loadTilemapFromFiles("../assets/scenes/NitroMan.tsj",
                                                "../assets/scenes/NitroMan.tmj",
                                                &mask);

    Megaman *megaman = newMegaman((vec2s){0.0f, -64.0f});
    Scene *scene = newScene(tilemap, megaman);

    generateColliders(scene, &mask, tilemap->width, tilemap->height);
    addObjectToScene(scene, AS_ENTITY_PTR(megaman));
    addSpawnerToScene(scene, newEnemySpawner((SpawnFn)newMetal, (vec2s){16.0f, 7.0f}, (vec2s){14.0f, 15.0f}));
    addSpawnerToScene(scene, newEnemySpawner((SpawnFn)newMetal, (vec2s){-32.0f, 55.0f}, (vec2s){14.0f, 15.0f}));

    AudioAPI *audio = (AudioAPI *)getGameInstanceService(SERVICE_TYPE_AUDIO);
    playAudioOGG(audio, "../assets/audio/music/001.ogg", AUDIO_NITRO_MAN);

    return scene;
}

Tile newBrick(vec2s position, int id)
{
    Tile tile = getTile(id, position);
    return tile;
}

void onUpdateBrick(void *self, float dt)
{
}

void onCollisionBrick(void *self, AABBColisionData data)
{
}
