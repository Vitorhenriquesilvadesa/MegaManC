#ifndef TRIGGER_H
#define TRIGGER_H

#include <stdint.h>
#include <stdbool.h>
#include <service.h>
#include <entity.h>

typedef enum
{
    TRIGGER_STATUS_WAITING,
    TRIGGER_STATUS_PERFORMED,
} TriggerStatus;

typedef bool (*TriggerCondition)(void *context);
typedef void (*TriggerCallback)(void *context);

typedef struct
{
    TriggerCondition condition;
    TriggerCallback action;
    TriggerStatus status;
    Id id;
    void *context;
} Trigger;

typedef struct
{
    Service service;
    Trigger **triggers;
    size_t triggerCount;
    size_t capacity;
} TriggerAPI;

TriggerAPI *newTriggerAPI();

void initTriggerAPI(void *self);
void updateTriggerAPI(void *self, float dt);
void shutdownTriggerAPI(void *self);

void deleteTriggerById(TriggerAPI *api, Id id);
void registerTrigger(TriggerAPI *api, Id id, TriggerCondition condition, TriggerCallback action, void *context);

#endif