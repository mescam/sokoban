#include "al_defs.h"
#include "level.h"

void load_resources(al_defs* al);
void unload_resources(al_defs* al);
void shutdown_allegro(al_defs *al);
bool initialize_allegro(al_defs *al);
void draw_tile(al_defs *al, int i, int j, level* lvl);
void draw_level_map(al_defs* al, level* lvl, const int counter, char *name);
void display_win_board(al_defs *al, const int counter, char *name);
