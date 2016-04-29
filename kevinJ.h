#ifndef _SOUND_H_
#define _SOUND_H_

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <cstring>
#include <stdio.h>
#include <cmath>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <stdlib.h>
#include <AL/al.h>
#include <AL/alut.h>
#include "game.h"


extern void playSounds(const char * sound, float gain, bool loop, bool muted);
extern void stopMusic(void); 
extern void playMusic(void); 
extern void maxScore(Game *game);

#endif

