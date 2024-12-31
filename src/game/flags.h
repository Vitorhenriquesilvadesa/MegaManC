#ifndef FLAGS_H
#define FLAGS_H

typedef enum
{
    FLAG_WINDOW_CLOSED,
    FLAG_MAX_COUNT,
} GameFlag;

typedef bool (*FlagFunction)(void *service);

#endif