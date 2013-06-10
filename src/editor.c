#include "../include/editor.h"
#include "../include/al_defs.h"
#include "../include/graphics.h"
#include "../include/level_list.h"
#include "../include/level.h"
#include <stdlib.h>

//private definitions
void ask_for_level(al_defs *al, char *name);
bool check_if_level_exists(char name[255]);
void ask_for_dimensions(al_defs *al, unsigned int *x, unsigned int *y);
void make_blank_level(char name[255], unsigned int x, unsigned int y);
void edit_level(al_defs *al, char name[255]);
void edit_or_remove(al_defs *al, char name[255]);
void remove_level(char name[255]);
void save_map(char name[255], level *lvl);

void editor_logic(al_defs *al) {
  char name[255];
  ask_for_level(al, name);
  if(!check_if_level_exists(name)) {
    unsigned int x,y;
    ask_for_dimensions(al, &x, &y);
    make_blank_level(name, x, y);
    edit_level(al, name);
  }else{
    edit_or_remove(al, name);
  }
}

void save_map(char name[255], level *lvl) {
  char p[255+50] = "res/levels/";
  strcat(p,name);
  FILE *f = fopen(p,"w");
  if(f==NULL) return;
  int i,j;
  fprintf(f,"%d %d\n",lvl->h,lvl->w);
  for(i=0;i<lvl->h;i++){
    for(j=0;j<lvl->w;j++){
      fprintf(f,"%c",lvl->map[i][j]);
    }
    fprintf(f,"\n");
  }
  fclose(f);
}

void edit_level(al_defs *al, char name[255]) {
  level *lvl = read_level(name);
  int posx,posy; posx=posy=1;
  while(true){
    draw_background(al);
    int i,j;
    for(i=0;i<lvl->h;i++) {
      for(j=0;j<lvl->w;j++) {
        draw_tile(al, i, j, lvl);
      }
    }
    draw_tile_border(al, posy, posx, lvl);
    al_draw_text(al->hint_font, al_map_rgb(120,120,120),
        20,al->height-80,ALLEGRO_ALIGN_LEFT,
        "W - wall, O - nothing");
    al_draw_text(al->hint_font, al_map_rgb(120,120,120),
        20,al->height-60,ALLEGRO_ALIGN_LEFT,
        "P - player, S - chest, X - destination point");
    al_draw_text(al->hint_font, al_map_rgb(120,120,120),
        20,al->height-40,ALLEGRO_ALIGN_LEFT,
        "Z - chest in correct place, C - player on dest. point");
    al_draw_text(al->hint_font, al_map_rgb(120,120,120),
        20,al->height-20,ALLEGRO_ALIGN_LEFT,
        "Esc - escape, Enter - save");
    al_flip_display();

    //event time!
    ALLEGRO_EVENT ev;
    al_wait_for_event(al->queue, &ev);

    if(ev.type!=ALLEGRO_EVENT_KEY_DOWN) 
      continue;

    switch(ev.keyboard.keycode) {
      case ALLEGRO_KEY_LEFT:
        if(posx>1) posx--;
        break;
      case ALLEGRO_KEY_RIGHT:
        if(posx<(lvl->w-2)) posx++;
        break;
      case ALLEGRO_KEY_UP:
        if(posy>1) posy--;
        break;
      case ALLEGRO_KEY_DOWN:
        if(posy<(lvl->h-2)) posy++;
        break;
      case ALLEGRO_KEY_P:
        for(i=0;i<lvl->h;i++)
          for(j=0;j<lvl->w;j++)
            if(lvl->map[i][j]=='P' || lvl->map[i][j]=='C')
              lvl->map[i][j]=' ';
        lvl->map[posy][posx]='P';
        break;
      case ALLEGRO_KEY_X:
        lvl->map[posy][posx]='X';
        break;
      case ALLEGRO_KEY_Z:
        lvl->map[posy][posx]='Z';
        break;
      case ALLEGRO_KEY_S:
        lvl->map[posy][posx]='S';
        break;
      case ALLEGRO_KEY_C:
        for(i=0;i<lvl->h;i++)
          for(j=0;j<lvl->w;j++)
            if(lvl->map[i][j]=='P' || lvl->map[i][j]=='C')
              lvl->map[i][j]=' ';

        lvl->map[posy][posx]='C';
        break;
      case ALLEGRO_KEY_O:
        lvl->map[posy][posx]=' ';
        break;
      case ALLEGRO_KEY_W:
        lvl->map[posy][posx]='#';
        break;
      case ALLEGRO_KEY_ENTER:
        save_map(name, lvl);
        return;
      case ALLEGRO_KEY_ESCAPE:
        return;
    }

  }
}

void remove_level(char name[255]) {
  //dec level num
  FILE *f = fopen("res/levels/levels","r");
  int i,d;
  fscanf(f,"%d",&d);
  //char* maps = (char*)malloc(sizeof(char)*d);
  char maps[300][255];
  for(i=0;i<d;i++)
    fscanf(f,"%s",maps[i]);
  fclose(f);
  f = fopen("res/levels/levels","w");
  fprintf(f,"%d\n",d-1);
  for(i=0;i<d;i++){
    if(strcmp(name,maps[i]))
      fprintf(f,"%s\n",maps[i]);
  }
  fclose(f);
  //so silent before the storm
  //awaiting command
  //a few has been chosen to stand
  //as one outnumbered by faaaar
  //free(maps);
  char p[255+50] = "res/levels/";
  strcat(p,name);
  remove(p);
}


void edit_or_remove(al_defs *al, char name[255]) {
  draw_background(al);
  al_draw_text(al->menu_font, al_map_rgb(150,150,150), al->width/2,
      120, ALLEGRO_ALIGN_CENTRE, "Press E to edit, R to remove");
  al_flip_display();
  ALLEGRO_EVENT ev;
  bool quit=false;
  while(!quit) {
    al_wait_for_event(al->queue, &ev);
    if(ev.type==ALLEGRO_EVENT_KEY_DOWN) {
      switch(ev.keyboard.keycode){
        case ALLEGRO_KEY_E:
          quit=true;
          edit_level(al, name);
          break;
        case ALLEGRO_KEY_R:
          quit=true;
          remove_level(name);
      }
    }
  }
}

void make_blank_level(char name[255], unsigned int x, unsigned int y) {
  char p[255+50];
  strcpy(p,"res/levels/");
  strcat(p,name);
  FILE *f = fopen(p,"w");
  fprintf(f,"%d %d\n",y,x);
  int i,j;
  for(i=0;i<y;i++) {
    for(j=0;j<x;j++)
      fprintf(f,"%c",(i==0||i==(y-1)||j==0||j==(x-1))?'#':' ');
    fprintf(f,"\n");
  }
  fclose(f);
  //inc level list
  f = fopen("res/levels/levels","r");
  int d;
  fscanf(f,"%d",&d);
  char maps[100][255];// = (char*)malloc(sizeof(char)*d);
  for(i=0;i<d;i++){
    fscanf(f,"%s",maps[i]);
  }
  fclose(f);
  f = fopen("res/levels/levels","w");
  fprintf(f,"%d\n",d+1);
  for(i=0;i<d;i++)
    fprintf(f,"%s\n",maps[i]);
  fprintf(f,"%s\n",name);
  fclose(f);
  //free(maps);
}

bool check_if_level_exists(char name[255]) {
  char path[255+50];
  strcpy(path,"res/levels/");
  strcat(path,name);
  FILE* f = fopen(path,"r");
  if(f==NULL) return false;
  fclose(f);
  return true;
}

void ask_for_dimensions(al_defs* al, unsigned int *x, unsigned int *y) {
  ALLEGRO_EVENT ev;
  *x=*y=5;
  while(true) {
    draw_background(al);
    al_draw_text(al->menu_font, al_map_rgb(255,255,255),
        al->width/2,80,ALLEGRO_ALIGN_CENTRE,"Dimensions:");

    al_draw_textf(al->menu_font, al_map_rgb(150,150,150),
        al->width/2,120,ALLEGRO_ALIGN_CENTRE,"Width: %d",*x);
    al_draw_textf(al->menu_font, al_map_rgb(150,150,150),
        al->width/2,160,ALLEGRO_ALIGN_CENTRE,"Height: %d",*y);

    al_draw_text(al->hint_font, al_map_rgb(120,120,120),
        al->width/2,al->height-40,ALLEGRO_ALIGN_CENTRE,
        "Use arrows to change dimensions, press ENTER to continue.");
    al_flip_display();
    al_wait_for_event(al->queue,&ev);

    if(ev.type != ALLEGRO_EVENT_KEY_DOWN) 
      continue;

    if(ev.keyboard.keycode==ALLEGRO_KEY_ENTER)
      break;

    switch(ev.keyboard.keycode) {
      case ALLEGRO_KEY_DOWN:
        if((*y)>5) (*y)--;
        break;

      case ALLEGRO_KEY_UP:
        (*y)++;
        break;

      case ALLEGRO_KEY_LEFT:
        if((*x)>5) (*x)--;
        break;

      case ALLEGRO_KEY_RIGHT:
        (*x)++;
        break;
    }
  }
}

void ask_for_level(al_defs *al, char *name) {
  int pos=0;
  name[0]='\0';
  ALLEGRO_EVENT ev;
  while(true) {
    draw_background(al);
    al_draw_text(al->menu_font,al_map_rgb(255,255,255),
        al->width/2,80,ALLEGRO_ALIGN_CENTRE,"Please enter the level name:");
    al_draw_textf(al->menu_font,al_map_rgb(150,150,150),
        al->width/2,140,ALLEGRO_ALIGN_CENTRE,"%s",name);
    al_draw_text(al->hint_font, al_map_rgb(120,120,120),
        al->width/2,al->height-40,ALLEGRO_ALIGN_CENTRE,
        "Only alphanumeric and spacebar (will be shown as underscore)");
    al_flip_display();
    al_wait_for_event(al->queue, &ev);
    if(ev.type != ALLEGRO_EVENT_KEY_DOWN)
      continue;

    if(ev.keyboard.keycode==ALLEGRO_KEY_ENTER) {
      if(strcmp(name,"levels")!=0)
        break;
    }

    if(ev.keyboard.keycode==ALLEGRO_KEY_BACKSPACE) {
      if(pos>0) 
        name[--pos]='\0';
    }

    if((ev.keyboard.keycode > 0 && ev.keyboard.keycode < 37) 
        || ev.keyboard.keycode == 75) {
      if(ev.keyboard.keycode==75) {
        name[pos++] = '_';
      }else{
        name[pos++] = *(al_keycode_to_name(ev.keyboard.keycode));
      }
      name[pos]='\0';
      if(pos>200) pos--;
    }
  }
}


