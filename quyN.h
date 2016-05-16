#ifndef _QUYN_H_
#define _QUYN_H_

#include "game.h"
#include </usr/include/AL/alut.h>

/*#ifdef USE_OPENAL_SOUND
extern void init_sound();
extern void cleanup_sound();
extern void play_sound(ALuint source);
#endif
*/

struct ButtonTextures {
	Vec pos;
	Ppmimage *buttonImage[9];
	GLuint buttonTexture[9];
	ButtonTextures() {
		buttonImage[0] = get_image("./images/play");
		buttonImage[1] = get_image("./images/score");
		buttonImage[2] = get_image("./images/resume");
		buttonImage[3] = get_image("./images/exit");
		buttonImage[4] = get_image("./images/restart");
		buttonImage[5] = get_image("./images/paused");
		buttonImage[6] = get_image("./images/mute");
		buttonImage[7] = get_image("./images/easy");
		buttonImage[8] = get_image("./images/medium");
		buttonImage[9] = get_image("./images/hard");
		for (int i = 0; i < 9; i++) {
			//create opengl texture elements
			glGenTextures(1, &buttonTexture[i]);
			int w = buttonImage[i]->width;
			int h = buttonImage[i]->height;
			//
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

#endif

