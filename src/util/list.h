#ifndef LIST_H
#define LIST_H

#include <stdbool.h>

typedef struct
{
    void *data;
    Node *next;
} Node;

typedef struct
{
    size_t length;
    Node *head;
} LinkedList;

typedef bool (*Predicate)(void *data);

LinkedList *newLinkedList();
void appendLinkedList(LinkedList *list, void *data);
void removeFromList(LinkedList *list, int index);
void removeUsingPreditace(LinkedList *list, Predicate predicate);

#endif