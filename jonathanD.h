// Author: Jonathan Dinh
// Date written: 4/18/2016
// Last modified: 5/03/2016
// Purpose: This file holds the structure definition of the lilypads
//
#ifndef _LILLY_
#define _LILLY_

#include <GL/glx.h>
#include "ppm.h"
#include "game.h"

typedef double Vec[3];

struct LilyTexture {
    Ppmimage *lillyImage[3];
    GLuint lillyTexture[3];
    LilyTexture() {
        lillyImage[0] = get_image("./images/lillypad");
		lillyImage[1] = get_image("./images/lillypad1");
		lillyImage[2] = get_image("./images/lillypad2");
        for (int i =0; i< 3; i++) {
			//create opengl texture elements
			glGenTextures(1, &lillyTexture[i]);
			int w = lillyImage[i]->width;
			int h = lillyImage[i]->height;
			//
			glBindTexture(GL_TEXTURE_2D, lillyTexture[i]);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
			unsigned char *lillyData = buildAlphaData(lillyImage[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			             GL_RGBA, GL_UNSIGNED_BYTE, lillyData);
			free(lillyData);
		}
    }
};

struct Lilypad {
    Vec pos;
    Vec vel;
    struct Lilypad *next;
    struct Lilypad *prev;
    Lilypad() {
        next = NULL;
        prev = NULL;
    }
};

extern void checkLilies(Game *game);
extern void drawLilies(Game *game);
extern void deleteLily(Lilypad *node, Game *game);
extern void clearLilies(Game *game);

#endif
