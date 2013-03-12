#ifndef MISC_H

#define MISC_H
#include "al_defs.h"
#include <allegro5/allegro.h>

void drawMenu(al_defs* al, char choice);
void handleKeyEvent(al_defs* al, char* choice, bool* pressedEnter);
void handleChoice(al_defs* al, char choice, bool* shouldQuit);


#endif /* end of include guard: MISC_H */
