#include <trigger.h>
#include <allocator.h>
#include <stdio.h>

TriggerAPI *newTriggerAPI()
{
    TriggerAPI *api = ALLOCATE(TriggerAPI, 1);

    api->service.type = SERVICE_TYPE_EVENT;
    api->service.init = initTriggerAPI;
    api->service.update = updateTriggerAPI;
    api->service.shutdown = shutdownTriggerAPI;
    api->triggers = ALLOCATE(Trigger *, ARRAY_MIN_SIZE);
    api->capacity = ARRAY_MIN_SIZE;
    api->triggerCount = 0;

    return api;
}

void initTriggerAPI(void *self)
{
}

void updateTriggerAPI(void *self, float dt)
{
    TriggerAPI *api = (TriggerAPI *)self;

    for (size_t i = 0; i < api->triggerCount; ++i)
    {
        Trigger *trigger = api->triggers[i];

        if (trigger->status == TRIGGER_STATUS_WAITING && trigger->condition(trigger->context))
        {
            trigger->action(trigger->context);
            trigger->status = TRIGGER_STATUS_PERFORMED;
        }
        else if (trigger->status == TRIGGER_STATUS_PERFORMED && !trigger->condition(trigger->context))
        {
            trigger->status = TRIGGER_STATUS_WAITING;
        }
    }
}

void shutdownTriggerAPI(void *self)
{
}

void registerTrigger(TriggerAPI *api, TriggerCondition condition, TriggerCallback action, void *context)
{
    Trigger *trigger = ALLOCATE(Trigger, 1);
    trigger->condition = condition;
    trigger->action = action;
    trigger->context = context;

    if (api->triggerCount >= api->capacity - 1)
    {
        size_t newCapacity = api->capacity * ARRAY_GROW_FACTOR;
        api->triggers = REALLOCATE(Trigger *, api->triggers, newCapacity);
        api->capacity = newCapacity;
    }

    api->triggers[api->triggerCount++] = trigger;
}