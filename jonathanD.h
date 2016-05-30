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
	bool left;
	struct Lilypad *next;
	struct Lilypad *prev;
	Lilypad() {
		next = NULL;
		prev = NULL;
	}
};

struct HighScoreBox {
    Vec pos;
    Ppmimage *hscoreboxImage[1];
	GLuint hscoreboxTexture[1];
	HighScoreBox() {
        hscoreboxImage[0] = get_image("./images/woodtexture3");
        glGenTextures(1, &hscoreboxTexture[0]);
        int w = hscoreboxImage[0]->width;
        int h = hscoreboxImage[0]->height;
        glBindTexture(GL_TEXTURE_2D, hscoreboxTexture[0]);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        unsigned char *hscoreboxData = buildAlphaData(hscoreboxImage[0]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, hscoreboxData);
        free(hscoreboxData);
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
	Ppmimage *scoreImage[20];
	GLuint scoreTexture[20];
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
		scoreImage[10] = get_image("./images/zeroP");
		scoreImage[11] = get_image("./images/oneP");
		scoreImage[12] = get_image("./images/twoP");
		scoreImage[13] = get_image("./images/threeP");
		scoreImage[14] = get_image("./images/fourP");
		scoreImage[15] = get_image("./images/fiveP");
		scoreImage[16] = get_image("./images/sixP");
		scoreImage[17] = get_image("./images/sevenP");
		scoreImage[18] = get_image("./images/eightP");
		scoreImage[19] = get_image("./images/nineP");
		for (int i = 0; i < 20; i++) {
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

extern void displayNlily(int a);
extern void displayFPS(int a, Game *game);
extern void initHighScore(Game *game);
extern bool checkHighScore(Game *game,int score);
extern void getName(XEvent *e, Game *game);
extern void drawHighScoreBox(Game *game);
extern void drawBubble(Game *game);
extern void checkLilies(Game *game);
extern void drawLilies(Game *game);
extern void deleteLily(Lilypad *node, Game *game);
extern void clearLilies(Game *game);
extern void drawScore(int s, Game *game,int,int,int);
extern bool getHighScore(Game *game, char shost[], char spage[], bool cscore, bool pscore);
#endif
