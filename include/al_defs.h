#ifndef AL_DEFS_H
#define AL_DEFS_H
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
typedef struct _al_defs{
  int width; //display width
  int height; //display height
  ALLEGRO_DISPLAY *disp;
  ALLEGRO_EVENT_QUEUE *queue;
  ALLEGRO_FONT *logo_font;
  ALLEGRO_FONT *menu_font;
  ALLEGRO_FONT *hint_font;
  ALLEGRO_CONFIG *conf;
} al_defs;


#endif /* end of include guard: AL_DEFS_H */
