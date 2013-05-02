#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/level.h"
#include "../include/graphics.h"

level *read_level(char *name) {
  fprintf(stdout, "[Sokoban] read_level(%s) called\n",name);
  FILE *handle;
  char *fStr = (char*)malloc(sizeof(char)*128);
  strcpy(fStr,"res/levels/\0");
  strcat(fStr, name);
  fprintf(stdout, "[Sokoban] opening %s\n",fStr);
  handle = fopen(fStr, "r");
  fprintf(stdout, "[Sokoban] free(fStr);\n");

  free(fStr);
  if(handle==NULL){
    fprintf(stderr, "[Sokoban] Error loading level name\n");
    return /*false*/ NULL;
  }
  fprintf(stdout, "[Sokoban] level file opened\n");
  level *lvl = (level*)malloc(sizeof(level));
  fscanf(handle, "%d %d\n",&(lvl->h), &(lvl->w));
  fprintf(stdout, "[Sokoban] level header read\n");

  lvl->map = (char**)malloc(sizeof(char*)*lvl->h);
  int i,j;
  for (i = 0; i < lvl->h; i++) {
    lvl->map[i] = (char*)malloc(sizeof(char)*lvl->w);
  }
  fprintf(stdout, "[Sokoban] memory allocated\n");

  for (i = 0; i < lvl->h; i++) {
    //fgets(lvl->map[i], lvl->w+1, handle);
    for (j = 0; j < lvl->w; j++) {
      lvl->map[i][j] = getc(handle);
    }
    getc(handle); //new line symbol
    fprintf(stdout, "[Sokoban] line %d processed\n",i);
  }
  fclose(handle);
  fprintf(stdout, "[Sokoban] returning level structure (end of read_level)\n");
  return lvl;
}

void free_level(level *lvl) {
  int i;
  for(i = 0; i < lvl->h; i++)
    free(lvl->map[i]);
  free(lvl->map);
  free(lvl);
}

bool play_level(al_defs *al, level *lvl, char *name) {
  int counter=0;
  fprintf(stdout,"[Sokoban] playing level %s\n",name);
  bool playing = true, reset = false;
  //bool found = false;
  int i,j,pi,pj,ni,nj;
  //pi, pj - player's position
  //ni, nj - destination point
  char **original_map = (char**)malloc(lvl->h*sizeof(char*));

  //searching for player coords
  for(i = 0; i < lvl->h; i++){
    original_map[i] = (char*)malloc(lvl->w*sizeof(char));
    for(j = 0; j < lvl->w; j++) {
      if(lvl->map[i][j]=='P') {
        pi=i; pj=j;
      }
      original_map[i][j]=lvl->map[i][j];
    }
  }

  fprintf(stdout,"[Sokoban] found player at [%d][%d]\n",pi,pj);
  while(playing) {
    draw_level_map(al, lvl, counter, name);
    ALLEGRO_EVENT ev;

    al_wait_for_event(al->queue, &ev);
    ni=pi; nj=pj; //no changes by default

    if(ev.type == ALLEGRO_EVENT_KEY_DOWN){
      switch(ev.keyboard.keycode) {
        case ALLEGRO_KEY_ESCAPE:
          playing=false;
          break;

        case ALLEGRO_KEY_R:
          playing=false;
          reset=true;
          break;

        case ALLEGRO_KEY_UP:
          ni=pi-1;
          break;

        case ALLEGRO_KEY_DOWN:
          ni=pi+1;
          break;

        case ALLEGRO_KEY_LEFT:
          nj=pj-1;
          break;

        case ALLEGRO_KEY_RIGHT:
          nj=pj+1;
          break;
      }
      if(ni!=pi || nj!=pj) { //player wants to move!
        if(ni<0 || ni==lvl->h || nj<0 || nj==lvl->w) continue; //nope!
        if(lvl->map[ni][nj]=='#') continue; //nope!
        if(lvl->map[ni][nj]=='S' || lvl->map[ni][nj]=='Z') {
          //continue; //not now :X
          int off_i = ni-pi, off_j = nj-pj;
          if(lvl->map[ni+off_i][nj+off_j]=='#' || 
              lvl->map[ni+off_i][nj+off_j]=='S' ||
              lvl->map[ni+off_i][nj+off_j]=='Z') 
            continue;

          if(lvl->map[ni+off_i][nj+off_j]=='X')
            lvl->map[ni+off_i][nj+off_j]='Z';
          else
            lvl->map[ni+off_i][nj+off_j]='S';

        }
        lvl->map[ni][nj]='P';
        if(original_map[pi][pj]=='X')
          lvl->map[pi][pj]='X';
        else
          lvl->map[pi][pj]=' ';

        counter++; pi=ni; pj=nj;

      }
    }
  }
  return reset;
}
