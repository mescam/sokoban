#include "../include/al_defs.h"
#include "../include/misc.h"
#include <allegro5/allegro.h>

void draw_menu(al_defs* al, char choice){

  char options[5][12] = {"new game","high scores","editor","author","quit"};
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

  if(ev.type == ALLEGRO_EVENT_KEY_UP){
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
    if(ev.type == ALLEGRO_EVENT_KEY_UP && ev.keyboard.keycode==ALLEGRO_KEY_ENTER){
      return;
    }
  }
}

void handle_choice(al_defs* al, char choice, bool* should_quit){
  switch (choice) {
    case 0: //new game
      break;

    case 1: //high scores
      break;

    case 2: //editor
      break;

    case 3: //author
      author_screen(al);
      break;

    case 4: //quit
      *should_quit=1;
      break;
  }
}

