//
// Created by Emilio B. Pedrollo on 29/03/2017.
//

#include "main.h"

#ifndef PERFECTPIXELMESS_LISTS_H

typedef struct Node {
    void *content;
    struct Node *next;
    struct Node *prev;
} Node;

Node *list_new();
void list_insert(Node *current, void *content);
void list_delete(Node *current, void *content);
void list_print(Node *current, void (*fun)(void *));
void list_rewind(Node *current);
int list_find(Node *current, void *content);

#define PERFECTPIXELMESS_LISTS_H

#endif //PERFECTPIXELMESS_LISTS_H
