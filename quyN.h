#ifndef _QUYN_H_
#define _QUYN_H_

#include "game.h"
#include </usr/include/AL/alut.h>

struct ButtonTextures {
	Ppmimage *buttonImage[16];
	GLuint buttonTexture[16];
	ButtonTextures() {
		buttonImage[0] = get_image("./images/play");
		buttonImage[1] = get_image("./images/medium");
		buttonImage[2] = get_image("./images/score");
		buttonImage[3] = get_image("./images/credits");
		buttonImage[4] = get_image("./images/exit");
		buttonImage[5] = get_image("./images/resume");
		buttonImage[6] = get_image("./images/sound");
		buttonImage[7] = get_image("./images/restart");
		buttonImage[8] = get_image("./images/mainMenu");
		buttonImage[9] = get_image("./images/exit");
		buttonImage[10] = get_image("./images/play");
		buttonImage[11] = get_image("./images/mainMenu");
		buttonImage[12] = get_image("./images/exit");
		buttonImage[13] = get_image("./images/mute");
		buttonImage[14] = get_image("./images/easy");
		buttonImage[15] = get_image("./images/hard");
		for (int i = 0; i < 16; i++) {
			glGenTextures(1, &buttonTexture[i]);
			int w = buttonImage[i]->width;
			int h = buttonImage[i]->height;
			glBindTexture(GL_TEXTURE_2D, buttonTexture[i]);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
			unsigned char *buttonData = buildAlphaData(buttonImage[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
					GL_RGBA, GL_UNSIGNED_BYTE, buttonData);
			free(buttonData);
		}
	}
};

extern void reset_game(Game *game);
extern void init_buttons(Game *game);
extern void render_main_menu(Game *game);
extern void render_sub_menu(Game *game);
extern void render_gameover_menu(Game *game);

extern void render_main_menu_buttons(Game *game);
extern void render_sub_menu_buttons(Game *game);
extern void render_gameover_menu_buttons(Game *game);
extern void render_ingame_buttons(Game *game);
extern void render_highscore(Game *game);
extern void render_credits(Game *game);

extern void check_menu_mouse(XEvent *e, Game *game);
extern void check_paused_mouse(XEvent *e, Game *game);
extern void check_gameover_mouse(XEvent *e, Game *game);

#endif
