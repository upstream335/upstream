// Author: Jonathan Dinh
// Date written: 4/18/2016
// Last modified: 5/01/2016
// Purpose: This file holds the structure definition of the lilypads
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
        next = NULL;
        prev = NULL;
    }
};

extern void checkLilies(Game *game);
extern void drawLilies(Game *game);
extern void deleteLily(Lilypad *node, Game *game);
extern void clearLilies(Game *game);

#endif
