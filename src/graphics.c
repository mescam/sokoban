#include <allegro5/allegro.h>
#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h> 
#include <allegro5/allegro_primitives.h>
#include "../include/al_defs.h"
#include "../include/graphics.h"
#include "../include/level.h"
#include "../include/misc.h"
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

void shutdown_allegro(al_defs *al){
  unload_resources(al);
  al_destroy_display(al->disp);
  al_destroy_event_queue(al->queue);
  al_shutdown_image_addon();
  al_shutdown_font_addon();
  al_shutdown_ttf_addon();
  al_uninstall_keyboard();
}

void load_resources(al_defs *al){
  al->queue = al_create_event_queue();
  al->logo_font = al_load_font("res/fonts/mc.ttf",32,ALLEGRO_ALIGN_CENTRE);
  al->menu_font = al_load_font("res/fonts/mc.ttf",21,ALLEGRO_ALIGN_CENTRE); 
  al->hint_font = al_load_font("res/fonts/mc.ttf",12,ALLEGRO_ALIGN_CENTRE);
  al_register_event_source(al->queue, al_get_keyboard_event_source());
}

void unload_resources(al_defs *al){
  al_destroy_font(al->menu_font);
  al_destroy_font(al->logo_font);
  al_destroy_font(al->hint_font);
  al_save_config_file("res/config.ini", al->conf);
  al_destroy_config(al->conf);
}


void draw_tile_border(al_defs *al, int i, int j, level *lvl) {
  int y_offset = 50;
  int wx = 0.8f * al->width;
  int wy = 0.8f * al->height - y_offset;
  int th = wy/lvl->h;
  int tw = wx/lvl->w;
  int t = (th>tw)?tw:th;
  t=(t>40)?40:t;
  int x_offset = (al->width - lvl->w*t)/2;
  int x1=x_offset+(j*t), 
      y1=y_offset+(i*t), 
      x2=x_offset+((j+1)*t), 
      y2=y_offset+((i+1)*t);
  //god will hate me x2
  al_draw_rectangle(x1,y1,x2,y2,al_map_rgb(0,0,255),2.0);
}

void draw_tile(al_defs *al, int i, int j, level* lvl) {
  int y_offset = 50;
  int wx = 0.8f * al->width;
  int wy = 0.8f * al->height - y_offset;
  int th = wy/lvl->h;
  int tw = wx/lvl->w;

  int t = (th>tw)?tw:th;
  t=(t>40)?40:t;
  int x_offset = (al->width - lvl->w*t)/2;
  //god will hate me.
  ALLEGRO_COLOR color;
  switch(lvl->map[i][j]) {
    case '#': 
      color = al_map_rgb(20,20,20);
      break;
    default:
      color = al_map_rgb(0,0,0);
  }

  int x1=x_offset+(j*t), 
      y1=y_offset+(i*t), 
      x2=x_offset+((j+1)*t), 
      y2=y_offset+((i+1)*t);
  
  al_draw_filled_rectangle(x1,y1,x2,y2,color);
  al_draw_rectangle(x1,y1,x2,y2, al_map_rgb(30,30,30), 1.0); //bounding 

  switch(lvl->map[i][j]) {
    case 'P': // player! will be replaced by sprite.
      //al_draw_filled_rectangle(x1+(t/3), y1+(t/3), x2-(t/3), y2-(t/3),
      //    al_map_rgb(255,0,0));
      al_draw_filled_circle(x1+(t/2), y1+(t/2), t/4, al_map_rgb(0,0,120));
      break;

    case 'S': // chest not in correct place
      al_draw_filled_rectangle(x1+(t/4), y1+(t/4), x2-(t/4), y2-(t/4),
          al_map_rgb(255,255,0));
      break;

    case 'X': //destination
      //al_draw_filled_rectangle(x1+(t/3), y1+(t/3), x2-(t/3), y2-(t/3),
      //    al_map_rgb(0,255,255));
      al_draw_line(x1,y1,x2,y2,al_map_rgb(255,0,0),1.0);
      al_draw_line(x1,y1+t,x2,y2-t,al_map_rgb(255,0,0),1.0);
      break;
      
    case 'Z': //chest in correct place 
      al_draw_line(x1,y1,x2,y2,al_map_rgb(0,255,0),1.0);
      al_draw_line(x1,y1+t,x2,y2-t,al_map_rgb(0,255,0),1.0);

      al_draw_filled_rectangle(x1+(t/4), y1+(t/4), x2-(t/4), y2-(t/4),
          al_map_rgb(255,255,0));
      break;

    case 'C': //player on destination point
      al_draw_line(x1,y1,x2,y2,al_map_rgb(255,0,0),1.0);
      al_draw_line(x1,y1+t,x2,y2-t,al_map_rgb(255,0,0),1.0);
      al_draw_filled_circle(x1+(t/2), y1+(t/2), t/4, al_map_rgb(0,0,120));
      break;
  }
  return;
}

void draw_level_map(al_defs* al, level* lvl, const int counter, char *name) {
  al_clear_to_color(al_map_rgb(0,0,0));
  al_draw_text(al->logo_font, al_map_rgb(255,255,255), al->width/2, 5, 
      ALLEGRO_ALIGN_CENTRE, "sokoban");

  int i,j;
  for (i = 0; i < lvl->h; i++) {
    for (j = 0; j < lvl->w; j++) {
      draw_tile(al, i, j, lvl);
    }
  }

  int wy = 0.8f * al->height;

  //al_draw_line(0,wy,640,wy,al_map_rgb(255,0,0),1.0); //debug
  al_draw_textf(al->menu_font, al_map_rgb(255,255,255), 30, wy+10, 
      ALLEGRO_ALIGN_LEFT, "Steps: %d",counter);
  al_draw_textf(al->menu_font, al_map_rgb(255,255,255), al->width-30, 
      wy+10, ALLEGRO_ALIGN_RIGHT, "Map: %s",name);

  al_draw_textf(al->hint_font, al_map_rgb(255,255,255), 10, al->height-20,
      ALLEGRO_ALIGN_LEFT,
      "R - reset    Esc - return to the main menu");
  al_flip_display();
}


#define draw_text_win(X,Z) al_draw_textf(al->menu_font, al_map_rgb(255,255,255), al->width/2, 150+(X*32), ALLEGRO_ALIGN_CENTRE, text[X], Z);
void display_win_board(al_defs *al, const int counter, char *name) {
  al_clear_to_color(al_map_rgb(0,0,0));

  al_draw_text(al->logo_font, al_map_rgb(255,255,255), al->width/2, 5, 
      ALLEGRO_ALIGN_CENTRE, "sokoban");

  char text[4][50] = {"Congratulations!","Map: %s","Steps: %d","Press Enter to continue..."};
  
  draw_text_win(0,0);
  draw_text_win(1,name);
  draw_text_win(2,counter);
  draw_text_win(3,0);

  al_flip_display();
  wait_for_key_enter(al);
}

void draw_background(al_defs *al) {
  al_clear_to_color(al_map_rgb(0,0,0));
  al_draw_text(al->logo_font,al_map_rgb(255,255,255),al->width/2,
      5,ALLEGRO_ALIGN_CENTRE,"sokoban");

}
