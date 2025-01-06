#include <test_scene.h>
#include <megaman/megaman.h>
#include <sizes.h>
#include <game.h>
#include <graphics_api.h>
#include <audio_api.h>
#include <allocator.h>

Scene *getTestScene()
{
    TilemapColliderMask mask;
    TilemapData *tilemap = loadTilemapFromFiles("../assets/scenes/Tilemap001.tsj",
                                                "../assets/scenes/scene001.tmj",
                                                &mask);

    Scene *scene = newScene(tilemap);

    generateColliders(scene, &mask, tilemap->width, tilemap->height);

    addObjectToScene(scene, AS_ENTITY_PTR(newMegaman((vec2s){0.0f, -64.0f})));

    AudioAPI *audio = (AudioAPI *)getGameInstanceService(SERVICE_TYPE_AUDIO);
    playAudioOGGAsyncWrapper(audio, "../assets/audio/music/001.ogg");

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
