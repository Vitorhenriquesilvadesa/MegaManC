#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include <stdint.h>

typedef struct Node
{
    void *data;
    struct Node *next;
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
void removeUsingPredicate(LinkedList *list, Predicate predicate);

#endif