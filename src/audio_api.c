#include <audio_api.h>
#include <allocator.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

#define STB_VORBIS_HEADER_ONLY
#include <stb_vorbis.c>

AudioAPI *newAudioAPI()
{
    AudioAPI *api = ALLOCATE(AudioAPI, 1);
    api->service.type = SERVICE_TYPE_AUDIO;
    api->service.init = initAudioAPI;
    api->service.update = updateAudioAPI;
    api->service.shutdown = shutdownAudioAPI;

    api->audios = newLinkedList();
    for (int i = 0; i < AUDIO_NAME_MAX; ++i)
    {
        api->musics[i].isLoaded = false;
    }

    return api;
}

bool isAudioPlaying(void *context)
{
    Audio *audio = (Audio *)context;
    ALint state;
    alGetSourcei(audio->source, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}

void initAudioAPI(void *self)
{
    CAST_API(AudioAPI, self);

    api->device = alcOpenDevice(NULL);
    if (!api->device)
    {
        printf("Failed to open audio device.");
        exit(EXIT_FAILURE);
    }

    api->context = alcCreateContext(api->device, NULL);
    if (!api->context)
    {
        printf("Failed to create OpenAL context.");
        exit(EXIT_FAILURE);
    }

    alcMakeContextCurrent(api->context);
}

void updateAudioAPI(void *self, float dt)
{
    CAST_API(AudioAPI, self);

    Node *current = api->audios->head;
    size_t index = 0;
    while (current)
    {
        Audio *audio = (Audio *)current->data;

        ALint state;
        alGetSourcei(audio->source, AL_SOURCE_STATE, &state);

        if (state != AL_PLAYING && !audio->isLoop)
        {
            alDeleteSources(1, &audio->source);
            removeFromList(api->audios, index);
        }

        index++;
        current = current->next;
    }
}

void shutdownAudioAPI(void *self)
{
    CAST_API(AudioAPI, self);
    alcDestroyContext(api->context);
    alcCloseDevice(api->device);
}

void playAudioWAV(AudioAPI *api, const char *filepath, AudioName music)
{
    if (api->musics[music].isLoaded)
    {
        ALuint source;
        alGenSources(1, &source);
        alSourcei(source, AL_BUFFER, api->musics[music].audio.buffer);

        alSourcePlay(source);

        Audio *newAudio = ALLOCATE(Audio, 1);
        newAudio->isLoop = api->musics[music].audio.isLoop;
        newAudio->buffer = api->musics[music].audio.buffer;
        newAudio->source = source;
        newAudio->name = music;
        appendLinkedList(api->audios, newAudio);

        return;
    }

    FILE *file = fopen(filepath, "rb");
    if (!file)
    {
        printf("Failed to open audio file: %s\n", filepath);
        return;
    }

    WavFileHeader wavheader;
    if (fread(&wavheader, sizeof(WavFileHeader), 1, file) != 1)
    {
        printf("Failed to read WAV file header.\n");
        fclose(file);
        return;
    }

    if (wavheader.RiffChunk != 0x46464952 || wavheader.FileFormat != 0x45564157)
    {
        printf("Invalid file format. Expected WAV.\n");
        fclose(file);
        return;
    }

    if (wavheader.PcmFlags != 1 || wavheader.BitDepth != 16)
    {
        printf("Only PCM WAV files with 16-bit samples are supported.\n");
        fclose(file);
        return;
    }

    short *samples = ALLOCATE(short, wavheader.DataSize / sizeof(short));
    if (fread(samples, 1, wavheader.DataSize, file) != wavheader.DataSize)
    {
        printf("Failed to read audio data.\n");
        FREE(samples);
        fclose(file);
        return;
    }

    ALuint buffer;
    alGenBuffers(1, &buffer);
    alBufferData(buffer, wavheader.Channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16, samples, wavheader.DataSize, wavheader.SampleRate);

    ALuint source;
    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, buffer);

    fclose(file);
    FREE(samples);

    api->musics[music].audio.isLoop = false;
    api->musics[music].audio.buffer = buffer;
    api->musics[music].audio.source = source;
    api->musics[music].isLoaded = true;

    Audio *audio = ALLOCATE(Audio, 1);
    audio->isLoop = false;
    audio->buffer = buffer;
    audio->source = source;
    audio->name = music;
    appendLinkedList(api->audios, audio);

    alSourcePlay(source);
}

DWORD WINAPI playAudioOGGAsync(LPVOID param)
{
    AudioPlayTask *task = (AudioPlayTask *)param;
    AudioAPI *api = task->audio;
    const char *filepath = task->filepath;

    FILE *file = fopen(filepath, "rb");
    if (!file)
    {
        fprintf(stderr, "Failed to open OGG file: %s\n", filepath);
        free(task);
        return 0;
    }

    int channels, sampleRate;
    short *output;
    int sampleCount = stb_vorbis_decode_filename(filepath, &channels, &sampleRate, &output);
    if (sampleCount <= 0)
    {
        fprintf(stderr, "Failed to decode OGG file: %s\n", filepath);
        fclose(file);
        free(task);
        return 0;
    }
    fclose(file);

    ALenum format;
    if (channels == 1)
    {
        format = AL_FORMAT_MONO16;
    }
    else if (channels == 2)
    {
        format = AL_FORMAT_STEREO16;
    }
    else
    {
        fprintf(stderr, "Unsupported channel count in OGG file: %d\n", channels);
        FREE(output);
        free(task);
        return 0;
    }

    Audio *audio = ALLOCATE(Audio, 1);
    alGenBuffers(1, &audio->buffer);
    alBufferData(audio->buffer, format, output, sampleCount * sizeof(short), sampleRate);

    FREE(output);

    alGenSources(1, &audio->source);
    alSourcei(audio->source, AL_BUFFER, audio->buffer);
    alSourcei(audio->source, AL_LOOPING, AL_FALSE);

    audio->isLoop = false;

    alSourcePlay(audio->source);

    appendLinkedList(api->audios, audio);

    free(task);
    return 0;
}

void playAudioOGGAsyncWrapper(AudioAPI *api, const char *filepath, AudioName music)
{
    AudioPlayTask *task = malloc(sizeof(AudioPlayTask));
    task->audio = api;
    task->filepath = filepath;
    task->format = AUDIO_FORMAT_OGG;

    CreateThread(NULL, 0, playAudioOGGAsync, task, 0, NULL);
}

void playAudioOGG(AudioAPI *api, const char *filepath, AudioName music)
{
    playAudioOGGAsyncWrapper(api, filepath, music);
}
