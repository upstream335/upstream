#ifndef _GAME_
#define _GAME_

#include "johnH.h"

typedef double Vec[3];
struct Lilypad;
struct LilyTexture;
struct Score;
struct Circle
{
	float radius;
	float newPosX;
	float newPosY;
	float jumpSpeed;
	float jumpSpeedMax;
	bool isJumping;
	bool isStanding;
	Vec velocity;
	Vec center;
	int detail;
};

struct Game
{
	bool playing;
	int windowWidth;
	int windowHeight;
	int score;
	int highscore[100];
	int scoreCount;
	bool muted ;
	int nlily;
	int timer;
	int maxtimer;
	int lilyspawnpoint; //y coordinate of where lilies spawn
	Lilypad *ihead;
	Score *hscore;
	LilyTexture *lily;
	Circle c;
	int n;
	Frog *frog;
	Gator *gator;

	Log *log[4];
	Water *water[3];
	Bridge *bridge;
	Splash *splash;
	Game()
	{
		c.isStanding = true;
		windowWidth = 600;
		windowHeight = 1024;
	}
};

extern void init_game(Game *game);

#endif

