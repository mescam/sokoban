#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/level.h"


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


