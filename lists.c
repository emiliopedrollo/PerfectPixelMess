#include <stdio.h>
#include <stdlib.h>
#include "lists.h"
#include "main.h"

FilterNode *list_new(){
    FilterNode *list;
    list = malloc(sizeof(FilterNode));
    list->filter = NULL;
    list->next = NULL;
    list->prev = NULL;
    return list;
}

void list_rewind(FilterNode *current) {
    while(current->prev != NULL) {
        current = current->prev;
    }
}

void list_insert(FilterNode *current, FilterDef *filter) {

    // current is pointing to first element
    // we iterate until we find the end
    while(current->next != NULL) {
        current = current->next;
    }
    // create a new Node and list_insert the item
    current->next = (FilterNode *)malloc(sizeof(FilterNode));
    (current->next)->prev = current;
    current = current->next;
    current->filter = filter;
    current->next = NULL;
}

void list_delete(FilterNode *current, FilterDef *filter){

    // Iterate until we find a pointer next to the one we need to list_delete
    while (current->next != NULL && (current->next)->filter != filter) {
        current = current->next;
    }

    // Item is not found
    if(current->next == NULL) {
        return;
    }

    // The element is found in the node next to the one that current points to
    // We removed the node which is next to the pointer (which is also temp)
    FilterNode *tmp = current->next;
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
void list_print(FilterNode *current) {
    int i;
    while(current != NULL) {
        if (current->filter != NULL) {
            printf("Filter: %s\n", current->filter->name);
            if (current->filter->params != NULL) {
                printf("Params: ");
                for (i = 0; *(current->filter->params + i); i++) {
                    printf("%s ", *(current->filter->params + i));
                }
                printf("\n");
            }
            printf("\n");
        }
        current = current->next;
    }
}

int list_find(FilterNode *current, FilterDef *filter) {
    // First pointer is head aka dummy node with no data
    // so we go to next one
    current = current->next;

    // Iterate through the linked list
    while(current != NULL) {
        if(current->filter == filter) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

