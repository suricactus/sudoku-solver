#pragma once
#include "solver.h"

typedef struct Node {
    int value;
    struct Node * next;
} Node;

Node * push_right(Node * head, int value)
{
    head->next = malloc(sizeof(Node));
    head->next->value = value;

    return head->next;
}

int list_len(Node * node)
{
    int len = 0;

    while (node)
    {
        len++;
        node = node->next;
    }

    return len;
}
