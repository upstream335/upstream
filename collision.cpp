#include "collision.h"
#include <cmath>

void collision ( Game *game )
{
	if ( !game->demo.on ) {
		float gravity = 0.3*game->difficulty;
		if ( game->c.isJumping ) {
			game->c.center[1] += game->c.velocity[1];
			game->c.velocity[0] = 0;
			game->c.velocity[1] -= gravity;
			if ( game->c.center[1] <= 0 ) {
				game->c.isJumping = false;
				game->c.isStanding = true;
				game->c.center[1] = 15;
			}
		}
		if ( game->c.center[0] == game->c.newPosX &&
				game->c.center[1] == game->c.newPosY )
			return;
		if ( game->c.center[0] < game->c.newPosX ) {
			if ( game->c.isJumping ) {
				game->c.center[0] += 20.0;
			} else {
				game->c.center[0] += 10.0;
				if ( game->c.velocity[0]<=0 )
					game->c.velocity[0]+=0.2;
			}
			if ( game->c.center[0] > game->c.newPosX )
				game->c.center[0] = game->c.newPosX;
		} else if ( game->c.center[0] > game->c.newPosX ) {
			if ( game->c.isJumping ) {
				game->c.center[0] -= 15.0;
			} else {
				game->c.center[0] -= 10.0;
				if ( game->c.velocity[0]>=0 )
					game->c.velocity[0] -=0.2;
			}
			if ( game->c.center[0] < game->c.newPosX )
				game->c.center[0] = game->c.newPosX;
		}
	}
	//check for frog(ball)'s collision with lilies
	Lilypad *node = game->ihead;
	if ( game->c.isStanding == false ) {
		while ( node ) {
			float d1 = node->pos[1] - game->c.center[1];
			float d2 = node->pos[0] - game->c.center[0];
			float dist = sqrt ( d1*d1 + d2*d2 );
			if ( dist <= game->c.radius+node->size+15/game->difficulty ) {
				//rejump the ball
				game->c.isJumping = true;
				game->c.velocity[1] = 15.0;
				playSounds ( "./wav/boing2.wav",1.0f, false,game->muted );
				deleteLily ( node,game );
				//basic scoring for now -Kevin
				game->score+=10;
			}
			node = node->next;
		}
	}
	// ========================================
	// collision frog with log
	for ( int i=0; i<4; i++ ) {
		if ( game->frog->getXpos() <= game->log[i]->getXpos()+15 &&
				game->frog->getXpos() >= game->log[i]->getXpos()-15 &&
				game->frog->getYpos() <= game->log[i]->getYpos()+50 &&
				game->frog->getYpos() >= game->log[i]->getYpos()-50 ) {
			game->c.velocity[1]=game->log[i]->getYvel()+15;
			playSounds ( "./wav/tick.wav",1.0f, false, game->muted );
			game->score+=2;
		}
	}
	// FLY =====================================================
	int tongue=60/game->difficulty;
	if ( game->frog->getXpos() >= game->fly->getXpos()- tongue &&
			game->frog->getXpos() <= game->fly->getXpos()+ tongue &&
			game->frog->getYpos() <= game->fly->getYpos()+ tongue &&
			game->frog->getYpos() >= game->fly->getYpos()- tongue ) {
		playSounds ( "./wav/tick.wav",1.0f, false, game->muted );
		game->score+=50;
		game->fly->death ( game->frog->getXpos(),game->frog->getYpos() );
		//std::cout<<"dead fly"<<std::endl;
	}
	// pickup rocketPack =====================================================
	int reach=50/game->difficulty;
	if ( game->frog->getXpos() >= game->rocketPack->getXpos()- reach &&
			game->frog->getXpos() <= game->rocketPack->getXpos()+ reach &&
			game->frog->getYpos() <= game->rocketPack->getYpos()+ reach &&
			game->frog->getYpos() >= game->rocketPack->getYpos()- reach ) {
		playSounds ( "./wav/tick.wav",1.0f, false, game->muted );
		game->score+=10;
		game->frog->addRocket();
		game->rocketPack->setFrame ( 500 );
		//std::cout<<"dead rocketPack"<<std::endl;
	}
	if ( game->frog->rocket() && game->frog->getFrame()==1) {
		playSounds ( "./wav/rocket.wav",0.5f, false,game->muted );
	}

	// collision frog with gator head
	int head = 5 * game->difficulty;
	if ( game->frog->getXpos() <= game->gator->getXpos()- ( 5 + head )  &&
			game->frog->getXpos() >= game->gator->getXpos()- ( 30 + head ) &&
			game->frog->getYpos() <= game->gator->getYpos()+ ( 5 + head ) &&
			game->frog->getYpos() >= game->gator->getYpos()- ( 5 + head ) ) {
		game->gator->eat();
		gameOver ( game );
	}
	// collision frog with gator back
	int back = 90/ game->difficulty;
	if ( game->frog->getXpos() <= game->gator->getXpos()+ back &&
			game->frog->getXpos() >= game->gator->getXpos()- ( back-head ) &&
			game->frog->getYpos() <= game->gator->getYpos()+ back/2 &&
			game->frog->getYpos() >= game->gator->getYpos()-back/2 ) {
		game->c.isJumping = true;
		game->c.velocity[1] = 15.0;
		playSounds ( "./wav/boing2.wav",1.0f, false,game->muted );
		game->score+=30;
		game->gator->dive();
	}
	// collision gator with log
	for ( int i=0; i<4; i++ ) {
		if ( game->gator->getXpos() <= game->log[i]->getXpos()+80 &&
				game->gator->getXpos() >= game->log[i]->getXpos()-20 &&
				game->gator->getYpos() >= game->log[i]->getYpos()-50 &&
				game->gator->getYpos() <= game->log[i]->getYpos()+50 ) {
			game->gator->move ( game->gator->getXpos()+45,
					game->gator->getYpos()+15,
					game->gator->getXvel(),game->gator->getYvel() );
		}
	}
	//fell down
	if ( game->frog->getYpos() <=40 && game->bridge->getYpos() <=100 ) {
		gameOver ( game );
	}

	//frog at bottom
	if ( game->frog->getYpos() <= 40.0 ) {
		game->highscore[++game->scoreCount] = game->score;
		game->score = 0;
	}
}

void screenUpdate ( Game *game )
{
	//std::cout<<game->splash->getFrame()<<std::endl;
	if ( game->frog->getYpos() > game->windowHeight/2 ) {
		int move_down = 10;
		if ( game->frog->getYpos() > ( game->windowHeight*2/3 ) ) {
			move_down = 30;
		}
		game->c.center[1]-= move_down;
		game->frog->move ( game->c.center[0],game->c.center[1]-move_down,
				game->frog->getXvel(),
				game->frog->getYvel() );
		game->water[0]->move ( game->water[0]->getXpos(),
				game->water[0]->getYpos()-move_down,
				game->water[0]->getXvel(),
				game->water[0]->getYvel() );
		game->water[1]->move ( game->water[1]->getXpos(),
				game->water[1]->getYpos()-move_down,
				game->water[1]->getXvel(),
				game->water[1]->getYvel() );
		game->water[2]->move ( game->water[2]->getXpos(),
				game->water[2]->getYpos()-move_down,
				game->water[2]->getXvel(),
				game->water[2]->getYvel() );
		game->gator->move ( game->gator->getXpos(),
				game->gator->getYpos()-move_down,
				game->gator->getXvel(),
				game->gator->getYvel() );
		game->fly->move (game->fly->getXpos(),
				game->fly->getYpos()-move_down,
				game->fly->getXvel(),
				game->fly->getYvel() );
		game->turtle->move (game->turtle->getXpos(),
				game->turtle->getYpos()-move_down,
				game->turtle->getXvel(),
				game->turtle->getYvel() );
		for ( int i=0; i<4; i++ ) {
			game->log[i]->move ( game->log[i]->getXpos(),
					game->log[i]->getYpos()-move_down,
					game->log[i]->getXvel(),
					game->log[i]->getYvel() );
		}
		game->bridge->move ( game->bridge->getXpos(),
				game->bridge->getYpos()-move_down,
				game->bridge->getXvel(),
				game->bridge->getYvel() );
		//shift lilies
		Lilypad *node = game->ihead;
		//adjust spawn timer to spawn faster as we go up
		game->lilytimer = 10;
		while ( node ) {
			node->pos[1] -= move_down;
			node = node->next;
		}
	} else {
		//reset spawn timer to original if below half screen
		game->lilytimer = game->maxtimer;
	}
}

void gameOver ( Game *game )
{
    if (checkHighScore(game,game->score)) {
        game->tempscore = game->score;
    }

    if (!game->hschecked) {
        game->hschecked = true;
        initHighScore(game);
    }
	// move splash on screen if offscreen
	if ( game->splash->getXpos() <0 ) {
		playSounds ( "./wav/fishsplash.wav",1.0f, false,game->muted );
		game->splash->move ( game->frog->getXpos(),
				game->frog->getYpos()+40,0,0 );
		//sendScoresToPHP(game->score, game->difficulty);
	}
	//move frog,logs and gator offscreen
	game->frog->move ( -200,-200,0,0 );
	for ( int i=0; i<4; i++ ) {
		game->log[i]->move ( -200*i,-300,0,0 );
	}
	//game->gator->move ( -200,-200,0,0 );
	// reset lilies
	clearLilies ( game );
	//wait for splash to complete
	if ( game->splash->getFrame() >=195 ) {
        cout << game->tempscore << endl;

		//move bridge back
		game->bridge->move ( 300,150,0,0 );
		game->lives--;
		for ( int i=0; i<4; i++ ) {
			game->log[i]->move ( 50*i,-100*i,-.15*i,-1 );
		}
		game->gator->move ( -300,400,-2,-.5 );
		game->frog->move ( -100,-800,0,0 );
		game->frog->setYpos ( game->frog->getYpos()-10 );
		game->frog->setXpos ( game->frog->getXpos()-800 );
		game->splash->move ( -200,-200,0,0 );
		game->gameover = true;
		game->hschecked = false;
	} else {
        if (checkHighScore(game,game->tempscore)) {
			cout << "isHighScore = true" << endl;
			game->isHighScore = true;
		} else {
			game->isHighScore = false;
		}
	}
}
