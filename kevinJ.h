#ifndef _KEVINJ_H_
#define _KEVINJ_H_

#include <AL/al.h>
#include <AL/alut.h>
#include "game.h"

extern void playSounds(const char * sound, float gain, bool loop, bool muted);
extern void stopMusic(void); 
extern void playMusic(void); 
extern void maxScore(Game *game);

#endif

