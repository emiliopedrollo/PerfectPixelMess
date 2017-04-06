#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lists.h"

Node *list_first(Node *current){
    while(current != NULL) {
        if(current->prev == NULL) {
            return current;
        }
        current = current->prev;
    }
    return NULL;
}

Node *list_last(Node *current){
    while(current != NULL) {
        if(current->next == NULL) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

Node *list_new(void){
    return NULL;
}

Node *list_insert(Node *current, void *content) {

    Node* node;
    node = malloc(sizeof(Node));

    node->content = content;
    node->prev = NULL;
    node->next = NULL;

    if (current != NULL){
        node->prev = current;
        node->next = current->next;
    }

    if (node->next != NULL) (node->next)->prev = node;
    if (node->prev != NULL) (node->prev)->next = node;

    return node;
}



Node *list_find(Node *current, void *content) {
    current = list_first(current);
    while(current != NULL) {
        if(current->content == content) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

bool list_delete(Node **current, void *content){

    Node *aux = (*current);
    Node *aux2 = NULL;

    *current = list_find(*current,content);

    if (*current == NULL) return false;

    if ((*current)->next != NULL){
        ((*current)->next)->prev = (*current)->prev;
    } else {
        aux2 = (*current)->prev;

    }

    if ((*current)->prev != NULL){
        ((*current)->prev)->next = (*current)->next;
    } else {
        aux2 = (*current)->next;
    }

    if (aux == *current){
        free(*current);
        *current = aux2;
    } else {
        free(*current);
        *current = aux;
    }
}

void list_each(Node *current, void (*fun)(void *)) {
    current = list_first(current);
    while(current != NULL) {
        if (current->content != NULL)
            fun(current->content);
        current = current->next;
    }
}

void list_each_extra(Node *current, void (*fun)(void *, void *), void *extra_arg) {
    current = list_first(current);
    while(current != NULL) {
        if (current->content != NULL)
           fun(current->content,extra_arg);
        current = current->next;
    }
}

