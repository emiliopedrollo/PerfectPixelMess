//
// Created by Emilio B. Pedrollo on 29/03/2017.
//

#include <stdbool.h>

#ifndef PERFECTPIXELMESS_LISTS_H

typedef struct Node {
    void *content;
    struct Node *next;
    struct Node *prev;
} Node;

Node *list_new(void);
Node *list_first(Node *current);
Node *list_last(Node *current);
Node *list_insert(Node *current, void *content);
Node *list_find(Node *current, void *content);
bool list_delete(Node **current, void *content);
void list_each(Node *current, void (*fun)(void *));
void list_each_extra(Node *current, void (*fun)(void *, void *), void *extra_arg);

#define PERFECTPIXELMESS_LISTS_H

#endif //PERFECTPIXELMESS_LISTS_H
