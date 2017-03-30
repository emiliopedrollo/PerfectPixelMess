//
// Created by Emilio B. Pedrollo on 29/03/2017.
//

#include "main.h"

#ifndef PERFECTPIXELMESS_LISTS_H

typedef struct FilterNode {
    FilterDef *filter;
    struct FilterNode *next;
    struct FilterNode *prev;
} FilterNode;

FilterNode *list_new();
void list_insert(FilterNode *current, FilterDef *filter);
void list_delete(FilterNode *current, FilterDef *filter);
void list_print(FilterNode *current);
void list_rewind(FilterNode *current);
int list_find(FilterNode *current, FilterDef *filter);

#define PERFECTPIXELMESS_LISTS_H

#endif //PERFECTPIXELMESS_LISTS_H
