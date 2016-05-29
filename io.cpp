#include "io.h"
#include "quyN.h"

void check_mouse ( XEvent *e, Game *game )
{
	static int savex = 0;
	static int savey = 0;
	int i,x,y;
	int lbutton=0;
	static int n = 0;
	if ( e->type == ButtonRelease ) {
		return;
	}
	if ( e->type == ButtonPress ) {
		if ( e->xbutton.button==1 ) {
			//Left button was pressed
			if ( !game->c.isJumping ) {
				game->c.isJumping = true;
				game->c.isStanding = false;
				game->c.velocity[1] = 15.0;
			}
			lbutton=1;
		}
		if ( e->xbutton.button==3 ) {
			//Right button was pressed
			game->rocketSound ^= 1;
			game->frog->toggleRocket();
			game->frog->setFrame ( 0 );
		}
	}
	x = e->xbutton.x;
	y = e->xbutton.y;
	//x = game->windowWidth - x;
	y = game->windowHeight - y;
	//Did the mouse move?
	if ( savex != e->xbutton.x || savey != e->xbutton.y ) {
		savex = e->xbutton.x;
		savey = game->windowHeight - e->xbutton.y;
		if ( ++n < 10 )
			return;
		game->c.newPosX = savex;
		game->c.newPosY = savey;
	}
	for (i=0; i<game->nbuttons; i++) {
		game->button[i].over=0;
		if (x >= game->button[i].r.left &&
				x <= game->button[i].r.right &&
				y >= game->button[i].r.bot &&
				y <= game->button[i].r.top) {
			game->button[i].over=1;
			if (game->button[i].over) {
				if (lbutton) {
					switch (i) {
						case 13:
						        game->c.isJumping = false;
							game->c.isStanding = true;
							//Sound
							muteSounds(game);
							break;
						case 14:
							//Help
							game->help_menu ^= true;
							break;
					}
				}
			}
		}
	}
}

void check_menu_mouse ( XEvent *e, Game *game )
{
	static int savex = 0;
	static int savey = 0;
	int i,x,y;
	int lbutton=0;
	//int rbutton=0;
	//
	if (e->type == ButtonRelease)
		return;
	if (e->type == ButtonPress) {
		if (e->xbutton.button==1) {
			//Left button is down
			lbutton=1;
		}
		if (e->xbutton.button==3) {
			//Right button is down
			//rbutton=1;
			//if (rbutton){}
		}
	}
	x = e->xbutton.x;
	y = e->xbutton.y;
	y = game->windowHeight - y;
	if (savex != e->xbutton.x || savey != e->xbutton.y) {
		//Mouse moved
		savex = e->xbutton.x;
		savey = e->xbutton.y;
	}
	for (i=0; i<game->nbuttons; i++) {
		game->button[i].over=0;
		if (x >= game->button[i].r.left &&
				x <= game->button[i].r.right &&
				y >= game->button[i].r.bot &&
				y <= game->button[i].r.top) {
			game->button[i].over=1;
			if (game->button[i].over) {
				if (lbutton) {
					switch (i) {
						case 0:
							//Play
							game->main_menu^=true;
							reset_game(game);
							break;
						case 1:
							//Difficulty
							game->difficulty++;
							if (game->difficulty > 3)
								game->difficulty = EASY;
							game->count++;
							if (game->count == 1) {
								strcpy(game->button[1].text, "EASY");
								std::cout << "easy mode ON" << std::endl;
							}
							if (game->count == 2) {
								strcpy(game->button[1].text, "MEDIUM");
								std::cout << "medium mode ON" << std::endl;
							}
							if (game->count == 3) {
								strcpy(game->button[1].text, "HARD");
								std::cout << "hard mode ON" << std::endl;
								game->count = 0;
							}
							break;
						case 2:
							//High Scores
							game->credits=false;
							game->highscoreboard^=true;	
							break;
						case 3:
							//Credits
							game->highscoreboard=false;
							game->credits^=true;
							break;
						case 4:
							//Exit Game
							game->done^=true;
							game->main_menu^=true;
							break;
					}
				}
			}
		}
	}
	return;
}

void check_paused_mouse ( XEvent *e, Game *game )
{
	static int savex = 0;
	static int savey = 0;
	int i,x,y;
	int lbutton=0;
	int rbutton=0;
	//
	if (e->type == ButtonRelease)
		return;
	if (e->type == ButtonPress) {
		if (e->xbutton.button==1) {
			//Left button is down
			lbutton=1;
		}
		if (e->xbutton.button==3) {
			//Right button is down
			rbutton=1;
			if (rbutton){}
		}
	}
	x = e->xbutton.x;
	y = e->xbutton.y;
	y = game->windowHeight - y;
	if (savex != e->xbutton.x || savey != e->xbutton.y) {
		//Mouse moved
		savex = e->xbutton.x;
		savey = e->xbutton.y;
	}
	for (i=0; i<game->nbuttons; i++) {
		game->button[i].over=0;
		if (x >= game->button[i].r.left &&
				x <= game->button[i].r.right &&
				y >= game->button[i].r.bot &&
				y <= game->button[i].r.top) {
			game->button[i].over=1;
			if (game->button[i].over) {
				if (lbutton) {
					switch (i) {
						case 5:
							//Resmue
							game->sub_menu^=true;
							break;
						case 6:
							//Sound
							muteSounds(game);
							break;
						case 7:
							//restart game
							game->sub_menu^=true;
							reset_game(game);
							break;
						case 8:
							//main menu
							game->sub_menu^=true;
							game->main_menu^=true;
							reset_game(game);
							break;
						case 9:
							//Exit Game
							game->done^=true;
							game->sub_menu^=true;
							break;
					}
				}
			}
		}
	}
	return;
}

void check_gameover_mouse ( XEvent *e, Game *game )
{
	static int savex = 0;
	static int savey = 0;
	int i,x,y;
	int lbutton=0;
	int rbutton=0;
	//
	if (e->type == ButtonRelease)

		return;
	if (e->type == ButtonPress) {
		if (e->xbutton.button==1) {
			//Left button is down
			lbutton=1;
		}
		if (e->xbutton.button==3) {
			//Right button is down
			rbutton=1;
			if (rbutton){}
		}
	}
	x = e->xbutton.x;
	y = e->xbutton.y;
	y = game->windowHeight - y;
	if (savex != e->xbutton.x || savey != e->xbutton.y) {
		//Mouse moved
		savex = e->xbutton.x;
		savey = e->xbutton.y;
	}
	for (i=0; i<game->nbuttons; i++) {
		game->button[i].over=0;
		if (x >= game->button[i].r.left &&
				x <= game->button[i].r.right &&
				y >= game->button[i].r.bot &&
				y <= game->button[i].r.top) {
			game->button[i].over=1;
			if (game->button[i].over) {
				if (lbutton) {
					switch (i) {
						case 10:
							//Play again
							game->gameover^=true;
							break;
						case 11:
							//Main menu
							game->gameover^=true;
							game->main_menu^=true;
							break;
						case 12:
							//Exit
							game->done^=true;
							game->gameover^=true;
							break;
					}
				}
			}
		}
	}
	return;
}

int check_keys ( XEvent *e, Game *game )
{
	//Was there input from the keyboard?
	if ( e->type == KeyPress ) {
		int key = XLookupKeysym ( &e->xkey, 0 );
		switch ( key ) {
			case XK_j:
				game->troll_lilypad ^= 1;
				break;
			case XK_h:
				game->help_menu ^= true;
				break;
			case XK_k:
				game->stresstest ^= 1;
				break;
			case XK_s:
				game->swarmOn ^= 1;
				break;
			case XK_r:
				game->raveMode ^= 1;
				break;
			case XK_t:
				game->demo.on^=1;
				break;
			case XK_Escape:
				game->sub_menu^=true;
				break;
			case XK_space:
				spawnBullet(game);
				break;
				return 1;
		}
	}
	return 0;
}
