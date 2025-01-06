#ifndef AUDIO_API_H
#define AUDIO_API_H

#include <service.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <list.h>

typedef struct
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

typedef struct
{
    bool isLoop;
    ALuint buffer;
    ALuint source;
} Audio;

typedef struct
{
    Service service;
    ALCdevice *device;
    ALCcontext *context;
    LinkedList *audios;
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
} AudioPlayTask;

AudioAPI *newAudioAPI();

void initAudioAPI(void *self);
void updateAudioAPI(void *self, float dt);
void shutdownAudioAPI(void *self);
void playAudioWAV(AudioAPI *api, const char *filepath);
void playAudioOGG(AudioAPI *api, const char *filepath);

void playAudioOGGAsyncWrapper(AudioAPI *api, const char *filepath);

#endif