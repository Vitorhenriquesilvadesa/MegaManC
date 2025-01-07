#ifndef AUDIO_API_H
#define AUDIO_API_H

#include <service.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <list.h>

typedef struct __attribute__((packed))
{
    unsigned RiffChunk;
    unsigned ChunkSize;
    unsigned FileFormat;
    unsigned FormatChunk;
    unsigned FormatSize;
    unsigned short PcmFlags;
    unsigned short Channels;
    unsigned SampleRate;
    unsigned ByteRate;
    unsigned short BlockAlign;
    unsigned short BitDepth;
    unsigned DataChunk;
    unsigned DataSize;
} WavFileHeader;

typedef enum
{
    AUDIO_NITRO_MAN,
    AUDIO_SOLAR_MAN,
    AUDIO_DINK,
    AUDIO_SMALL_EXPLOSION,
    AUDIO_MEGA_BUSTER,
    AUDIO_NAME_MAX,
} AudioName;

typedef struct
{
    bool isLoop;
    ALuint buffer;
    ALuint source;
    AudioName name;
} Audio;

typedef struct
{
    bool isLoaded;
    Audio audio;
} AudioCache;

typedef struct
{
    Service service;
    ALCdevice *device;
    ALCcontext *context;
    LinkedList *audios;
    AudioCache musics[AUDIO_NAME_MAX];
} AudioAPI;

typedef enum
{
    AUDIO_FORMAT_WAV,
    AUDIO_FORMAT_OGG,
} AudioFormat;

typedef struct
{
    AudioAPI *audio;
    const char *filepath;
    AudioFormat format;
    AudioName music;
} AudioPlayTask;

AudioAPI *newAudioAPI();

bool isAudioPlaying(void *context);
void initAudioAPI(void *self);
void updateAudioAPI(void *self, float dt);
void shutdownAudioAPI(void *self);
void playAudioWAV(AudioAPI *api, const char *filepath, AudioName music);
void playAudioOGG(AudioAPI *api, const char *filepath, AudioName music);

void playAudioOGGAsyncWrapper(AudioAPI *api, const char *filepath, AudioName music);

#endif