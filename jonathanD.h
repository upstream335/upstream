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
#include <string>
#include <sstream>
typedef double Vec[3];

struct Lilypad {
	Vec pos;
	Vec vel;
	int size;
	struct Lilypad *next;
	struct Lilypad *prev;
	Lilypad() {
		next = NULL;
		prev = NULL;
	}
};

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

struct Ripple {
    Vec pos;
    float radius;
    int detail;
    Ripple() {
        radius = 10.0;
        detail = 400;
    }
};

struct Score {
	Vec pos;
	Ppmimage *scoreImage[10];
	GLuint scoreTexture[10];
	Score() {
		scoreImage[0] = get_image("./images/zero");
		scoreImage[1] = get_image("./images/one");
		scoreImage[2] = get_image("./images/two");
		scoreImage[3] = get_image("./images/three");
		scoreImage[4] = get_image("./images/four");
		scoreImage[5] = get_image("./images/five");
		scoreImage[6] = get_image("./images/six");
		scoreImage[7] = get_image("./images/seven");
		scoreImage[8] = get_image("./images/eight");
		scoreImage[9] = get_image("./images/nine");
		for (int i = 0; i < 10; i++) {
			//create opengl texture elements
			glGenTextures(1, &scoreTexture[i]);
			int w = scoreImage[i]->width;
			int h = scoreImage[i]->height;
			//
			glBindTexture(GL_TEXTURE_2D, scoreTexture[i]);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
			unsigned char *scoreData = buildAlphaData(scoreImage[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
					GL_RGBA, GL_UNSIGNED_BYTE, scoreData);
			free(scoreData);
		}
	}
};

extern void checkLilies(Game *game);
extern void drawLilies(Game *game);
extern void deleteLily(Lilypad *node, Game *game);
extern void clearLilies(Game *game);
extern void drawScore(int s, Game *game,int);
#endif
