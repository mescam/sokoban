#ifndef LEVEL_H

#define LEVEL_H
#include "al_defs.h"
typedef struct _level {
  int w,h;
  char **map;
} level;

level *read_level(char *name);
void free_level(level *lvl); 
bool play_level(al_defs *al, level *lvl, char *name);
#endif /* end of include guard: LEVEL_H */
