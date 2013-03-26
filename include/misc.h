#ifndef MISC_H

#define MISC_H
#include "al_defs.h"
#include <allegro5/allegro.h>

void draw_menu(al_defs* al, char choice);
void handle_key_event(al_defs* al, char* choice, bool* pressed_enter);
void handle_choice(al_defs* al, char choice, bool* should_quit);
void wait_for_key_enter(al_defs* al);
void author_screen(al_defs* al);

#endif /* end of include guard: MISC_H */
