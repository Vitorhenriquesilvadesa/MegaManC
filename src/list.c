#include <list.h>
#include <stdlib.h>
#include <stdio.h>
#include <allocator.h>

LinkedList *newLinkedList()
{
    LinkedList *list = ALLOCATE(LinkedList, 1);
    list->length = 0;
    list->head = NULL;
    return list;
}

void appendLinkedList(LinkedList *list, void *data)
{
    if (list == NULL)
        return;

    Node *newNode = ALLOCATE(Node, 1);
    newNode->data = data;
    newNode->next = NULL;

    if (list->head == NULL)
    {
        list->head = newNode;
    }
    else
    {
        Node *current = list->head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = newNode;
    }

    list->length++;
}

void removeFromList(LinkedList *list, int index)
{
    if (list == NULL || index < 0 || index >= list->length)
        return;

    Node *current = list->head;
    Node *previous = NULL;

    if (index == 0)
    {
        list->head = current->next;
        free(current);
    }
    else
    {
        for (int i = 0; i < index; i++)
        {
            previous = current;
            current = current->next;
        }
        previous->next = current->next;
        free(current);
    }

    list->length--;
}

void removeUsingPredicate(LinkedList *list, Predicate predicate)
{
    if (list == NULL || predicate == NULL)
        return;

    Node *current = list->head;
    Node *previous = NULL;

    while (current != NULL)
    {
        if (predicate(current->data))
        {
            if (previous == NULL)
            {
                list->head = current->next;
            }
            else
            {
                previous->next = current->next;
            }
            Node *temp = current;
            current = current->next;
            free(temp);
            list->length--;
        }
        else
        {
            previous = current;
            current = current->next;
        }
    }
}
