#ifndef LEVEL_H

#define LEVEL_H

typedef struct _level {
  int w,h;
  char **map;
} level;

level *read_level(char *name);
#endif /* end of include guard: LEVEL_H */
