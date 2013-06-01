#ifndef LEVEL_LIST_H

#define LEVEL_LIST_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _level_list_mem {
  char name[255];
  struct _level_list_mem *prev, *next;
} level_list_mem;

typedef struct _level_list {
  struct _level_list_mem *first, *last;
} level_list;

void add_to_level_list(char name[255], level_list *list);
void free_level_list(level_list *list);
level_list *get_level_list();

#endif /* end of include guard: LEVEL_LIST_H */
