#ifndef _QUYN_H_
#define _QUYN_H_

#include "game.h"
#include </usr/include/AL/alut.h>

#ifdef USE_OPENAL_SOUND
extern void init_sound();
extern void cleanup_sound();
extern void play_sound(ALuint source);
#endif

extern void reset_game(Game *game);
extern void init_buttons(Game *game);
extern void render_main_menu(Game *game);
extern void render_sub_menu(Game *game);
extern void render_menu_buttons(Game *game);
extern void render_continue_button(Game *game);

#endif

