#ifndef _QUYN_H_
#define _QUYN_H_

#include "game.h"

extern void check_mouse(XEvent *e, Game *game);
extern int check_keys(XEvent *e, Game *game);
extern void init_menu(Game *menu);
extern void render_menu(Game *menu);
extern void init_menu_opengl(Game *menu);
#endif
