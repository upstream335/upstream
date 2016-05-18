#ifndef _GAME_
#define _GAME_

#include "johnH.h"
#include <string>
typedef double Vec[3];
struct Lilypad;
struct LilyTexture;
struct Score;
struct HighScoreBox;
struct Circle {
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

#define MAXBUTTONS 13
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

struct Demo {
	bool on;
	int moveLeft;
	int moveRight;
	int jump;
};

#define EASY    1
#define MED     2
#define HARD    3

struct Game {
	int lives;
	int gameover;
	bool done;
	int difficulty; //1=easy,2=med,3=hard
	Demo demo;
	bool playing;
	bool isHighScore;
	int troll_lilypad;
	int windowWidth;
	int windowHeight;
	int xres;
	int yres;
	int score;
	int highscore[100];
	int scoreCount;
	bool muted ;
	bool hschecked;
	int nlily;
	int timer;
	int maxtimer;
	int lilytimer;
	int lilyspawnpoint; //y coordinate of where lilies spawn
	int stresstest;
	int tempscore;
	char playername[256];
	char hsname[256];
	Lilypad *ihead;
	Score *hscore;
	LilyTexture *lily;
	HighScoreBox *hscorebox;
	Circle c;
	int n;
	HUD *hud;
	Meter *meter;
	Frog *frog;
	Gator *gator;
	Fly *fly;
	Log *log[4];
	Water *water[3];
	Bridge *bridge;
	Splash *splash;
	Turtle *turtle;
	RocketPack *rocketPack;

	//Game menu
	bool main_menu;
	bool sub_menu;
	bool gameover_menu;
	IntroBG *introbg;
	PausedBG *pausedbg;

	//Ppmimage *introbgImage;
	//GLuint introbgTexture;
	Button button[MAXBUTTONS];
	int nbuttons;
	//
	//ALuint alBufferDrip, alBufferTick;
	//ALuint alSourceDrip, alSourceTick;

	Game()
	{
		done = false;
		c.isStanding = true;
		tempscore = 0;
		windowWidth = 600;
		windowHeight = 760;
		troll_lilypad = 0;
		stresstest = 0;
        isHighScore = false;
        playername[0] = '\0';
        hsname[0] = '|';
		//init for game menu
		main_menu = false;
		sub_menu = true;
		gameover_menu = false;
		gameover = false;
		// buttons
		nbuttons = 0;
		//introbgImage=NULL;
	}
};

extern void init_game ( Game *game );
extern void demo ( Game *game );


#endif
