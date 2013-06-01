#include "../include/level_list.h"

void add_to_level_list(char name[255], level_list *list) {
  level_list_mem *mem = (level_list_mem*)malloc(sizeof(level_list_mem));
  strncpy(mem->name, name, 255);
  if(list->first==NULL) {
    list->first = mem;
    list->last = mem;
    list->first->prev = list->first->next = mem;
  }else{
    list->first = mem;
    mem->next = list->last->next;
    mem->prev = list->last;
    mem->next->prev = mem;
    list->last->next = mem;
  }
}

void free_level_list(level_list *list) {
  level_list_mem *mem = list->first;
  while(mem->next!=list->last) {
    mem=mem->next;
    free(mem->prev);
  }
  free(mem->next);
  free(mem);
}

level_list *get_level_list() {
  level_list *list = (level_list*) malloc(sizeof(level_list));
  list->first=NULL;
  list->last=NULL;

  FILE *f = fopen("res/levels/levels","r");
  int n, i;
  fscanf(f, "%d", &n);
  for(i=0;i<n;i++) {
    char name[255];
    fscanf(f, "%s", name);
    add_to_level_list(name, list);
  }
  fclose(f);
  return list;
}

