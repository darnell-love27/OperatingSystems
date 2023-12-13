// list/list.c
// 
// Implementation for linked list.
//
// <Author>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

list_t *list_alloc() { 
  list_t* mylist =  (list_t *)malloc(sizeof(list_t)); 
  if ((mylist->head) == NULL) {
    perror("list_alloc");
    exit(EXIT_FAILURE);
  }
  mylist->head = NULL;
  return mylist;
}

void list_free(list_t *l) {
  if (l == NULL){
    return;
  }

  //Free all nodes in the list
  node_t *current = l->head;
  while (current != NULL) {
    node_t *temp = current;
    current = current->next;
    free(temp);
  }
  //Free the list structure
  free(l);
}

void list_print(list_t *l) {
  if (l == NULL || l->head == NULL) {
    printf("List is empty\n");
    return;
  }

  node_t *current = l->head;
  while (current != NULL) {
    printf("%d -> ", current->value);
    current = current->next;
  }
  printf("NULL\n");
}


char * listToString(list_t *l) {
  char* buf = (char *) malloc(sizeof(char) * 1024);
  if (buf == NULL) {
    perror("listToString");
    exit(EXIT_FAILURE);
  }
  buf[0] = '\0';
  char tbuf[20];

	node_t* curr = l->head;
  while (curr != NULL) {
    sprintf(tbuf, "%d->", curr->value);
    curr = curr->next;
    strcat(buf, tbuf);
  }
  strcat(buf, "NULL");
  return buf;
}

int list_length(list_t *l) { 
  int length = 0;
  if (l == NULL){
    return length;
  }
  node_t *current = l->head;
  while (current != NULL) {
    length++;
    current = current->next;
  }

  return length; 
}

void list_add_to_back(list_t *l, elem value) {
  node_t *curr_node = (node_t *)malloc(sizeof(node_t));
  if (curr_node == NULL) {
    perror("list_add_to_back");
    exit(EXIT_FAILURE);
  }
  curr_node->value = value;
  curr_node->next = NULL;

  if (l==NULL) {
    perror("list_add_to_back");
    exit(EXIT_FAILURE);
  }

  if (l->head == NULL) {
    // If the list is empty, set the new node as the head 
    l->head = curr_node;
  } else {
    node_t *current = l->head;
    while (current->next != NULL) {
      current = current->next;
    }
    current->next = curr_node;
  }
}
void list_add_to_front(list_t *l, elem value) {
   if (l == NULL) {
        // Invalid list
        return;
    }

    // Create a new node
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    if (new_node == NULL) {
        perror("list_add_to_front");
        exit(EXIT_FAILURE);
    }

    new_node->value = value;
    new_node->next = l->head; // Set the new node's next to the current head
    l->head = new_node; // Update the list's head to the new node
}

void list_add_at_index(list_t *l, elem value, int index) {
  if (l == NULL || index < 1) {
        perror("list_add_at_index");
        exit(EXIT_FAILURE);
    }

    // Create a new node with the given value
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    if (new_node == NULL) {
        perror("list_add_at_index");
        exit(EXIT_FAILURE);
    }
    new_node->value = value;

    if (index == 1 || l->head == NULL) {
        // If index is 1 or the list is empty, set the new node as the head
        new_node->next = l->head;
        l->head = new_node;
    } else {
        // Traverse the list to find the node before the target index
        int current_index = 1;
        node_t *current = l->head;
        while (current != NULL && current_index < index - 1) {
            current = current->next;
            current_index++;
        }

        if (current == NULL) {
            // Index is out of bounds, insert at the end
            current = l->head;
            while (current->next != NULL) {
                current = current->next;
            }
        }

        // Insert the new node after the current node
        new_node->next = current->next;
        current->next = new_node;
    }
}

elem list_remove_from_back(list_t *l) { 
  if (l == NULL || l->head == NULL) {
        perror("list_remove_from_back");
        exit(EXIT_FAILURE);
    }

    if (l->head->next == NULL) {
        // If there's only one node in the list
        elem value = l->head->value;
        free(l->head);
        l->head = NULL;
        return value;
    } else {
        // Traverse to the second-to-last node
        node_t *current = l->head;
        while (current->next->next != NULL) {
            current = current->next;
        }

        // Remove the last node and return its value
        elem value = current->next->value;
        free(current->next);
        current->next = NULL;
        return value;
    }
}
elem list_remove_from_front(list_t *l) { 
  if (l == NULL || l->head == NULL) {
        perror("list_remove_from_front");
        exit(EXIT_FAILURE);
    }

    // Remove the first node and return its value
    elem value = l->head->value;
    node_t *temp = l->head;
    l->head = l->head->next;
    free(temp);
    return value;
 }
elem list_remove_at_index(list_t *l, int index) {
  if (l == NULL || l->head == NULL || index < 1) {
        perror("list_remove_at_index");
        exit(EXIT_FAILURE);
    }

    if (index == 1) {
        // Removing the first node
        elem value = l->head->value;
        node_t *temp = l->head;
        l->head = l->head->next;
        free(temp);
        return value;
    } else {
        // Traverse the list to find the node before the target index
        int current_index = 1;
        node_t *current = l->head;
        while (current != NULL && current_index < index - 1) {
            current = current->next;
            current_index++;
        }

        if (current == NULL || current->next == NULL) {
            perror("list_remove_at_index");
            exit(EXIT_FAILURE);
            }
        return -1;   
    }
}

bool list_is_in(list_t *l, elem value) {
  if (l == NULL || l->head == NULL) {
        // Empty list, or list is not allocated
        return false;
    }

    // Traverse the list to check if the value exists
    node_t *current = l->head;
    while (current != NULL) {
        if (current->value == value) {
            // Element found in the list
            return true;
        }
        current = current->next;
    }

    // Element not found in the list
    return false;
 }
elem list_get_elem_at(list_t *l, int index) {
  if (l == NULL || l->head == NULL || index < 1) {
        // Invalid list, empty list, or invalid index
        return -1;
    }

    node_t *current = l->head;
    int current_index = 1;

    // Traverse the list to find the element at the specified index
    while (current != NULL && current_index < index) {
        current = current->next;
        current_index++;
    }

    if (current == NULL) {
        // Index is out of bounds
        return -1;
    }

    // Return the element at the specified index
    return current->value;
 }
int list_get_index_of(list_t *l, elem value) { 
  if (l == NULL || l->head == NULL) {
        // Invalid list or empty list
        return -1;
    }

    node_t *current = l->head;
    int index = 1;

    // Traverse the list to find the index of the element
    while (current != NULL) {
        if (current->value == value) {
            // Element found, return its index (indexing starts at 1)
            return index;
        }
        current = current->next;
        index++;
    }

    // Element not found in the list
    return -1;
 }


