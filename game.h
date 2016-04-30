#ifndef _GAME_
#define _GAME_

#include "johnH.h"
typedef double Vec[3];
struct Lilypad;

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
	Ppmimage *lillyImage[3];
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
		lillyImage[0] = ppm6GetImage("./images/lillypad.ppm");
		lillyImage[1] = ppm6GetImage("./images/lillypad1.ppm");
		lillyImage[2] = ppm6GetImage("./images/lillypad2.ppm");
		windowWidth = 600;
		windowHeight = 1024;
	}
};

extern void init_game(Game *game);

#endif

