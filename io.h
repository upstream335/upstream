#ifndef _IO_H_
#define _IO_H_

#include "game.h"
#include "kevinJ.h"

extern void check_mouse(XEvent *e, Game *game);
extern int check_keys(XEvent *e, Game *game);
#endif
