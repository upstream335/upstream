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

#define MAXBUTTONS 4
typedef struct t_button {
	Rect r;
	char text[32];
	int over;
	int down;
	int click;
	float color[3];
	float dcolor[3];
	unsigned int text_color;
} Button;

struct Game
{
	bool playing;
	int windowWidth;
	int windowHeight;
	int xres;
	int yres;
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
	Fly     *fly;
	//for game menu
	int menu_done;
	int boardDim;
	int gridDim;
	int gameover;
	int nbuttons;
	Ppmimage *bgImage;
	GLuint bgTexture;
	Button button[MAXBUTTONS];

	Log *log[4];
	Water *water[3];
	Bridge *bridge;
	Splash *splash;
	Game()
	{
		c.isStanding = true;
		windowWidth = 600;
		windowHeight = 1024;

		//init for game menu
		xres = 200;
		yres = 200;
		menu_done = 0;
		gridDim = 40;
		gameover = 0;
		nbuttons = 0;
		bgImage=NULL;
	}
};

extern void init_game(Game *game);

#endif
