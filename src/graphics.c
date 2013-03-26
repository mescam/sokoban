#include <allegro5/allegro.h>
#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h> 
#include "../include/al_defs.h"
#include "../include/graphics.h"
//TODO: clean it.

bool initialize_allegro(al_defs* al){
  if(!al_init()){
    fprintf(stderr, "[Sokoban] Failed to initialize graphics. \n");
    return 0;
  }
  
  al->conf = al_load_config_file("res/config.ini");
  if(!al->conf){
    al->conf = al_create_config();
    al_save_config_file("res/config.ini", al->conf);
  }

  al->width = atoi(al_get_config_value(al->conf, "window", "width"));
  al->height = atoi(al_get_config_value(al->conf, "window", "height"));
  
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

  al_init_font_addon();
  if(!al_init_ttf_addon()){
    fprintf(stderr, "[Sokoban] Cannot initialize fonts.");
    return 0;
  }

  load_resources(al);

  return 1;
}

void shutdown_allegro(al_defs* al){
  unload_resources(al);
  al_destroy_display(al->disp);
  al_destroy_event_queue(al->queue);
  al_shutdown_image_addon();
  al_shutdown_font_addon();
  al_shutdown_ttf_addon();
  al_uninstall_keyboard();
}

void load_resources(al_defs* al){
  al->queue = al_create_event_queue();
  al->logo_font = al_load_font("res/fonts/mc.ttf",32,ALLEGRO_ALIGN_CENTRE);
  al->menu_font = al_load_font("res/fonts/mc.ttf",21,ALLEGRO_ALIGN_CENTRE); 
  al_register_event_source(al->queue, al_get_keyboard_event_source());
}

void unload_resources(al_defs* al){
  al_destroy_font(al->menu_font);
  al_destroy_font(al->logo_font);
  al_save_config_file("res/config.ini", al->conf);
  al_destroy_config(al->conf);
}
