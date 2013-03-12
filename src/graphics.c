#include "../include/al_defs.h"
#include <allegro5/allegro.h>
#include <stdio.h>
#include <stdlib.h>

bool initialize_allegro(al_defs* al){
  if(!al_init()){
    fprintf(stderr, "[Sokoban] Failed to initialize graphics. \n");
    return 0;
  }
  
  al->disp = al_create_display(al->width, al->height);
  if(!al->disp){
    fprintf(stderr, "[Sokoban] Failed to create display. \n");
    return 0;
  }

  if(!al_init_image_addon()){
    fprintf(stderr, "[Sokoban] Cannot initialize image addon. \n");
    return 0;
  }

  if(!al_install_keyboard()){
    fprintf(stderr, "[Sokoban] Cannot install keyboard. \n");
    return 0;
  }

  if(!al_init_font_addon() || !al_init_ttf_addon()){
    fprintf(stderr, "[Sokoban] Cannot initialize fonts.");
    return 0;
  }

  return 1;
}

void shutdown_allegro(al_defs* al){
  al_destroy_display(al->disp);
  al_shutdown_image_addon();
  al_shutdown_font_addon();
  al_shutdown_ttf_addon();
  al_uninstall_keyboard();
}
