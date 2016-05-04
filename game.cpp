#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <cstring>
#include <stdio.h>
#include <cmath>
#include <stdlib.h>
#include "game.h"
#include "jonathanD.h"

void init_game(Game *game)
{
	//game->windowWidth = 800;
	//game->windowHeight = 1024;
	game->score = 0;

	game->scoreCount=0;
	game->muted = false;
	game->n=0;
	game->ihead = NULL;
	game->frog = new Frog;
	game->gator = new Gator;
	for(int i=0;i<4;i++)
        game->log[i] = new Log;
	game->water[0] = new Water;
	game->water[1] = new Water;
	game->water[2] = new Water;
	game->bridge = new Bridge;
	game->splash = new Splash;
	game->lily = new LilyTexture;
	game->nlily = 0;
	game->timer = 0;
	game->maxtimer = 35;
	game->lilyspawnpoint = game->windowHeight + 15;
	game->c.radius = 15.0;
	game->c.center[0] = game->windowWidth/2.0;
	game->c.center[1] = 15;
	game->c.detail = 150;
	game->c.jumpSpeed = 0;
	game->c.jumpSpeedMax = 25;
	game->frog->move(game->c.center[0],game->c.center[1],0,0);
	game->splash->move(0,-10,0,0);
	game->water[0]->move(400,0,0,-2);
	game->water[1]->move(400,400,0,-2);
	game->water[2]->move(400,600,0,-2);
	game->playing=true;
	for (int i = 0; i < 100; i++) {
		game->highscore[i] = 0;
	}
}
