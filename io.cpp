#include "io.h"

void check_mouse(XEvent *e, Game *game)
{
	static int savex = 0;
	static int savey = 0;
	static int n = 0;
	if (e->type == ButtonRelease) {
		return;
	}
	if (e->type == ButtonPress) {
		if (e->xbutton.button==1) {
			//Left button was pressed
			if (e->xbutton.x >= 465 && e->xbutton.x <= 500 &&
					e->xbutton.y >= 10 && e->xbutton.y <= 30) {
				printf("Sounds Muted\n");
				game->muted ^= 1;
				if (game->muted)
					stopMusic();
				else
					playMusic();
			}
			if (!game->c.isJumping) {
				game->c.isJumping = true;
				game->c.isStanding = false;
				game->c.velocity[1] = 15.0;
			}
			return;
		}
		if (e->xbutton.button==3) {
			//Right button was pressed
			return;
		}
	}
	//Did the mouse move?
	if (savex != e->xbutton.x || savey != e->xbutton.y) {
		savex = e->xbutton.x;
		savey = game->windowHeight - e->xbutton.y;
		if (++n < 10)
			return;
		game->c.newPosX = savex;
		game->c.newPosY = savey;
	}
}

int check_keys(XEvent *e, Game *game)
{
	//Was there input from the keyboard?
	if (e->type == KeyPress) {
		int key = XLookupKeysym(&e->xkey, 0);
		switch (key) {
			case XK_f:
				break;
			case XK_b:
				break;
			case XK_m:
				return game->menu_done=1;
				break;
            case XK_d:
                game->difficulty++;
                if (game->difficulty>3)
                    game->difficulty=EASY;
				break;
			case XK_r:
                game->frog->toggleRocket();
                game->frog->setFrame(0);
				break;
			case XK_Escape:
				break;
				return 1;
		}
	}
	return 0;
}
