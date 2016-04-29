// author: Jonathan Dinh
// cs335 project
// this file holds the structure of the lilypads
//
#ifndef _LILLY_
#define _LILLY_

#include <GL/glx.h>
#include "ppm.h"
#include "game.h"


typedef double Vec[3];

struct Lilypad {
    Vec pos;
    Vec vel;
    struct Lilypad *next;
    struct Lilypad *prev;
    //Ppmimage *lillyImage[3];
    GLuint lillyTexture[3];
    Lilypad() {
        //lillyImage[0] = ppm6GetImage("./images/lillypad.ppm");
        //lillyImage[1] = ppm6GetImage("./images/lillypad1.ppm");
        //lillyImage[2] = ppm6GetImage("./images/lillypad2.ppm");
        next = NULL;
        prev = NULL;
    }
};

extern void checkLilies(Game *game);
extern void drawLilies(Game *game);
extern void deleteLily(Lilypad *node, Game *game);
extern void clearLilies(Game *game);

#endif
