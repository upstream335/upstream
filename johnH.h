//cs335 john hargreaves
//animation classes for
//frog,log,gator,bridge
//splash,fly and background water
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "log.h"
#include "ppm.h"
#include "fonts.h"
#include "tinyxml2.h"



//defined types
#define HEIGHT 700
#define WIDTH  600

struct Position {
	int x_pos;
	int y_pos;
	int x_vel;
	int y_vel;
	int frame;
};

extern Position update_position ( Position *c,float,float,float,float );
extern unsigned char *buildAlphaData ( Ppmimage *img );
extern Ppmimage*  get_image ( std::string filename );
extern std::string loadScores ( int player );

class helpBG
{
private:
	Position current;
	Position previous;
	Ppmimage *helpbgImage;
	GLuint helpbgTexture;
public:
// Constructor with default values for data members
	helpBG()
	{
		current.frame = 0;
		current.x_pos = WIDTH;
		current.y_pos = HEIGHT;
		current.x_vel = 0;
		current.y_vel = -1;
		previous = current;
		helpbgImage = get_image ( "./images/helpbg" );
//create opengl texture elements
		glGenTextures ( 1, &helpbgTexture );
		int w = helpbgImage->width;
		int h = helpbgImage->height;
//
		glBindTexture ( GL_TEXTURE_2D, helpbgTexture );
		glTexParameteri ( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST );
		glTexParameteri ( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST );
		unsigned char *helpbgData = buildAlphaData ( helpbgImage );
		glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		               GL_RGBA, GL_UNSIGNED_BYTE, helpbgData );
		free ( helpbgData );
	} //end constructor
//--------------------------------------------------------------------
	void render ( void );
	float getXpos()
	{
		return current.x_pos;
	}
	float getYpos()
	{
		return current.y_pos;
	}
	float getXvel()
	{
		return current.x_vel;
	}
	float getYvel()
	{
		return current.y_vel;
	}
	void move ( float xp, float yp, float xv, float yv )
	{
		current = update_position ( &current,xp,yp,xv,yv );
	}
};

class HUD
{
private:
	Position current;
	Position previous;
	Ppmimage *hudImage[2];
	GLuint hudTexture[2];
public:
	// Constructor with default values for data members
	HUD()
	{
		current.frame = 0;
		current.x_pos = WIDTH/2-40;
		current.y_pos = 510;
		current.x_vel = 0;
		current.y_vel = 0;
		previous = current;
		hudImage[0] = get_image ( "./images/hud" );
		int i=0;
		//create opengl texture elements
		glGenTextures ( 1, &hudTexture[i] );
		int w = hudImage[i]->width;
		int h = hudImage[i]->height;
		//
		glBindTexture ( GL_TEXTURE_2D, hudTexture[i] );
		glTexParameteri ( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST );
		glTexParameteri ( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST );
		unsigned char *hudData = buildAlphaData ( hudImage[i] );
		glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
						GL_RGBA, GL_UNSIGNED_BYTE, hudData );
		free ( hudData );
	} //end constructor
	void render ( void );
};


class IntroBG
{
private:
	Position current;
	Position previous;
	Ppmimage *introbgImage[2];
	GLuint introbgTexture[2];
public:
	// Constructor with default values for data members
	IntroBG()
	{
		current.frame = 0;
		current.x_pos = WIDTH;
		current.y_pos = HEIGHT;
		current.x_vel = 0;
		current.y_vel = -1;
		previous = current;
		introbgImage[0] = get_image ( "./images/introbg" );
		int i=0;
		//create opengl texture elements
		glGenTextures ( 1, &introbgTexture[i] );
		int w = introbgImage[i]->width;
		int h = introbgImage[i]->height;
		//
		glBindTexture ( GL_TEXTURE_2D, introbgTexture[i] );
		glTexParameteri ( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST );
		glTexParameteri ( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST );
		unsigned char *introbgData = buildAlphaData ( introbgImage[i] );
		glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
						GL_RGBA, GL_UNSIGNED_BYTE, introbgData );
		free ( introbgData );
	} //end constructor
	//--------------------------------------------------------------------
	void render ( void );
	float getXpos()
	{
		return current.x_pos;
	}
	float getYpos()
	{
		return current.y_pos;
	}
	float getXvel()
	{
		return current.x_vel;
	}
	float getYvel()
	{
		return current.y_vel;
	}
	void move ( float xp, float yp, float xv, float yv )
	{
		current = update_position ( &current,xp,yp,xv,yv );
	}
};

class PausedBG
{
private:
	Position current;
	Position previous;
	Ppmimage *pausedbgImage;
	GLuint pausedbgTexture;
public:
	// Constructor with default values for data members
	PausedBG()
	{
		current.frame = 0;
		current.x_pos = WIDTH;
		current.y_pos = HEIGHT;
		current.x_vel = 0;
		current.y_vel = -1;
		previous = current;
		pausedbgImage = get_image ( "./images/pausedbg" );
		//create opengl texture elements
		glGenTextures ( 1, &pausedbgTexture );
		int w = pausedbgImage->width;
		int h = pausedbgImage->height;
		//
		glBindTexture ( GL_TEXTURE_2D, pausedbgTexture );
		glTexParameteri ( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST );
		glTexParameteri ( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST );
		unsigned char *pausedbgData = buildAlphaData ( pausedbgImage );
		glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
						GL_RGBA, GL_UNSIGNED_BYTE, pausedbgData );
		free ( pausedbgData );
	} //end constructor
	//--------------------------------------------------------------------
	void render ( void );
	float getXpos()
	{
		return current.x_pos;
	}
	float getYpos()
	{
		return current.y_pos;
	}
	float getXvel()
	{
		return current.x_vel;
	}
	float getYvel()
	{
		return current.y_vel;
	}
	void move ( float xp, float yp, float xv, float yv )
	{
		current = update_position ( &current,xp,yp,xv,yv );
	}
};

class GameoverBG
{
private:
	Position current;
	Position previous;
	Ppmimage *gameoverImage;
	GLuint gameoverTexture;
public:
	// Constructor with default values for data members
	GameoverBG()
	{
		current.frame = 0;
		current.x_pos = WIDTH;
		current.y_pos = HEIGHT;
		current.x_vel = 0;
		current.y_vel = -1;
		previous = current;
		gameoverImage = get_image ( "./images/gameoverbg" );
		//create opengl texture elements
		glGenTextures ( 1, &gameoverTexture );
		int w = gameoverImage->width;
		int h = gameoverImage->height;
		//
		glBindTexture ( GL_TEXTURE_2D, gameoverTexture );
		glTexParameteri ( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST );
		glTexParameteri ( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST );
		unsigned char *gameoverData = buildAlphaData ( gameoverImage );
		glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
						GL_RGBA, GL_UNSIGNED_BYTE, gameoverData );
		free ( gameoverData );
	} //end constructor
	//--------------------------------------------------------------------
	void render ( void );
	float getXpos()
	{
		return current.x_pos;
	}
	float getYpos()
	{
		return current.y_pos;
	}
	float getXvel()
	{
		return current.x_vel;
	}
	float getYvel()
	{
		return current.y_vel;
	}
	void move ( float xp, float yp, float xv, float yv )
	{
		current = update_position ( &current,xp,yp,xv,yv );
	}
};

class highscoreBG
{
private:
	Position current;
	Position previous;
	Ppmimage *highscorebgImage;
	GLuint highscorebgTexture;
public:
	highscoreBG()
	{
		current.frame = 0;
		current.x_pos = WIDTH;
		current.y_pos = HEIGHT;
		current.x_vel = 0;
		current.y_vel = -1;
		previous = current;
		highscorebgImage = get_image ( "./images/highscorebg" );

		glGenTextures ( 1, &highscorebgTexture );
		int w = highscorebgImage->width;
		int h = highscorebgImage->height;

		glBindTexture ( GL_TEXTURE_2D, highscorebgTexture );
		glTexParameteri ( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST );
		glTexParameteri ( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST );
		unsigned char *highscorebgData = buildAlphaData ( highscorebgImage );
		glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		               GL_RGBA, GL_UNSIGNED_BYTE, highscorebgData );
		free ( highscorebgData );
	}
//--------------------------------------------------------------------
	void render ( void );
	float getXpos()
	{
		return current.x_pos;
	}
	float getYpos()
	{
		return current.y_pos;
	}
	float getXvel()
	{
		return current.x_vel;
	}
	float getYvel()
	{
		return current.y_vel;
	}
	void move ( float xp, float yp, float xv, float yv )
	{
		current = update_position ( &current,xp,yp,xv,yv );
	}
};
// =========================================================
class Monster
{
	private:
		Position current;
		Position previous;
		Ppmimage *monsterImage[7];
		GLuint monsterTexture[7];

	public:
		// Constructor with default values for data members
		Monster()
		{
			current.frame = 0;
			current.x_pos = 0;
			current.y_pos = HEIGHT - 150;
			current.x_vel = 0;
			current.y_vel = 0;
			previous = current;
		monsterImage[0] = get_image ( "./images/monster" );
		monsterImage[1] = get_image ( "./images/monster1" );
		monsterImage[2] = get_image ( "./images/monster2" );
		monsterImage[3] = get_image ( "./images/monster3" );
		monsterImage[4] = get_image ( "./images/monster4" );
		monsterImage[5] = get_image ( "./images/monster5" );
		monsterImage[6] = get_image ( "./images/monster6" );
		for ( int i =0; i<7; i++ ) {
			//create opengl texture elements
			glGenTextures ( 1, &monsterTexture[i] );
			int w = monsterImage[i]->width;
			int h = monsterImage[i]->height;
			//
			glBindTexture ( GL_TEXTURE_2D, monsterTexture[i] );
			glTexParameteri ( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,
					GL_NEAREST );
			glTexParameteri ( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,
					GL_NEAREST );
			unsigned char *monsterData = buildAlphaData ( monsterImage[i] );
			glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
					GL_RGBA, GL_UNSIGNED_BYTE, monsterData );
			free ( monsterData);
			}
		} //end constructor
		//--------------------------------------------------------------------
		float getFrame()
		{
			return current.frame;
		}
		void setFrame ( float x )
		{
			current.frame=x;
		}
		void render ( void );
		float getXpos()
		{
			return current.x_pos;
		}
		float getYpos()
		{
			return current.y_pos;
		}
		float getXvel()
		{
			return current.x_vel;
		}
		float getYvel()
		{
			return current.y_vel;
		}
		void move ( float xp, float yp, float xv, float yv )
		{
			current = update_position ( &current,xp,yp,xv,yv );
			current.frame=0;
		}
};


// =========================================================
class Frog
{
private:
	Position current;
	Position previous;
	bool isStanding;
	Ppmimage *frogImage[18];
	GLuint frogTexture[18];
	bool rocketFrog;
	int numberRockets;
	int wink;
	int blink;
	int left;
	int right;
	bool meter;

public:
	// Constructor with default values for data members
	Frog()
	{
		meter = true;
		rocketFrog = false;
		isStanding = true;
		current.frame = 0;
		current.x_pos = 300;
		current.y_pos =150;
		current.x_vel = 0;
		current.y_vel = -1;
		numberRockets = 1;
		wink = 0;
		blink = 0;
		left = 0;
		right = 0;
		previous = current;
		frogImage[0] = get_image ( "./images/frog" );
		frogImage[1] = get_image ( "./images/frog1" );
		frogImage[2] = get_image ( "./images/frog2" );
		frogImage[3] = get_image ( "./images/frog3" );
		frogImage[4] = get_image ( "./images/frog4" );
		frogImage[5] = get_image ( "./images/frog5" );
		frogImage[6] = get_image ( "./images/frog6" );
		frogImage[7] = get_image ( "./images/frog7" );
		frogImage[8] = get_image ( "./images/frog8" );
		frogImage[9] = get_image ( "./images/frog9" );
		frogImage[10] = get_image ( "./images/frog10" );
		frogImage[11] = get_image ( "./images/rocket" );
		frogImage[12] = get_image ( "./images/rocket1" );
		frogImage[13] = get_image ( "./images/rocket2" );
		frogImage[14] = get_image ( "./images/rocket3" );
		frogImage[15] = get_image ( "./images/rocket4" );
		frogImage[16] = get_image ( "./images/frog_wink" );
		frogImage[17] = get_image ( "./images/frog_blink" );
		for ( int i =0; i<=17; i++ ) {
			//create opengl texture elements
			glGenTextures ( 1, &frogTexture[i] );
			int w = frogImage[i]->width;
			int h = frogImage[i]->height;
			//
			glBindTexture ( GL_TEXTURE_2D, frogTexture[i] );
			glTexParameteri ( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,
								GL_NEAREST );
			glTexParameteri ( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,
								GL_NEAREST );
			unsigned char *frogData = buildAlphaData ( frogImage[i] );
			glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
							GL_RGBA, GL_UNSIGNED_BYTE, frogData );
			free ( frogData );
		}
	} //end constructor
	//---class functions--------------------------------------------------
	// set frogs x,y values and velocities
	void move ( float xp, float yp, float xv, float yv )
	{
		current = update_position ( &current,xp,yp,xv,yv );
	}
	//draw frog to screen
	void render ( void );

	//get x,y positions and velocities
	float getXpos()
	{
		return current.x_pos;
	}
	void setFrame ( int frame )
	{
		current.frame = frame;
	}
	int getFrame ( )
	{
		return current.frame;
	}
	float getYpos()
	{
		return current.y_pos;
	}
	void setYpos ( float y )
	{
		current.y_pos = y;
	}
	void setXpos ( float x )
	{
		current.x_pos = x;
	}
	float getXvel()
	{
		return current.x_vel;
	}
	float getYvel()
	{
		return current.y_vel;
	}
	bool rocket()
	{
		return rocketFrog;
	}
	void toggleRocket()
	{
		if ( rocketFrog ) {
			rocketFrog = false;
			return;
		}
		if ( numberRockets > 0 && !rocketFrog ) {
			rocketFrog = true;
			numberRockets--;
		}
	}
	void addRocket()
	{
		numberRockets++;
	}
	void resetRocket()
	{
		numberRockets=1;
	}
	int getNumberRockets()
	{
		return numberRockets;
	}
	void meterOff()
	{
		meter = false;
	}
	void meterOn()
	{
		meter = true;
	}
	bool getMeter()
	{
		return meter;
	}
}; //end frog class
// ===========================================================================

class Fly
{
private:
	Position current;
	Position previous;
	Position frog;
	int deadTime;
	bool isStanding;
	Ppmimage *flyImage[11];
	GLuint flyTexture[11];
	bool alive;
	int flip;
public:
	// Constructor with default values for data members
	Fly()
	{
		alive = true;
		deadTime=0;
		isStanding = true;
		current.frame = 0;
		current.x_pos = 300;
		current.y_pos =150;
		current.x_vel = 0;
		current.y_vel = -1;
		flip=0;
		previous = current;
		flyImage[0] = get_image ( "./images/fly" );
		flyImage[1] = get_image ( "./images/fly1" );
		flyImage[2] = get_image ( "./images/fly2" );
		flyImage[3] = get_image ( "./images/fly3" );
		flyImage[4] = get_image ( "./images/fly4" );
		flyImage[5] = get_image ( "./images/fly5" );
		flyImage[6] = get_image ( "./images/fly6" );
		flyImage[7] = get_image ( "./images/fly7" );
		flyImage[8] = get_image ( "./images/fly8" );
		flyImage[9] = get_image ( "./images/fly9" );
		flyImage[10] = get_image ( "./images/fly10" );
		for ( int i =0; i<=10; i++ ) {
			//create opengl texture elements
			glGenTextures ( 1, &flyTexture[i] );
			int w = flyImage[i]->width;
			int h = flyImage[i]->height;
			glBindTexture ( GL_TEXTURE_2D, flyTexture[i] );
			glTexParameteri ( GL_TEXTURE_2D,
								GL_TEXTURE_MAG_FILTER,GL_NEAREST );
			glTexParameteri ( GL_TEXTURE_2D,
								GL_TEXTURE_MIN_FILTER,GL_NEAREST );
			unsigned char *flyData = buildAlphaData ( flyImage[i] );
			glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
							GL_RGBA, GL_UNSIGNED_BYTE, flyData );
			free ( flyData );
		}
	} //end constructor
	//--------------------------------------------------------------------
	void move ( float xp, float yp, float xv, float yv )
	{
		current = update_position ( &current,xp,yp,xv,yv );
	}

	void setFrame ( int frame )
	{
		current.frame = frame;
	}

	void render ( float );

	float getXpos()
	{
		return current.x_pos;
	}
	float getYpos()
	{
		return current.y_pos;
	}
	float getXvel()
	{
		return current.x_vel;
	}
	float getYvel()
	{
		return current.y_vel;
	}
	float isAlive()
	{
		return alive;
	}
	void live()
	{
		alive =true;
	}
	void death ( int x,int y )
	{
		alive = false;
		frog.x_pos = x;
		frog.y_pos = y;
	}
}; //end fly class
// ===========================================================================
class Bridge
{
private:
	Position current;
	Position previous;
	Ppmimage *bridgeImage[2];
	GLuint bridgeTexture[2];
public:
	// Constructor with default values for data members
	Bridge()
	{
		current.frame =0;
		current.x_pos = 300;
		current.y_pos =150;
		current.x_vel = 0;
		current.y_vel = -1;
		previous = current;
		bridgeImage[0] = get_image ( "./images/bridge" );
		int i=0;
		//create opengl texture elements
		glGenTextures ( 1, &bridgeTexture[i] );
		int w = bridgeImage[i]->width;
		int h = bridgeImage[i]->height;
		//
		glBindTexture ( GL_TEXTURE_2D, bridgeTexture[i] );
		glTexParameteri ( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST );
		glTexParameteri ( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST );
		unsigned char *bridgeData = buildAlphaData ( bridgeImage[i] );
		glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
						GL_RGBA, GL_UNSIGNED_BYTE, bridgeData );
		free ( bridgeData );
	} //end constructor
	//--------------------------------------------------------------------
	void render ( void );
	float getXpos()
	{
		return current.x_pos;
	}
	float getYpos()
	{
		return current.y_pos;
	}
	float getXvel()
	{
		return current.x_vel;
	}
	float getYvel()
	{
		return current.y_vel;
	}


	void move ( float xp, float yp, float xv, float yv )
	{
		current = update_position ( &current,xp,yp,xv,yv );
	}

}; //end bridge class
// ==========================================================

class Log
{
private:
	Position current;
	Position previous;
	Ppmimage *logImage[2];
	GLuint logTexture[2];
public:
	// Constructor with default values for data members
	Log()
	{
		current.frame =0;
		current.x_pos = 300;
		current.y_pos =-200;
		current.x_vel = 0;
		current.y_vel = -1;
		previous = current;
		logImage[0] = get_image ( "./images/log" );
		logImage[1] = get_image ( "./images/log1" );
		for ( int i=0; i<2; i++ ) {
			//create opengl texture elements
			glGenTextures ( 1, &logTexture[i] );
			int w = logImage[i]->width;
			int h = logImage[i]->height;
			//
			glBindTexture ( GL_TEXTURE_2D, logTexture[i] );
			glTexParameteri ( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST );
			glTexParameteri ( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST );
			unsigned char *logData = buildAlphaData ( logImage[i] );
			glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
							GL_RGBA, GL_UNSIGNED_BYTE, logData );
			free ( logData );
		}
	} //end constructor
	//--------------------------------------------------------------------
	void render ( void );
	float getXpos()
	{
		return current.x_pos;
	}
	float getYpos()
	{
		return current.y_pos;
	}
	float getXvel()
	{
		return current.x_vel;
	}
	float getYvel()
	{
		return current.y_vel;
	}
	void move ( float xp, float yp, float xv, float yv )
	{
		current = update_position ( &current,xp,yp,xv,yv );
	}
}; //end log class
// =======================================================

class Gator
{
private:
	Position current;
	Position previous;
	Ppmimage *gatorImage[7];
	GLuint gatorTexture[7];
	int diving;
	int eating;
public:
	// Constructor with default values for data members
	Gator()
	{
		diving = 0;
		eating = 1;
		current.frame =0;
		current.x_pos = 500;
		current.y_pos =400;
		current.x_vel = -2;
		current.y_vel = -0.5;
		previous = current;
		gatorImage[0] = get_image ( "./images/aligator" );
		gatorImage[1] = get_image ( "./images/aligator1" );
		gatorImage[2] = get_image ( "./images/aligator2" );
		gatorImage[3] = get_image ( "./images/aligator3" );
		gatorImage[4] = get_image ( "./images/aligator4" );
		gatorImage[5] = get_image ( "./images/aligator5" );
		gatorImage[6] = get_image ( "./images/aligator6" );
		for ( int i=0; i<7; i++ ) {
			//create opengl texture elements
			glGenTextures ( 1, &gatorTexture[i] );
			int w = gatorImage[i]->width;
			int h = gatorImage[i]->height;
			glBindTexture ( GL_TEXTURE_2D, gatorTexture[i] );
			glTexParameteri ( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,
								GL_NEAREST );
			glTexParameteri ( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,
								GL_NEAREST );
			unsigned char *gatorData = buildAlphaData ( gatorImage[i] );
			glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
							GL_RGBA, GL_UNSIGNED_BYTE, gatorData );
			free ( gatorData );
		}
	} //end constructor

	//--------------------------------------------------------------------
	void render ( void );
	float getXpos()
	{
		return current.x_pos;
	}
	float getYpos()
	{
		return current.y_pos;
	}
	float getXvel()
	{
		return current.x_vel;
	}
	float getYvel()
	{
		return current.y_vel;
	}
	void move ( float xp, float yp, float xv, float yv )
	{
		current = update_position ( &current,xp,yp,xv,yv );
	}
	void dive()
	{
		diving=1;
	}
	void eat()
	{
		eating=1;
	}
};
//end gator class ==============================================

class WaterBG
{
private:
	Position current;
	Position previous;
	Ppmimage *waterbgImage[2];
	GLuint waterbgTexture[2];
public:
	// Constructor with default values for data members
	WaterBG()
	{
		current.frame = 0;
		current.x_pos = WIDTH;
		current.y_pos = HEIGHT;
		current.x_vel = 0;
		current.y_vel = -1;
		previous = current;
		waterbgImage[0] = get_image ( "./images/waterBG" );
		waterbgImage[1] = get_image ( "./images/waterBG1" );
		for ( int i=0; i<2; i++ ) {
		//create opengl texture elements
		glGenTextures ( 1, &waterbgTexture[i] );
		int w = waterbgImage[i]->width;
		int h = waterbgImage[i]->height;
		//
		glBindTexture ( GL_TEXTURE_2D, waterbgTexture[i] );
		glTexParameteri ( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST );
		glTexParameteri ( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST );
		unsigned char *waterbgData = buildAlphaData ( waterbgImage[i] );
		glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
						GL_RGBA, GL_UNSIGNED_BYTE, waterbgData );
		free ( waterbgData );
		}
	} //end constructor
	//--------------------------------------------------------------------
	void render ( void );
	float getXpos()
	{
		return current.x_pos;
	}
	float getYpos()
	{
		return current.y_pos;
	}
	float getXvel()
	{
		return current.x_vel;
	}
	float getYvel()
	{
		return current.y_vel;
	}
	void move ( float xp, float yp, float xv, float yv )
	{
		current = update_position ( &current,xp,yp,xv,yv );
	}
};
// ==============================================================
class Water
{
private:
	Position current;
	Position previous;
	Ppmimage *waterImage[4];
	GLuint waterTexture[4];
public:
	// Constructor with default values for data members
	Water()
	{
		current.frame =0;
		current.x_pos = WIDTH/2;
		current.y_pos =HEIGHT;
		current.x_vel = 0;
		current.y_vel = -1;
		previous = current;
		waterImage[0] = get_image ( "./images/water1" );
		waterImage[1] = get_image ( "./images/water1" );
		for ( int i=0; i<2; i++ ) {
			//create opengl texture elements
			glGenTextures ( 1, &waterTexture[i] );
			int w = waterImage[i]->width;
			int h = waterImage[i]->height;
			glBindTexture ( GL_TEXTURE_2D, waterTexture[i] );
			glTexParameteri ( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,
							GL_NEAREST );
			glTexParameteri ( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,
							GL_NEAREST );
			unsigned char *waterData = buildAlphaData ( waterImage[i] );
			glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
							GL_RGBA, GL_UNSIGNED_BYTE, waterData );
			free ( waterData );
		}
	} //end constructor

	//--------------------------------------------------------------------
	void render ( void );
	float getXpos()
	{
		return current.x_pos;
	}
	float getYpos()
	{
		return current.y_pos;
	}
	float getXvel()
	{
		return current.x_vel;
	}
	float getYvel()
	{
		return current.y_vel;
	}
	void move ( float xp, float yp, float xv, float yv )
	{
		current = update_position ( &current,xp,yp,xv,yv );
	}
}; //end water class =========================================================




// ===========================================================================
class Splash
{
private:
	Position current;
	Position previous;
	Ppmimage *splashImage[5];
	GLuint splashTexture[5];

public:
	// Constructor with default values for data members
	Splash()
	{
		current.frame = 0;
		current.x_pos = -300;
		current.y_pos =-800;
		current.x_vel = 0;
		current.y_vel = 0;
		previous = current;
		splashImage[0] = get_image ( "./images/splash" );
		splashImage[1] = get_image ( "./images/splash1" );
		splashImage[2] = get_image ( "./images/splash2" );
		splashImage[3] = get_image ( "./images/splash3" );
		splashImage[4] = get_image ( "./images/splash4" );
		for ( int i =0; i<5; i++ ) {
			//create opengl texture elements
			glGenTextures ( 1, &splashTexture[i] );
			int w = splashImage[i]->width;
			int h = splashImage[i]->height;
			//
			glBindTexture ( GL_TEXTURE_2D, splashTexture[i] );
			glTexParameteri ( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,
							GL_NEAREST );
			glTexParameteri ( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,
							GL_NEAREST );
			unsigned char *splashData = buildAlphaData ( splashImage[i] );
			glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
							GL_RGBA, GL_UNSIGNED_BYTE, splashData );
			free ( splashData );
		}
	} //end constructor
	//--------------------------------------------------------------------
	float getFrame()
	{
		return current.frame;
	}
	void setFrame ( float x )
	{
		current.frame=x;
	}
	void render ( void );
	float getXpos()
	{
		return current.x_pos;
	}
	float getYpos()
	{
		return current.y_pos;
	}
	float getXvel()
	{
		return current.x_vel;
	}
	float getYvel()
	{
		return current.y_vel;
	}
	void move ( float xp, float yp, float xv, float yv )
	{
		current = update_position ( &current,xp,yp,xv,yv );
		current.frame=0;
	}
}; //end splash class ======================================

class Explosion
{
private:
	Position current;
	Position previous;
	Ppmimage *explosionImage[7];
	GLuint explosionTexture[7];

public:
	// Constructor with default values for data members
	Explosion()
	{
		current.frame = 0;
		current.x_pos = -300;
		current.y_pos =-800;
		current.x_vel = 0;
		current.y_vel = 0;
		previous = current;
		explosionImage[0] = get_image ( "./images/explosion" );
		explosionImage[1] = get_image ( "./images/explosion1" );
		explosionImage[2] = get_image ( "./images/explosion2" );
		explosionImage[3] = get_image ( "./images/explosion3" );
		explosionImage[4] = get_image ( "./images/explosion4" );
		explosionImage[4] = get_image ( "./images/explosion5" );
		explosionImage[4] = get_image ( "./images/explosion6" );
		for ( int i =0; i<5; i++ ) {
			//create opengl texture elements
			glGenTextures ( 1, &explosionTexture[i] );
			int w = explosionImage[i]->width;
			int h = explosionImage[i]->height;
			//
			glBindTexture ( GL_TEXTURE_2D, explosionTexture[i] );
			glTexParameteri ( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,
							GL_NEAREST );
			glTexParameteri ( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,
							GL_NEAREST );
			unsigned char *explosionData = buildAlphaData ( explosionImage[i] );
			glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
							GL_RGBA, GL_UNSIGNED_BYTE, explosionData );
			free ( explosionData );
		}
	} //end constructor
	//--------------------------------------------------------------------
	float getFrame()
	{
		return current.frame;
	}
	void setFrame ( float x )
	{
		current.frame=x;
	}
	void render ( void );
	float getXpos()
	{
		return current.x_pos;
	}
	float getYpos()
	{
		return current.y_pos;
	}
	float getXvel()
	{
		return current.x_vel;
	}
	float getYvel()
	{
		return current.y_vel;
	}
	void move ( float xp, float yp, float xv, float yv )
	{
		current = update_position ( &current,xp,yp,xv,yv );
		current.frame=0;
	}
}; //end explosion class ======================================

// ===========================================================================
class RocketPack
{
private:
	Position current;
	Position previous;
	Ppmimage *rocketPackImage[2];
	GLuint rocketPackTexture[2];

public:
	// Constructor with default values for data members
	RocketPack()
	{
		current.frame = 0;
		current.x_pos = -300;
		current.y_pos =-800;
		current.x_vel = 0;
		current.y_vel = 0;
		previous = current;
		rocketPackImage[0] = get_image ( "./images/rocketPack" );
		int i=0;
		//create opengl texture elements
		glGenTextures ( 1, &rocketPackTexture[i] );
		int w = rocketPackImage[i]->width;
		int h = rocketPackImage[i]->height;
		//
		glBindTexture ( GL_TEXTURE_2D, rocketPackTexture[i] );
		glTexParameteri ( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,
						GL_NEAREST );
		glTexParameteri ( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,
						GL_NEAREST );
		unsigned char *rocketPackData = buildAlphaData ( rocketPackImage[i] );
		glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
						GL_RGBA, GL_UNSIGNED_BYTE, rocketPackData );
		free ( rocketPackData );
	} //end constructor
	//--------------------------------------------------------------------
	float getFrame()
	{
		return current.frame;
	}
	void setFrame ( float x )
	{
		current.frame=x;
	}
	void render ( void );
	float getXpos()
	{
		return current.x_pos;
	}
	float getYpos()
	{
		return current.y_pos;
	}
	float getXvel()
	{
		return current.x_vel;
	}
	float getYvel()
	{
		return current.y_vel;
	}
	void move ( float xp, float yp, float xv, float yv )
	{
		current = update_position ( &current,xp,yp,xv,yv );
		current.frame=0;
	}
}; //end rocketPack class ======================================

class Meter
{
private:
	Position current;
	Position previous;
	Ppmimage *meterImage[2];
	GLuint meterTexture[2];
public:
	// Constructor with default values for data members
	Meter()
	{
		current.frame = 0;
		current.x_pos = WIDTH-40;
		current.y_pos = HEIGHT-20;
		current.x_vel = 0;
		current.y_vel = 0;
		previous = current;
		meterImage[0] = get_image ( "./images/meter" );
		int i=0;
		//create opengl texture elements
		glGenTextures ( 1, &meterTexture[i] );
		int w = meterImage[i]->width;
		int h = meterImage[i]->height;
		//
		glBindTexture ( GL_TEXTURE_2D, meterTexture[i] );
		glTexParameteri ( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,
						GL_NEAREST );
		glTexParameteri ( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,
						GL_NEAREST );
		unsigned char *meterData = buildAlphaData ( meterImage[i] );
		glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
						GL_RGBA, GL_UNSIGNED_BYTE, meterData );
		free ( meterData );
	} //end constructor
	//--------------------------------------------------------------------
	float getFrame()
	{
		return current.frame;
	}
	void setFrame ( float x )
	{
		current.frame=x;
	}
	void render ( void );
	float getXpos()
	{
		return current.x_pos;
	}
	float getYpos()
	{
		return current.y_pos;
	}
	float getXvel()
	{
		return current.x_vel;
	}
	float getYvel()
	{
		return current.y_vel;
	}
	void move ( float xp, float yp, float xv, float yv )
	{
		current = update_position ( &current,xp,yp,xv,yv );
		current.frame=0;
	}
}; //end meter class ======================================

class Turtle
{
private:
	Position current;
	Position previous;
	Ppmimage *turtleImage[12];
	GLuint turtleTexture[12];
	bool golden;
	int goldenFrames;
public:
	// Constructor with default values for data members
	Turtle()
	{
		int r= rand() %600+1;
		current.frame =0;
		current.x_pos = WIDTH-r;
		current.y_pos =-HEIGHT/2 + r;
		current.x_vel = 2;
		current.y_vel = 0.1;
		previous = current;
		golden = false;
		goldenFrames =0;
		turtleImage[0] = get_image ( "./images/turtle" );
		turtleImage[1] = get_image ( "./images/turtle1" );
		turtleImage[2] = get_image ( "./images/turtle2" );
		turtleImage[3] = get_image ( "./images/turtle3" );
		turtleImage[4] = get_image ( "./images/turtle4" );
		turtleImage[5] = get_image ( "./images/turtle5" );
		turtleImage[6] = get_image ( "./images/turtleG" );
		turtleImage[7] = get_image ( "./images/turtleG1" );
		turtleImage[8] = get_image ( "./images/turtleG2" );
		turtleImage[9] = get_image ( "./images/turtleG3" );
		turtleImage[10] = get_image ( "./images/turtleG4" );
		turtleImage[11] = get_image ( "./images/turtleG5" );
		for ( int i=0; i<12; i++ ) {
			//create opengl texture elements
			glGenTextures ( 1, &turtleTexture[i] );
			int w = turtleImage[i]->width;
			int h = turtleImage[i]->height;
			//
			glBindTexture ( GL_TEXTURE_2D, turtleTexture[i] );
			glTexParameteri ( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,
							GL_NEAREST );
			glTexParameteri ( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,
							GL_NEAREST );
			unsigned char *turtleData = buildAlphaData ( turtleImage[i] );
			glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
							GL_RGBA, GL_UNSIGNED_BYTE, turtleData );
			free ( turtleData );
		}
	} //end constructor
	//--------------------------------------------------------------------
	void render ( void );
	float getXpos()
	{
		return current.x_pos;
	}
	float getYpos()
	{
		return current.y_pos;
	}
	float getXvel()
	{
		return current.x_vel;
	}
	float getYvel()
	{
		return current.y_vel;
	}
	void move ( float xp, float yp, float xv, float yv )
	{
		current = update_position ( &current,xp,yp,xv,yv );
	}
	bool isGolden()
	{
		return golden;
	}
	void notGolden()
	{
		goldenFrames = 0;
		golden = false;
	}
}; //end turtle class

// =======================================================
// =========================================================
