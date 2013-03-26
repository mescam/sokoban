#include <allegro5/allegro.h>
#include "../include/al_defs.h"
#include "../include/graphics.h"
#include <stdio.h>
#include <stdlib.h>

int main(void){
  al_defs al;  
  
  if(!initialize_allegro(&al)){
    fprintf(stderr,"[Sokoban] I'm sorry\n");
    return -1;
  }

  //TODO: stuff
  printf("%d %d\n\n",al.width,al.height); 
  char choice=0;
  bool pressed_enter=0;
  
  while(1){
    draw_menu(&al, choice);
    //remember about choice%options
    handle_key_event(&al, &choice, &pressed_enter);
    if(pressed_enter){
      bool should_quit=0;
      handle_choice(&al, choice, &should_quit);

      if(should_quit)
        break;
      pressed_enter=0;
      choice=0;
    }
    //al_rest(10.0);
  }


  shutdown_allegro(&al);
  
  return 0;
}
