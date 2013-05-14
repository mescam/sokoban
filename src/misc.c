#include "../include/al_defs.h"
#include "../include/misc.h"
#include "../include/level.h"
#include "../include/graphics.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdio.h>

void draw_menu(al_defs* al, char choice){
  char options[5][12] = {"new game","editor","author","quit","secret!"};
  al_clear_to_color(al_map_rgb(0,0,0));

  //flags.
  al_draw_text(al->logo_font,al_map_rgb(254,255,255),al->width/2,20,ALLEGRO_ALIGN_CENTRE,"sokoban");
  
  int i;
  for (i = 0; i < sizeof(options)/(sizeof(char)*12); i++) {
    ALLEGRO_COLOR c;
    c = (choice==i)?al_map_rgb(255,0,0):al_map_rgb(255,255,255);
    al_draw_textf(al->menu_font, c, al->width/2, 75+(i*25),ALLEGRO_ALIGN_CENTRE,"%s",options[i]);
  }
  al_flip_display();
}

void handle_key_event(al_defs* al, char* choice, bool* pressed_enter){
  ALLEGRO_EVENT ev;
  al_wait_for_event(al->queue, &ev);

  if(ev.type == ALLEGRO_EVENT_KEY_DOWN){
    switch (ev.keyboard.keycode) {
      case ALLEGRO_KEY_UP:
        (*choice)--;
        if(*choice<0) *choice=4;
        break;

      case ALLEGRO_KEY_DOWN:
        (*choice)++;
        *choice = (*choice)%5;
        break;

      case ALLEGRO_KEY_ENTER:
        *pressed_enter=1;
        break;
    }
  }
}

void author_screen(al_defs* al){
  al_clear_to_color(al_map_rgb(0,0,0));
  al_draw_text(al->logo_font,al_map_rgb(254,255,255),al->width/2,20,ALLEGRO_ALIGN_CENTRE,"sokoban");
  ALLEGRO_COLOR c = al_map_rgb(255,255,255);
  char info[5][60] = {"Jakub Wozniak", "Student", "Faculty of Computing Science", "Poznan University of Technology", "Album: 109686"};
  int i;
  for (i = 0; i < sizeof(info)/(sizeof(char)*60); i++) {
    al_draw_textf(al->menu_font, c, al->width/2, 90+(i*35), ALLEGRO_ALIGN_CENTRE, "%s", info[i]);
  }
  al_flip_display();
  wait_for_key_enter(al);
  return;
}


void wait_for_key_enter(al_defs* al){
  ALLEGRO_EVENT ev;
  while(1){
    al_wait_for_event(al->queue, &ev);
    if(ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode==ALLEGRO_KEY_ENTER){
      return;
    }
  }
}

void inquisition(al_defs* al) {
  al_install_audio();
  al_init_acodec_addon();
  al_reserve_samples(1);
  ALLEGRO_SAMPLE *sample = al_load_sample("res/spanish.wav");
  if(!sample) fprintf(stderr, "nie :< \n");
  al_clear_to_color(al_map_rgb(0,0,0));
  ALLEGRO_BITMAP* bmp = al_load_bitmap("res/troll.jpg");
  al_draw_bitmap(bmp, (al->width-500)/2, (al->height-360)/2, 0);
  al_flip_display();
  al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
  wait_for_key_enter(al);
  al_destroy_bitmap(bmp);
  al_destroy_sample(sample);
}

void handle_choice(al_defs* al, char choice, bool* should_quit){
  level *l = NULL;
  int i,j;
  switch (choice) {
    case 0: //new game      
      do {
        if(l!=NULL) free_level(l);
        l = read_level("nowai");
        if(l==NULL) break;
      }while(play_level(al, l, "nowai"));
      free_level(l);
      
      break;

    case 1: //editor
      break;

    case 2: //author
      author_screen(al);
      break;

    case 3: //quit
      *should_quit=1;
      break;

    case 4: //inquisition
      inquisition(al);
      break;

  }
}

