#include <service.h>
#include <allocator.h>

void initServiceSet(ServiceSet *array)
{
    array->services = ALLOCATE(Service *, SERVICE_TYPE_MAX);
}

void setServiceFromArray(ServiceSet *array, Service *service)
{
    array->services[service->type] = service;
}
