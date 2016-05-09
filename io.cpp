#include "io.h"
#include "quyN.h"

void check_mouse ( XEvent *e, Game *game )
{
	static int savex = 0;
	static int savey = 0;
	static int n = 0;
	if ( e->type == ButtonRelease ) {
		return;
	}
	if ( e->type == ButtonPress ) {
		if ( e->xbutton.button==1 ) {
			//Left button was pressed
			if ( e->xbutton.x >= 465 && e->xbutton.x <= 500 &&
					e->xbutton.y >= 10 && e->xbutton.y <= 30 ) {
				printf ( "Sounds Muted\n" );
				game->muted ^= 1;
				if ( game->muted )
					stopMusic();
				else
					playMusic();
			}
			if ( !game->c.isJumping ) {
				game->c.isJumping = true;
				game->c.isStanding = false;
				game->c.velocity[1] = 15.0;
			}
			return;
		}
		if ( e->xbutton.button==3 ) {
			//Right button was pressed
		}
	}
	//Did the mouse move?
	if ( savex != e->xbutton.x || savey != e->xbutton.y ) {
		savex = e->xbutton.x;
		savey = game->windowHeight - e->xbutton.y;
		if ( ++n < 10 )
			return;
		game->c.newPosX = savex;
		game->c.newPosY = savey;
	}
}

void check_menu_mouse ( XEvent *e, Game *game )
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
					switch(i) {
						case 0:
							game->main_menu^=true;
							break;
						case 1:
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

void check_continue_mouse ( XEvent *e, Game *game )
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
					switch(i) {
						case 2:
							game->sub_menu^=true;
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
			case XK_m:
				game->main_menu^=true;
				break;
			case XK_p:
				game->sub_menu^=true;
			case XK_o:
				reset_game(game);
				break;
			case XK_j:
				game->troll_lilypad ^= 1;
				break;
			case XK_k:
				game->stresstest ^= 1;
				break;
			case XK_d:
				game->difficulty++;
				if ( game->difficulty>3 )
					game->difficulty=EASY;
				break;
			case XK_r:
				game->frog->toggleRocket();
				game->frog->setFrame ( 0 );
				break;
			case XK_t:
				game->demo.on^=1;
				break;
			case XK_Escape:
				break;
				return 1;
		}
	}
	return 0;
}
