#ifndef AL_DEFS_H
#define AL_DEFS_H
#include <allegro5/allegro.h>

typedef struct _al_defs{
  int width; //display width
  int height; //display height
  ALLEGRO_DISPLAY* disp;
} al_defs;

#endif /* end of include guard: AL_DEFS_H */
