#include "../include/al_defs.h"
#include "../include/misc.h"
#include "../include/level.h"
#include "../include/graphics.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdio.h>
#include <dirent.h>

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

void handle_key_event(al_defs* al, char* choice, bool* pressed_enter, int count){
  ALLEGRO_EVENT ev;
  al_wait_for_event(al->queue, &ev);

  if(ev.type == ALLEGRO_EVENT_KEY_DOWN){
    switch (ev.keyboard.keycode) {
      case ALLEGRO_KEY_UP:
        (*choice)--;
        if(*choice<0) *choice=count-1;
        break;

      case ALLEGRO_KEY_DOWN:
        (*choice)++;
        *choice = (*choice)%count;
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
      /*do {
        if(l!=NULL) free_level(l);
        l = read_level("nowai");
        if(l==NULL) break;
        }while(play_level(al, l, "nowai"));
        free_level(l);*/
      available_levels(al);
      //al_rest(10.0);

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

typedef struct _level_list_mem {
  char name[255];
  struct _level_list_mem *prev, *next;
} level_list_mem;

typedef struct _level_list {
  struct _level_list_mem *first, *last;
} level_list;

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


void available_levels(al_defs *al) {

  bool quit=false;
  level_list *list = get_level_list();
  level_list_mem *p = list->first;
  level* l = NULL;

  while(!quit) {
    al_clear_to_color(al_map_rgb(0,0,0));
    al_draw_text(al->logo_font,al_map_rgb(255,255,255),al->width/2,
        20,ALLEGRO_ALIGN_CENTRE,"sokoban");
    al_draw_text(al->menu_font,al_map_rgb(255,255,255), al->width/2, 
        160, ALLEGRO_ALIGN_CENTRE,"Select your level:");

    al_draw_textf(al->menu_font, al_map_rgb(255,0,0), al->width/2, 
        al->height/2, ALLEGRO_ALIGN_CENTRE, "%s", p->name);

    al_draw_text(al->hint_font, al_map_rgb(120, 120, 120), 10,
        al->height-30, ALLEGRO_ALIGN_LEFT, 
        "Use arrows to navigate, ENTER to choose, Esc to return to the main screen");
    al_flip_display();

    ALLEGRO_EVENT ev;
    al_wait_for_event(al->queue, &ev);
    if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
      printf("EVENT KEYCODE: %d\n",ev.keyboard.keycode);
      switch(ev.keyboard.keycode) {
        case ALLEGRO_KEY_ESCAPE:
          quit=true;
          break;

        case ALLEGRO_KEY_ENTER: 

          do {
            if(l!=NULL) free_level(l);
            l = read_level(p->name);
            if(l==NULL) break;
          }while(play_level(al, l, p->name));
          free_level(l);
          l=NULL;
          break;


        case ALLEGRO_KEY_LEFT:
          p=p->prev;
          break;

        case ALLEGRO_KEY_RIGHT:
          p=p->next;
          break;
      }
    }
  }
}
