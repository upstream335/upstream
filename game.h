#ifndef _GAME_
#define _GAME_

#include "johnH.h"
#include <string>
typedef double Vec[3];
struct Lilypad;
struct LilyTexture;
struct Score;
struct HighScoreBox;
struct ButtonTextures;
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

#define MAXBUTTONS 15
typedef struct t_button {
	Rect r;
	char text[32];
	int over;
	float color[3];
	unsigned int text_color;
} Button;

struct Demo {
	bool on;
	int moveLeft;
	int moveRight;
	int jump;
};

struct Bullet {
	Vec pos;
	Vec vel;
	float color[3];
	struct timespec time;
	struct Bullet *prev;
	struct Bullet *next;
	Bullet() {
		prev = NULL;
		next = NULL;
	}
};

#define EASY    1
#define MED     2
#define HARD    3

struct Game {
	int lilycount;
	int fps;
	int showfps;
	int lives;
	int gameover;
	bool bossGO;
	bool done;
	int difficulty; //1=easy,2=med,3=hard
	Demo demo;
	bool playing;
	bool isHighScore;
	bool rocketSound;
	int x;  // fly x position in game over
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
	bool showTaunt;
	int nlily;
	int timer;
	int maxtimer;
	int lilytimer;
	int lilyspawnpoint; //y coordinate of where lilies spawn
	int stresstest;
	int tempscore;
	int nbullets;
	Bullet *barr;
	struct timespec bulletTimer;
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
	Monster *monster;
	int swarmSize;
	Fly *swarm[20];
	bool swarmOn;
	Log *log[4];
	Water *water[3];
	WaterBG *waterbg;
	Bridge *bridge;
	Splash *splash;
	Explosion *explosion;
	Turtle *turtle;
	RocketPack *rocketPack;
	std::string highScores[20];

	//Game menu
	bool main_menu;
	bool sub_menu;
	bool gameover_menu;
	IntroBG *introbg;
	PausedBG *pausedbg;
	GameoverBG *gameoverbg;
	ButtonTextures *buttonT;
	bool intromuted;

	//highscore display
	bool highscoreboard;
	highscoreBG *highscorebg;

	//credits
	bool credits;

	Button button[MAXBUTTONS];
	int nbuttons;
	int count;

	//Help Menu
	bool help_menu;
	helpBG *helpbg;

	Game()
	{
		//game resolution
		windowWidth = 600;
		windowHeight = 760;

		done = false;
		c.isStanding = true;
		showTaunt = true;
		tempscore = 0;
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
		highscoreboard = false;
		credits = false;
		count = 2;
		// buttons count
		nbuttons = 0;
		intromuted = false;
	}
};

extern void init_game ( Game *game );
extern void demo ( Game *game );

#endif
