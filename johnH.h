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

extern Position update_position(Position *c,float,float,float,float);
extern unsigned char *buildAlphaData(Ppmimage *img);
extern Ppmimage*  get_image(std::string filename);


class Frog
{
	private:
		Position current;
		Position previous;
		bool isStanding;
		Ppmimage *frogImage[18];
		GLuint frogTexture[18];
		bool rocketFrog;
		int wink;
		int blink;
		int left;
		int right;

	public:
		// Constructor with default values for data members
		Frog() {
			isStanding = true;
			current.frame = 0;
			current.x_pos = 300;
			current.y_pos =150;
			current.x_vel = 0;
			current.y_vel = -1;
			wink = 0;
			blink = 0;
			left = 0;
			right = 0;
			previous = current;
			frogImage[0] = get_image("./images/frog");
			frogImage[1] = get_image("./images/frog1");
			frogImage[2] = get_image("./images/frog2");
			frogImage[3] = get_image("./images/frog3");
			frogImage[4] = get_image("./images/frog4");
			frogImage[5] = get_image("./images/frog5");
			frogImage[6] = get_image("./images/frog6");
			frogImage[7] = get_image("./images/frog7");
			frogImage[8] = get_image("./images/frog8");
			frogImage[9] = get_image("./images/frog9");
			frogImage[10] = get_image("./images/frog10");
			frogImage[11] = get_image("./images/rocket");
			frogImage[12] = get_image("./images/rocket1");
			frogImage[13] = get_image("./images/rocket2");
			frogImage[14] = get_image("./images/rocket3");
			frogImage[15] = get_image("./images/rocket4");
			frogImage[16] = get_image("./images/frog_wink");
			frogImage[17] = get_image("./images/frog_blink");
			for (int i =0; i<=17; i++) {
				//create opengl texture elements
				glGenTextures(1, &frogTexture[i]);
				int w = frogImage[i]->width;
				int h = frogImage[i]->height;
				//
				glBindTexture(GL_TEXTURE_2D, frogTexture[i]);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
				unsigned char *frogData = buildAlphaData(frogImage[i]);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
						GL_RGBA, GL_UNSIGNED_BYTE, frogData);
				free(frogData);
			}
		} //end constructor
		//---class functions-----------------------------------------------------------
		// set frogs x,y values and velocities
		void move (float xp, float yp, float xv, float yv) {
			current = update_position(&current,xp,yp,xv,yv);
		}
		//draw frog to screen
		void render(void);

		//get x,y positions and velocities
		float getXpos() {
			return current.x_pos;
		}
		void setFrame(int frame) {
			current.frame = frame;
		}
		float getYpos() {
			return current.y_pos;
		}
		void setYpos(float y){
		current.y_pos = y;
		}
		void setXpos(float x){
		current.x_pos = x;
		}
		float getXvel() {
			return current.x_vel;
		}
		float getYvel() {
			return current.y_vel;
		}
		bool rocket() {
            return rocketFrog;
		}
		void toggleRocket() {
            rocketFrog ^=1;
		}
}; //end frog class
// ======================================================================================

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
		Fly() {
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
			flyImage[0] = get_image("./images/fly");
			flyImage[1] = get_image("./images/fly1");
			flyImage[2] = get_image("./images/fly2");
			flyImage[3] = get_image("./images/fly3");
			flyImage[4] = get_image("./images/fly4");
			flyImage[5] = get_image("./images/fly5");
			flyImage[6] = get_image("./images/fly6");
			flyImage[7] = get_image("./images/fly7");
			flyImage[8] = get_image("./images/fly8");
			flyImage[9] = get_image("./images/fly9");
			flyImage[10] = get_image("./images/fly10");
			for (int i =0; i<=10; i++) {
				//create opengl texture elements
				glGenTextures(1, &flyTexture[i]);
				int w = flyImage[i]->width;
				int h = flyImage[i]->height;
				glBindTexture(GL_TEXTURE_2D, flyTexture[i]);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
				unsigned char *flyData = buildAlphaData(flyImage[i]);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
						GL_RGBA, GL_UNSIGNED_BYTE, flyData);
				free(flyData);
			}
		} //end constructor
		//-------------------------------------------------------------------------
		void move (float xp, float yp, float xv, float yv) {
			current = update_position(&current,xp,yp,xv,yv);
		}

		void setFrame(int frame){
		current.frame = frame;
		}

		void render(void);

		float getXpos() {
			return current.x_pos;
		}
		float getYpos() {
			return current.y_pos;
		}
		float getXvel() {
			return current.x_vel;
		}
		float getYvel() {
			return current.y_vel;
			}
		float isAlive(){
			return alive;
		}
		void live(){
			alive =true;
		}
		void death(int x,int y){
			alive = false;
			frog.x_pos = x;
			frog.y_pos = y;
		}
}; //end fly class
// ======================================================================================
class Bridge
{
	private:
		Position current;
		Position previous;
		Ppmimage *bridgeImage[2];
		GLuint bridgeTexture[2];
	public:
		// Constructor with default values for data members
		Bridge() {
			current.frame =0;
			current.x_pos = 300;
			current.y_pos =150;
			current.x_vel = 0;
			current.y_vel = -1;
			previous = current;
			bridgeImage[0] = get_image("./images/bridge");

			int i=0;
			//create opengl texture elements
			glGenTextures(1, &bridgeTexture[i]);
			int w = bridgeImage[i]->width;
			int h = bridgeImage[i]->height;
			//
			glBindTexture(GL_TEXTURE_2D, bridgeTexture[i]);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
			unsigned char *bridgeData = buildAlphaData(bridgeImage[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
					GL_RGBA, GL_UNSIGNED_BYTE, bridgeData);
			free(bridgeData);

		} //end constructor
		//-------------------------------------------------------------------------
		void render(void);
		float getXpos() {
			return current.x_pos;
		}
		float getYpos() {
			return current.y_pos;
		}
		float getXvel() {
			return current.x_vel;
		}
		float getYvel() {
			return current.y_vel;
		}


		void move (float xp, float yp, float xv, float yv) {
			current = update_position(&current,xp,yp,xv,yv);
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
		Log() {
			current.frame =0;
			current.x_pos = 300;
			current.y_pos =-200;
			current.x_vel = 0;
			current.y_vel = -1;
			previous = current;
			logImage[0] = get_image("./images/log");
			logImage[1] = get_image("./images/log1");
			for (int i=0; i<2; i++) {
				//create opengl texture elements
				glGenTextures(1, &logTexture[i]);
				int w = logImage[i]->width;
				int h = logImage[i]->height;
				//
				glBindTexture(GL_TEXTURE_2D, logTexture[i]);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
				unsigned char *logData = buildAlphaData(logImage[i]);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
						GL_RGBA, GL_UNSIGNED_BYTE, logData);
				free(logData);
			}
		} //end constructor
		//-------------------------------------------------------------------------
		void render(void);
		float getXpos() {
			return current.x_pos;
		}
		float getYpos() {
			return current.y_pos;
		}
		float getXvel() {
			return current.x_vel;
		}
		float getYvel() {
			return current.y_vel;
		}
		void move (float xp, float yp, float xv, float yv) {
			current = update_position(&current,xp,yp,xv,yv);
		}
}; //end log class



// =======================================================

class Gator
{
	private:
		Position current;
		Position previous;
		Ppmimage *gatorImage[2];
		GLuint gatorTexture[2];
	public:
		// Constructor with default values for data members
		Gator() {
			current.frame =0;
			current.x_pos = 500;
			current.y_pos =400;
			current.x_vel = -2;
			current.y_vel = -0.5;
			previous = current;
			gatorImage[0] = get_image("./images/aligator");
			gatorImage[1] = get_image("./images/aligator1");
			for (int i=0; i<2; i++) {
				//create opengl texture elements
				glGenTextures(1, &gatorTexture[i]);
				int w = gatorImage[i]->width;
				int h = gatorImage[i]->height;
				glBindTexture(GL_TEXTURE_2D, gatorTexture[i]);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
				unsigned char *gatorData = buildAlphaData(gatorImage[i]);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
						GL_RGBA, GL_UNSIGNED_BYTE, gatorData);
				free(gatorData);
			}
		} //end constructor

		//-------------------------------------------------------------------------
		void render(void);
		float getXpos() {
			return current.x_pos;
		}
		float getYpos() {
			return current.y_pos;
		}
		float getXvel() {
			return current.x_vel;
		}
		float getYvel() {
			return current.y_vel;
		}
		void move (float xp, float yp, float xv, float yv) {
			current = update_position(&current,xp,yp,xv,yv);
		}

}; //end gator class ==============================================




// ================================================================
class Water
{
	private:
		Position current;
		Position previous;
		Ppmimage *waterImage[4];
		GLuint waterTexture[4];
	public:
		// Constructor with default values for data members
		Water() {
			current.frame =0;
			current.x_pos = WIDTH/2;
			current.y_pos =HEIGHT;
			current.x_vel = 0;
			current.y_vel = -1;
			previous = current;
			waterImage[0] = get_image("./images/water");
			waterImage[1] = get_image("./images/water1");
			for (int i=0; i<2; i++) {
				//create opengl texture elements
				glGenTextures(1, &waterTexture[i]);
				int w = waterImage[i]->width;
				int h = waterImage[i]->height;
				glBindTexture(GL_TEXTURE_2D, waterTexture[i]);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
				unsigned char *waterData = buildAlphaData(waterImage[i]);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
						GL_RGBA, GL_UNSIGNED_BYTE, waterData);
				free(waterData);
			}
		} //end constructor

		//-------------------------------------------------------------------------
		void render(void);
		float getXpos() {
			return current.x_pos;
		}
		float getYpos() {
			return current.y_pos;
		}
		float getXvel() {
			return current.x_vel;
		}
		float getYvel() {
			return current.y_vel;
		}
		void move (float xp, float yp, float xv, float yv) {
			current = update_position(&current,xp,yp,xv,yv);
		}
}; //end water class ================================================================




// ==================================================================================
class Splash
{
	private:
		Position current;
		Position previous;
		Ppmimage *splashImage[5];
		GLuint splashTexture[5];

	public:
		// Constructor with default values for data members
		Splash() {
			current.frame = 0;
			current.x_pos = -300;
			current.y_pos =-800;
			current.x_vel = 0;
			current.y_vel = 0;
			previous = current;
			splashImage[0] = get_image("./images/splash");
			splashImage[1] = get_image("./images/splash1");
			splashImage[2] = get_image("./images/splash2");
			splashImage[3] = get_image("./images/splash3");
			splashImage[4] = get_image("./images/splash4");
			for (int i =0; i<5; i++) {
				//create opengl texture elements
				glGenTextures(1, &splashTexture[i]);
				int w = splashImage[i]->width;
				int h = splashImage[i]->height;
				//
				glBindTexture(GL_TEXTURE_2D, splashTexture[i]);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
				unsigned char *splashData = buildAlphaData(splashImage[i]);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
						GL_RGBA, GL_UNSIGNED_BYTE, splashData);
				free(splashData);
			}
		} //end constructor
		//-------------------------------------------------------------------------
		float getFrame() {
			return current.frame;
		}
		void setFrame(float x) {
			current.frame=x;
		}
		void render(void);
		float getXpos() {
			return current.x_pos;
		}
		float getYpos() {
			return current.y_pos;
		}
		float getXvel() {
			return current.x_vel;
		}
		float getYvel() {
			return current.y_vel;
		}
		void move (float xp, float yp, float xv, float yv) {
			current = update_position(&current,xp,yp,xv,yv);
			current.frame=0;
		}
}; //end splash class ======================================

class LP
{
	private:
		Position current;
		Position previous;
		Ppmimage *lpImage[2];
		GLuint lpTexture[2];
	public:
		// Constructor with default values for data members
		LP()
		{      int r= rand()%600+1;
			current.frame =0;
			current.x_pos = 300-r;
			current.y_pos =-200+r;
			current.x_vel = 0.5;
			current.y_vel = -2.5;
			previous = current;
			lpImage[0] = get_image("./images/lp");
			lpImage[1] = get_image("./images/lp1");
			for(int i=0; i<2; i++) {
				//create opengl texture elements
				glGenTextures(1, &lpTexture[i]);
				int w = lpImage[i]->width;
				int h = lpImage[i]->height;
				//
				glBindTexture(GL_TEXTURE_2D, lpTexture[i]);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
				unsigned char *lpData = buildAlphaData(lpImage[i]);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
						GL_RGBA, GL_UNSIGNED_BYTE, lpData);
				free(lpData);
			}
		} //end constructor
		//-------------------------------------------------------------------------
		void render(void);
		float getXpos() {
			return current.x_pos;
		}
		float getYpos() {
			return current.y_pos;
		}
		float getXvel() {
			return current.x_vel;
		}
		float getYvel() {
			return current.y_vel;
		}
		void move (float xp, float yp, float xv, float yv) {
			current = update_position(&current,xp,yp,xv,yv);
		}
}; //end lp class

// =======================================================
// =========================================================
