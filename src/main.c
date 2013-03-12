#include <allegro5/allegro.h>
#include "../include/al_defs.h"
#include "../include/graphics.h"
<<<<<<< HEAD
#include <stdio.h>
#include <stdlib.h>
=======
>>>>>>> 86a40f0a9f068cc5f29c015aafb38816e42e7d88

#define S_WIDTH 640
#define S_HEIGHT 480
int main(void){
  al_defs al;

  al.width = S_WIDTH;
  al.height = S_HEIGHT;
  
  if(!initialize_allegro(&al)){
    fprintf(stderr,"[Sokoban] I'm sorry\n");
    return -1;
  }

  //TODO: stuff
  
  char choice=0;
  bool pressedEnter=0;
  
  while(1){
    drawMenu(&al, choice);
    //remember about choice%options
    handleKeyEvent(&al, &choice, &pressedEnter);
    if(pressedEnter){
      bool shouldQuit=0;
      handleChoice(&al, choice, &shouldQuit);

      if(shouldQuit)
        break;
      pressedEnter=0;
      choice=0;
    }
  }


  shutdown_allegro(&al);
  
  return 0;
}
