#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <cstring>
#include <stdio.h>
#include <cmath>
#include <stdlib.h>
#include "jonathanD.h"
#include "game.h"


void init_game ( Game *game )
{
	//game->windowWidth = 800;
	//game->windowHeight = 1024;
	game->difficulty = MED;
	game->demo.on = false;
	game->demo.jump     =   0;
	game->demo.moveLeft =   0;
	game->demo.moveRight =  0;
	game->score = 0;
	game->scoreCount=0;
	game->muted = false;
	game->n=0;
	game->ihead = NULL;
	game->frog = new Frog;
	game->gator = new Gator;
	game->fly = new Fly;
	for ( int i=0; i<4; i++ )
		game->log[i] = new Log;
	game->water[0] = new Water;
	game->water[1] = new Water;
	game->water[2] = new Water;
	game->bridge = new Bridge;
	game->splash = new Splash;
	game->lily = new LilyTexture;
	game->hscore = new Score;
	game->nlily = 0;
	game->timer = 0;
	game->maxtimer = 35;
	game->lilytimer = 35;
	game->lilyspawnpoint = game->windowHeight + 15;
	game->c.radius = 15.0;
	game->c.center[0] = game->windowWidth/2.0;
	game->c.center[1] = 15;
	game->c.detail = 150;
	game->c.jumpSpeed = 0;
	game->c.jumpSpeedMax = 25;
	game->frog->move ( game->c.center[0],game->c.center[1],0,0 );
	game->splash->move ( 0,-10,0,0 );
	game->water[0]->move ( 400,0,0,-2 );
	game->water[1]->move ( 400,500,0,-2 );
	game->water[2]->move ( 400,1000,0,-2 );
	game->playing=true;
	for ( int i = 0; i < 100; i++ ) {
		game->highscore[i] = 0;
	}
}

void demo ( Game *game )
{
	int moving = rand() %5+1;
	int x =rand() %5+1;
	// move right
	if ( moving==1 && game->demo.moveRight==0
	        && game->demo.moveLeft==0 && game->demo.jump ==0 )
		game->demo.moveRight++;
	// move left
	if ( moving==2 && game->demo.moveRight==0
	        && game->demo.moveLeft==0 && game->demo.jump ==0 )
		game->demo.moveLeft++;
	// jump
	if ( moving==3 && game->demo.moveRight==0
	        && game->demo.moveLeft==0 && game->demo.jump ==0 )
		game->demo.jump++;
	if ( game->demo.moveRight>0 ) {
		if ( game->frog->getXpos() <WIDTH )
			game->c.center[0]+=x;
		game->demo.moveRight++;
		game->c.isJumping = false;
		game->c.isStanding = true;
		if ( game->demo.moveRight>60 ) {
			game->demo.moveRight=0;
		}
	}
	if ( game->demo.moveLeft>0 ) {
		if ( game->frog->getXpos() >20 )
			game->c.center[0]-=x;
		game->demo.moveLeft++;
		game->c.isJumping = false;
		game->c.isStanding = true;
		if ( game->demo.moveLeft > 60 ) {
			game->demo.moveLeft = 0;
		}
	}
	if ( game->demo.jump >0 ) {
		game->demo.jump++;
		game->c.isJumping = true;
		game->c.isStanding = false;
		if ( game->demo.jump<=2 ) {
			game->c.velocity[1] = 15.0;
		}
		game->c.center[1] += game->c.velocity[1];
		game->c.velocity[0] = 0;
		game->c.velocity[1] -= 1;
		game->frog->move ( game->c.center[0],game->c.center[1]-0.8,game->c.velocity[0],
		                   game->c.velocity[1] );
		if ( game->demo.jump > 40 && game->c.center[1] < 50 ) {
			game->demo.jump = 0;
			game->c.center[1]=40;
			game->c.velocity[1]=0;
		}
	}
}






