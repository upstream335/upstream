#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <cstring>
#include <stdio.h>
#include <cmath>
#include <stdlib.h>
#include "quyN.h"
#include "jonathanD.h"
#include "game.h"

void init_game ( Game *game )
{
	game->fps = 0;
	game->showfps = 0;
	game->lilycount = 0;
	game->bossGO = false;
	game->x = game->windowWidth/2;
	game->difficulty = MED;
	game->lives = 1;
	game->demo.on = false;
	game->demo.jump     =   0;
	game->demo.moveLeft =   0;
	game->demo.moveRight =  0;
	game->score = 0;
	game->nbullets = 0;
	game->barr = new Bullet[10];
	game->scoreCount=0;
	game->muted = false;
	game->rocketSound = true;
	game->n=0;
	game->ihead = NULL;
	game->hud = new HUD;
	game->meter = new Meter;
	game->frog = new Frog;
	game->gator = new Gator;
	game->monster = new Monster;
	game->fly = new Fly;
	game->swarmSize = 5;
	for ( int i=0; i<game->swarmSize; i++ )
		game->swarm[i] = new Fly;
	game->swarmOn = false;

	game->turtle = new Turtle;

	for ( int i=0; i<4; i++ )
		game->log[i] = new Log;

	game->waterbg = new WaterBG;
	game->water[0] = new Water;
	game->water[1] = new Water;
	game->water[2] = new Water;
	game->bridge = new Bridge;
	game->splash = new Splash;
	game->explosion = new Explosion;
	game->lily = new LilyTexture;
	game->hscore = new Score;
	game->rocketPack = new RocketPack;
	game->hscorebox = new HighScoreBox;
	game->helpbg = new helpBG;
	game->help_menu = false;
	game->hscorebox->pos[0] = game->windowWidth/2;
	game->hscorebox->pos[1] = game->windowHeight/2;
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
	game->waterbg->move (game->windowWidth/2,game->windowHeight/2,0,0);
	game->water[0]->move ( game->windowWidth/2,0,0,-2.2 );
	game->water[1]->move ( game->windowWidth/2,350,0,-2.2 );
	game->water[2]->move ( game->windowWidth/2,700,0,-2.2 );
	game->playing=true;
	for ( int i = 0; i < 100; i++ ) {
		game->highscore[i] = 0;
	}
	//Game menu init texture
	game->introbg = new IntroBG;
	game->pausedbg = new PausedBG;
	game->gameoverbg = new GameoverBG;
	game->highscorebg = new highscoreBG;
	game->buttonT = new ButtonTextures;
	//Website update score stff
	char *host = ( char* ) "sleipnir.cs.csub.edu";
	char *tpage = ( char* )
		"/~jhargreaves/upstream/lowScore.txt";
	//get lowest highscores into text
	getHighScore ( game, host, tpage,true,false );
	system ( "rm highscore.xml" );
	std::string command =
		"wget http://www.cs.csub.edu/~jhargreaves/upstream/highscore.xml";
	system ( ( command ).c_str() );
	//get highscore xml
	for ( int i=0; i<20; i++ ) {
		game->highScores[i] = loadScores ( i );
		std::cout<<"player#"<<i<<" = "<<game->highScores[i]<<std::endl;
	}
}

void demo ( Game *game )
{

	int moving = rand() %1000+1;
	int x =rand() %5+1;
	// move right
	if ( !game->frog->rocket() ) {
		if ( moving>5 && moving <10 && game->demo.moveRight==0
				&& game->demo.moveLeft==0 &&
				game->demo.jump ==0 &&
				game->frog->getYpos() <80 )
			game->demo.moveRight++;
		// move left
		if ( moving >10 && moving <15 && game->demo.moveRight==0
				&& game->demo.moveLeft==0 &&
				game->demo.jump ==0 &&
				game->frog->getYpos() <80 )
			game->demo.moveLeft++;
		// jump
		if ( moving >15 && moving <20 && game->demo.moveRight==0
				&& game->demo.moveLeft==0 && game->demo.jump ==0 )
			game->demo.jump++;
		// rocket
		if ( moving >20 && moving < 22 && game->demo.jump==0 &&
				game->demo.moveRight==0      &&
				game->demo.moveLeft==0 &&
				game->demo.jump ==0 &&
				game->frog->getXpos() <WIDTH-100 &&
				game->frog->getXpos() >100 &&
				game->main_menu == true ) {
			game->frog->addRocket();
			game->frog->toggleRocket();
		}
	}
	if ( game->demo.moveRight>0 ) {
		if ( game->frog->getXpos() <=WIDTH ) {
			if ( !game->main_menu ) {
				game->frog->move ( game->frog->getXpos()+3,
						game->frog->getYpos(),
						game->frog->getXvel(),
						game->frog->getYvel() );
			}
			game->c.center[0]+=x;
		}
		game->demo.moveRight++;
		game->c.isJumping = false;
		game->c.isStanding = true;
		if ( game->demo.moveRight>60 ) {
			game->demo.moveRight=0;
		}
	}
	if ( game->demo.moveLeft>0 ) {
		if ( game->frog->getXpos() >20 ) {
			if ( !game->main_menu ) {
				game->frog->move ( game->frog->getXpos()-2,
						game->frog->getYpos(),
						game->frog->getXvel(),
						game->frog->getYvel() );
			}
			game->c.center[0]-=x;
		}
		game->demo.moveLeft++;
		game->c.isJumping = false;
		game->c.isStanding = true;
		if ( game->demo.moveLeft > 60 ) {
			game->demo.moveLeft = 0;
		}
	}
	//std::cout<<game->frog->getXpos() <<" Y="<<game->frog->getYpos()
	//       <<" right="<<game->demo.moveLeft<<std::endl;
	if ( game->demo.jump >0 ) {
		game->demo.jump++;
		game->c.isJumping = true;
		game->c.isStanding = false;
		if ( game->demo.jump<=2 ) {
			game->c.velocity[1] = 15.0;
		}
		if ( !game->main_menu ) {
			game->frog->move ( game->frog->getXpos(),
					game->frog->getYpos() + game->c.velocity[1],
					game->frog->getXvel(),
					game->frog->getYvel() );
		}
		game->c.center[1] += game->c.velocity[1];
		game->c.velocity[0] = 0;
		game->c.velocity[1] -= 1;
		game->frog->move ( game->c.center[0],game->c.center[1]-0.8,
				game->c.velocity[0],game->c.velocity[1] );
		if ( game->demo.jump > 40 && game->c.center[1] < 50 ) {
			game->demo.jump = 0;
			game->c.center[1]=40;
			game->c.velocity[1]=0;
		}
	}
	if ( game->frog->getXpos() >= WIDTH ) {
		game->frog->move ( WIDTH-6,
				game->frog->getYpos(),
				game->frog->getXvel(),
				game->frog->getYvel() );
		game->demo.moveRight = 0;
		game->demo.jump = 0;
		game->demo.moveLeft++;
	}
	if ( game->frog->getXpos() <=5 ) {
		game->frog->move ( 5,
				game->frog->getYpos(),
				game->frog->getXvel(),
				game->frog->getYvel() );
		game->demo.moveLeft = 0;
		game->demo.jump = 0;
		game->demo.moveRight++;
	}
	if ( game->frog->getYpos() > HEIGHT ) {
		game->frog->move ( game->frog->getXpos(), HEIGHT-5,
				game->frog->getXvel(),
				game->frog->getYvel() );
	}
	if ( game->frog->getYpos() <= 10 ) {
		game->frog->move ( game->frog->getXpos(),10,
				game->frog->getXvel(),
				game->frog->getYvel() );
		game->demo.moveRight = 0;
		game->demo.jump = 0;
		game->demo.moveLeft =0;
	}
	game->c.center[0]=game->frog->getXpos();
	game->c.center[1]=game->frog->getYpos();
	if ( !game->main_menu ) {
		if ( game->frog->getMeter() )
			game->frog->meterOff();
	}
}
