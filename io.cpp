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
			lbutton=0;
			if ( !game->c.isJumping ) {
				game->c.isJumping = true;
				game->c.isStanding = false;
				game->c.velocity[1] = 15.0;
				lbutton=1;
			}
		}
		if ( e->xbutton.button==3 ) {
			//game->windowHeight = 1024;
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
							//Sound
							game->c.isJumping = false;
							game->c.isStanding = false;
							playSounds ( "./wav/tick.wav",1.0f,
									false, game->muted );
							muteSounds(game);
							break;
						case 14:
							//Help
							playSounds ( "./wav/tick.wav",1.0f,
									false, game->muted );
							game->c.isJumping = false;
							game->c.isStanding = false;
							game->help_menu ^= true;
							break;
					}
				}
			}
		}
	}
}

int check_keys ( XEvent *e, Game *game )
{
	//Was there input from the keyboard?
	if ( e->type == KeyPress ) {
		int key = XLookupKeysym ( &e->xkey, 0 );
		switch ( key ) {
			case XK_p:
				game->sub_menu^=true;
				break;
			case XK_o:
				game->gameover^=true;
				break;
			case XK_f:
				game->showfps ^= 1;
				break;
			case XK_i:
				reset_game(game);
				break;
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
				break;
			case XK_t:
				game->demo.on^=1;
				break;
			case XK_Escape:
				game->sub_menu^=true;
				break;
			case XK_b:
				game->bossGO ^= true;
				break;
				return 1;
		}
	}
	return 0;
}
