#include <stdio.h>
#include <stdlib.h>
#include "lists.h"

Node *list_new(){
    return NULL;
}

void list_insert(Node *current, void *content) {

    // current is pointing to first element
    // we iterate until we find the end
    while(current->next != NULL) {
        current = current->next;
    }
    // create a new Node and list_insert the item
    current->next = (Node *)malloc(sizeof(Node));
    (current->next)->prev = current;
    current = current->next;
    current->content = content;
    current->next = NULL;
}

void list_delete(Node *current, void *content){

    // Iterate until we find a pointer next to the one we need to list_delete
    while (current->next != NULL && (current->next)->content != content) {
        current = current->next;
    }

    // Item is not found
    if(current->next == NULL) {
        return;
    }

    // The element is found in the node next to the one that current points to
    // We removed the node which is next to the pointer (which is also temp)
    Node *tmp = current->next;
    // In special case that we are deleting last node
    if(tmp->next == NULL) {
        current->next = NULL;
    } else {
        current->next = tmp->next;
        (current->next)->prev = tmp->prev;
    }
    tmp->prev = current;

    // We got rid of the node, now time to dellocate the memory
    free(tmp);

    return;
}

void list_each(Node *current, void (*fun)(void *)) {
    while(current != NULL) {
        if (current->content != NULL)
            fun(current->content);
        current = current->next;
    }
}

void list_each_extra(Node *current, void (*fun)(void *, void *), void *extra_arg) {
    while(current != NULL) {
        if (current->content != NULL)
           fun(current->content,extra_arg);
        current = current->next;
    }
}

int list_find(Node *current, void *content) {
    // First pointer is head aka dummy node with no data
    // so we go to next one
    current = current->next;

    // Iterate through the linked list
    while(current != NULL) {
        if(current->content == content) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

