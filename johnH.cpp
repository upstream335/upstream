//cs335 john hargreaves
//animation classes for
//frog,log,gator,bridge
//splash and background water
#include "johnH.h"
using namespace tinyxml2;

//convert png to ppm read in texture and destroy ppm
Ppmimage* get_image ( std::string filename )
{
	std::string filename2 = filename;
	filename2 = filename + ".ppm";
	std::string command = "convert " + filename + ".png " + filename2;
	system ( ( command ).c_str() );
	Ppmimage *tempimage = ppm6GetImage ( ( filename2 ).c_str() );
	command ="rm " + filename2;
	system ( ( command ).c_str() );
	return tempimage;
}

//update objects position and velocity
Position update_position ( Position *c,float xp, float yp,float xv,float yv )
{
	c->x_pos = xp;
	c->y_pos = yp;
	c->x_vel = xv;
	c->y_vel = yv;
	c->x_pos += c->x_vel;
	c->y_pos += c->y_vel;
	return *c;
}


void HUD::render ( void )
{
	if ( current.x_pos<-100 ) {
		//hud is offscreen
	}
	float wid = 240.0f; // size of HUD sprite
	glColor3f ( 1.0, 1.0, 1.0 );
	glPushMatrix();
	glTranslatef ( current.x_pos, current.y_pos, 0 );
	glBindTexture ( GL_TEXTURE_2D, hudTexture[0] );
	glBindTexture ( GL_TEXTURE_2D, hudTexture[0] );
	glEnable ( GL_ALPHA_TEST );
	glAlphaFunc ( GL_GREATER, 0.0f );
	glColor4ub ( 255,255,255,255 );
	glBegin ( GL_QUADS );
	glTexCoord2f ( 0.0f, 1.0f );
	glVertex2i ( -wid,-wid );
	glTexCoord2f ( 0.0f, 0.0f );
	glVertex2i ( -wid, wid );
	glTexCoord2f ( 1.0f, 0.0f );
	glVertex2i ( wid, wid );
	glTexCoord2f ( 1.0f, 1.0f );
	glVertex2i ( wid,-wid );
	glEnd();
	glPopMatrix();
	glDisable ( GL_ALPHA_TEST );
	glDisable ( GL_TEXTURE_2D );
	glBlendFunc ( GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA );
	glEnable ( GL_BLEND );
	glDisable ( GL_BLEND );
	glEnable ( GL_TEXTURE_2D );
}
// end hud render ===========================================


// Render frog =============================================
void Frog::render ( void )
{
	if ( current.y_pos < 50 ) {
		current.y_vel=0;
	}
	float wid = 30.0f; // size of frog sprite
	glColor3f ( 1.0, 1.0, 1.0 );
	glPushMatrix();
	if ( rocketFrog ) {
		//draw a line for rocket power remaining
		glLineWidth ( 15 );
		glColor3f ( 200.0, 0.0, 0.0 );
		glBindTexture ( GL_TEXTURE_2D, 0 );
		glBegin ( GL_LINES );
		// start position
		glVertex2f ( WIDTH-20, ( HEIGHT+20 ) + ( 120-current.frame ) /5 );
		// end position
		glVertex2f ( WIDTH-20, ( HEIGHT+20 ) -100 );
		glEnd();
	}
	if ( !rocketFrog ) {
		if ( meter ) {
			//draw a line for jump height
			glLineWidth ( 15 );
			glColor3f ( 0.0, 200.0, 0.0 );
			glBindTexture ( GL_TEXTURE_2D, 0 );
			glBegin ( GL_LINES );
			// start position
			int velocity=0;
			if ( current.y_vel >= 0 ) {
				velocity = current.y_vel*5;
			} else {
				velocity = 60 + current.y_vel*5;
			}
			glVertex2f ( WIDTH-80, ( HEIGHT+20 ) -100 + velocity*1.5 );
			//std::cout<<velocity<<std::endl;
			// end position
			glVertex2f ( WIDTH-80, ( HEIGHT+20 ) -100 );
			glEnd();
		}
		if ( current.y_pos<30 ) {
			current.y_pos=40;
			current.y_vel=0;
		}
		if ( current.y_pos>100 ) {
			current.x_vel=0;
		}
		glBindTexture ( GL_TEXTURE_2D, frogTexture[0] );
		// JUMPING UP===========================
		if ( current.frame >= 30 && current.y_vel>0 )
			current.frame =0;
		if ( current.x_vel <= 0 && current.frame <= 5 && current.y_vel>0 ) {
			glBindTexture ( GL_TEXTURE_2D, frogTexture[1] );
			current.frame++;
		}
		if ( current.x_vel <= 0 && current.frame <= 10 &&
				current.frame > 5 && current.y_vel>0 ) {
			glBindTexture ( GL_TEXTURE_2D, frogTexture[2] );
			current.frame++;
		}
		if ( current.x_vel <= 0 && /*current.frame <= 20 && */
				current.frame > 10 && current.y_vel>0 ) {
			glBindTexture ( GL_TEXTURE_2D, frogTexture[3] );
			current.frame++;
		}
		// JUMPING DOWN ===================================
		if ( current.x_vel <= 0 && current.frame >= 15 && current.y_vel<0 ) {
			glBindTexture ( GL_TEXTURE_2D, frogTexture[3] );
			current.frame--;
		}
		if ( current.x_vel <= 0 && current.frame >= 5 &&
				current.frame<=10 && current.y_vel<0 ) {
			glBindTexture ( GL_TEXTURE_2D, frogTexture[4] );
			current.frame--;
		}
		if ( current.x_vel <= 0 && current.frame <= 5 && current.y_vel<0 ) {
			glBindTexture ( GL_TEXTURE_2D, frogTexture[5] );
			current.frame--;
		}
		// ==================================================
		//moving along bridge
		if ( previous.x_pos < current.x_pos ) {
			right =10;
			left = 0;
		}
		if ( previous.x_pos > current.x_pos ) {
			left =10;
			right =0;
		}
		if ( right > 0 && current.y_vel <= 0 ) {
			glBindTexture ( GL_TEXTURE_2D, frogTexture[7] );
			current.frame++;
			right--;
			if ( current.frame >= 15 ) {
				glBindTexture ( GL_TEXTURE_2D, frogTexture[8] );
				if ( current.frame > 30 )
					current.frame = 0;
			}
		}
		if ( left > 0 && current.y_vel<=0 ) {
			glBindTexture ( GL_TEXTURE_2D, frogTexture[10] );
			current.frame++;
			left--;
			if ( current.frame >= 15 ) {
				glBindTexture ( GL_TEXTURE_2D, frogTexture[9] );
				if ( current.frame > 30 )
					current.frame = 0;
			}
		}
		//sitting still
		if ( current.x_vel==0 && current.y_vel==0 &&
				current.x_pos == previous.x_pos &&
				right<=0 && left<=0 ) {
			int r   =   rand() %500+1;
			if ( r>2 && blink== 0 && wink == 0 )
				glBindTexture ( GL_TEXTURE_2D, frogTexture[0] );
			// blinking
			if ( r==1 || wink>0 ) {
				if ( wink==0 )
					wink = 15;
				wink--;
				glBindTexture ( GL_TEXTURE_2D, frogTexture[16] );
			}
			//winking
			if ( r==2 || blink >0 ) {
				if ( blink==0 )
					blink = 15;
				blink--;
				glBindTexture ( GL_TEXTURE_2D, frogTexture[17] );
			}
			current.frame = 0;
		}
		if ( current.x_vel==0 && current.y_vel<0 )
			glBindTexture ( GL_TEXTURE_2D, frogTexture[5] );
	} else {
		// ROCKET FROG ==========================
		current.y_pos+=10;
		int frame;
		current.frame++;
		frame = ( current.frame%5 )+11;
		glBindTexture ( GL_TEXTURE_2D, frogTexture[frame] );
		if ( current.frame>300 && current.y_pos>200 ) {
			current.y_pos-=11;
			current.y_vel=0.2;
		}
		if ( current.frame>500 ) {
			rocketFrog=false;
			current.frame=0;
			current.y_vel=0.2;
		}
	}
	previous = current;
	glTranslatef ( current.x_pos, current.y_pos, 0 );
	//draw frog sprite
	glEnable ( GL_ALPHA_TEST );
	glAlphaFunc ( GL_GREATER, 0.0f );
	glColor4ub ( 255,255,255,255 );
	glBegin ( GL_QUADS );
	glTexCoord2f ( 0.0f, 1.0f );
	glVertex2i ( -wid,-wid );
	glTexCoord2f ( 0.0f, 0.0f );
	glVertex2i ( -wid, wid );
	glTexCoord2f ( 1.0f, 0.0f );
	glVertex2i ( wid, wid );
	glTexCoord2f ( 1.0f, 1.0f );
	glVertex2i ( wid,-wid );
	glEnd();
	glPopMatrix();
	glDisable ( GL_ALPHA_TEST );
	glDisable ( GL_TEXTURE_2D );
	glBlendFunc ( GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA );
	glEnable ( GL_BLEND );
	glDisable ( GL_BLEND );
	glEnable ( GL_TEXTURE_2D );
} //end frog render=============================================


void Bridge::render ( void )
{
	if ( current.y_pos < 80 ) {

		glColor3f ( 1.0, 1.0, 1.0 );
		glPushMatrix();
		//draw a line at bottom of screen
		glLineWidth (3);
		glColor3f ( 200.0, 0.0, 0.0 );
		glBindTexture ( GL_TEXTURE_2D, 0 );
		glBegin ( GL_LINES );
		// start position
		glVertex2f ( WIDTH, 85 );
		// end position
		glVertex2f ( 0, 85);
		glEnd();
		//bridge is offscreen
	}
	float wid = 500.0f; // size of bridge sprite
	glColor3f ( 1.0, 1.0, 1.0 );
	glPushMatrix();
	glTranslatef ( current.x_pos, current.y_pos, 0 );
	glBindTexture ( GL_TEXTURE_2D, bridgeTexture[0] );
	glBindTexture ( GL_TEXTURE_2D, bridgeTexture[0] );
	glEnable ( GL_ALPHA_TEST );
	glAlphaFunc ( GL_GREATER, 0.0f );
	glColor4ub ( 255,255,255,255 );
	glBegin ( GL_QUADS );
	glTexCoord2f ( 0.0f, 1.0f );
	glVertex2i ( -wid,-wid );
	glTexCoord2f ( 0.0f, 0.0f );
	glVertex2i ( -wid, wid );
	glTexCoord2f ( 1.0f, 0.0f );
	glVertex2i ( wid, wid );
	glTexCoord2f ( 1.0f, 1.0f );
	glVertex2i ( wid,-wid );
	glEnd();
	glPopMatrix();
	glDisable ( GL_ALPHA_TEST );
	glDisable ( GL_TEXTURE_2D );
	glBlendFunc ( GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA );
	glEnable ( GL_BLEND );
	glDisable ( GL_BLEND );
	glEnable ( GL_TEXTURE_2D );
}
// end bridge render ===========================================



void Log::render ( void )
{
	int r = rand() %600+1;
	current.x_pos += current.x_vel;
	current.y_pos += current.y_vel;
	if ( current.y_pos < -100 ) {
		current.x_pos = WIDTH/2-r;
		current.y_pos = HEIGHT+200;
	}
	float wid = 60.0f; // size of logsprite
	glColor3f ( 1.0, 1.0, 1.0 );
	glPushMatrix();
	glTranslatef ( current.x_pos, current.y_pos, 0 );
	glBindTexture ( GL_TEXTURE_2D, logTexture[0] );
	if ( current.frame>=10 ) {
		glBindTexture ( GL_TEXTURE_2D, logTexture[0] );
		current.frame++;
		if ( current.frame>20 )
			current.frame=0;
	}
	if ( current.frame<10 ) {
		glBindTexture ( GL_TEXTURE_2D, logTexture[1] );
		current.frame++;
	}
	glEnable ( GL_ALPHA_TEST );
	glAlphaFunc ( GL_GREATER, 0.0f );
	glColor4ub ( 255,255,255,255 );
	glBegin ( GL_QUADS );
	glTexCoord2f ( 0.0f, 1.0f );
	glVertex2i ( -wid,-wid );
	glTexCoord2f ( 0.0f, 0.0f );
	glVertex2i ( -wid, wid );
	glTexCoord2f ( 1.0f, 0.0f );
	glVertex2i ( wid, wid );
	glTexCoord2f ( 1.0f, 1.0f );
	glVertex2i ( wid,-wid );
	glEnd();
	glPopMatrix();
	glDisable ( GL_ALPHA_TEST );
	glDisable ( GL_TEXTURE_2D );
	glBlendFunc ( GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA );
	glEnable ( GL_BLEND );
	glDisable ( GL_BLEND );
	glEnable ( GL_TEXTURE_2D );
}
// end Log render ==============================================

void Gator::render ( void )
{
	float wid = 70.0f; // size of gator sprite
	glColor3f ( 1.0, 1.0, 1.0 );
	glPushMatrix();
	glTranslatef ( current.x_pos, current.y_pos, 0 );
	if ( eating==0 and diving==0 ) {
		int r = rand() %800+1;
		current.x_pos += current.x_vel;
		current.y_pos += current.y_vel;
		if ( current.x_pos<0 ) {
			current.x_pos = WIDTH+r;
			current.y_pos = HEIGHT-r;
			current.y_vel = 0.1;
		}
		if ( current.y_pos<200 )  {
			current.y_vel +=.1;
		}
		glBindTexture ( GL_TEXTURE_2D, gatorTexture[0] );
		if ( current.frame>=10 ) {
			glBindTexture ( GL_TEXTURE_2D, gatorTexture[0] );
			current.frame++;
			if ( current.frame>20 )
				current.frame=0;
		}
		if ( current.frame<10 ) {
			glBindTexture ( GL_TEXTURE_2D, gatorTexture[1] );
			current.frame++;
		}
	}
	if ( eating>0 ) {
		glBindTexture ( GL_TEXTURE_2D, gatorTexture[2] );
		eating++;
		if ( eating>10 )
			glBindTexture ( GL_TEXTURE_2D, gatorTexture[3] );
		if ( eating>20 )
			glBindTexture ( GL_TEXTURE_2D, gatorTexture[2] );
		if ( eating>30 )
			eating =0;
	}
	if ( diving>0 && eating<=0 ) {
		glBindTexture ( GL_TEXTURE_2D, gatorTexture[4] );
		diving++;
		if ( diving>10 )
			glBindTexture ( GL_TEXTURE_2D, gatorTexture[5] );
		if ( diving>20 )
			glBindTexture ( GL_TEXTURE_2D, gatorTexture[6] );
		if ( diving>30 )
			diving =0;
	}
	glEnable ( GL_ALPHA_TEST );
	glAlphaFunc ( GL_GREATER, 0.0f );
	glColor4ub ( 255,255,255,255 );
	glBegin ( GL_QUADS );
	glTexCoord2f ( 0.0f, 1.0f );
	glVertex2i ( -wid,-wid );
	glTexCoord2f ( 0.0f, 0.0f );
	glVertex2i ( -wid, wid );
	glTexCoord2f ( 1.0f, 0.0f );
	glVertex2i ( wid, wid );
	glTexCoord2f ( 1.0f, 1.0f );
	glVertex2i ( wid,-wid );
	glEnd();
	glPopMatrix();
	glDisable ( GL_ALPHA_TEST );
	glDisable ( GL_TEXTURE_2D );
	glBlendFunc ( GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA );
	glEnable ( GL_BLEND );
	glDisable ( GL_BLEND );
	glEnable ( GL_TEXTURE_2D );
}
// end Gator render ============================================
void WaterBG::render ( void )
{
	if ( current.x_pos<-100 ) {
		//hud is offscreen
	}
	float wid = 800.0f; // size of waterbg sprite

	glColor3f ( 1.0, 1.0, 1.0 );
	glPushMatrix();
	glTranslatef ( current.x_pos, current.y_pos, 0 );
	current.frame++;
	int frame = current.frame % 2;
	glBindTexture ( GL_TEXTURE_2D, waterbgTexture[frame] );
	if ( current.frame<=10 ) {
		glBindTexture ( GL_TEXTURE_2D, waterbgTexture[0] );
		current.frame++;
	}
	if ( current.frame>10 && current.frame <=20 ) {
		glBindTexture ( GL_TEXTURE_2D, waterbgTexture[1] );
		current.frame++;
	}
	if ( current.frame>=20 )
		current.frame=0;
	glEnable ( GL_ALPHA_TEST );
	glAlphaFunc ( GL_GREATER, 0.0f );
	glColor4ub ( 255,255,255,255 );
	glBegin ( GL_QUADS );
	glTexCoord2f ( 0.0f, 1.0f );
	glVertex2i ( -wid,-wid );
	glTexCoord2f ( 0.0f, 0.0f );
	glVertex2i ( -wid, wid );
	glTexCoord2f ( 1.0f, 0.0f );
	glVertex2i ( wid, wid );
	glTexCoord2f ( 1.0f, 1.0f );
	glVertex2i ( wid,-wid );
	glEnd();
	glPopMatrix();
	glDisable ( GL_ALPHA_TEST );
	glDisable ( GL_TEXTURE_2D );
	glBlendFunc ( GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA );
	glEnable ( GL_BLEND );
	glDisable ( GL_BLEND );
	glEnable ( GL_TEXTURE_2D );
}
// end waterbg render ===========================================

// =============================================================
void Water::render ( void )
{
	current.x_pos += current.x_vel;
	current.y_pos += current.y_vel;
	if ( current.y_pos<-300 ) {
		current.x_pos = WIDTH/2;
		current.y_pos = HEIGHT+300;
	}
	float wid = 300; // size of water
	glColor3f ( 1.0, 1.0, 1.0 );
	glPushMatrix();
	glTranslatef ( current.x_pos, current.y_pos, 0 );
	current.frame++;
	int frame = current.frame % 2;
	glBindTexture ( GL_TEXTURE_2D, waterTexture[frame] );
	if ( current.frame<=10 ) {
		glBindTexture ( GL_TEXTURE_2D, waterTexture[0] );
		current.frame++;
	}
	if ( current.frame>10 && current.frame <=20 ) {
		glBindTexture ( GL_TEXTURE_2D, waterTexture[1] );
		current.frame++;
	}
	if ( current.frame>=20 )
		current.frame=0;
	glEnable ( GL_ALPHA_TEST );
	glAlphaFunc ( GL_GREATER, 0.0f );
	glColor4ub ( 255,255,255,255 );
	glBegin ( GL_QUADS );
	glTexCoord2f ( 0.0f, 1.0f );
	glVertex2i ( -wid,-wid );
	glTexCoord2f ( 0.0f, 0.0f );
	glVertex2i ( -wid, wid );
	glTexCoord2f ( 1.0f, 0.0f );
	glVertex2i ( wid, wid );
	glTexCoord2f ( 1.0f, 1.0f );
	glVertex2i ( wid,-wid );
	glEnd();
	glPopMatrix();
	glDisable ( GL_ALPHA_TEST );
	glDisable ( GL_TEXTURE_2D );
	glBlendFunc ( GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA );
	glEnable ( GL_BLEND );
	glDisable ( GL_BLEND );
	glEnable ( GL_TEXTURE_2D );
}
// end water render ============================================

void Splash::render ( void )
{
	float wid = 50.0f; // size of splash sprite
	glColor3f ( 1.0, 1.0, 1.0 );
	glPushMatrix();
	glTranslatef ( current.x_pos, current.y_pos, 0 );
	glBindTexture ( GL_TEXTURE_2D, splashTexture[0] );
	for ( int i=0; i<5; i++ ) {
		current.frame++;
		if ( current.frame >=i*30 && current.frame <=200 ) {
			glBindTexture ( GL_TEXTURE_2D, splashTexture[i] );
		}
		if ( current.frame >=200 ) {
			current.x_pos = -500;
			current.y_pos = -500;
		}
	}
	if ( current.frame>500 )
		current.frame=500;
	glEnable ( GL_ALPHA_TEST );
	glAlphaFunc ( GL_GREATER, 0.0f );
	glColor4ub ( 255,255,255,255 );
	glBegin ( GL_QUADS );
	glTexCoord2f ( 0.0f, 1.0f );
	glVertex2i ( -wid,-wid );
	glTexCoord2f ( 0.0f, 0.0f );
	glVertex2i ( -wid, wid );
	glTexCoord2f ( 1.0f, 0.0f );
	glVertex2i ( wid, wid );
	glTexCoord2f ( 1.0f, 1.0f );
	glVertex2i ( wid,-wid );
	glEnd();
	glPopMatrix();
	glDisable ( GL_ALPHA_TEST );
	glDisable ( GL_TEXTURE_2D );
	glBlendFunc ( GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA );
	glEnable ( GL_BLEND );
	glDisable ( GL_BLEND );
	glEnable ( GL_TEXTURE_2D );
}
// end splash render ===========================================

void Explosion::render ( void )
{
	float wid = 100.0f; // size of explosion sprite
	glColor3f ( 1.0, 1.0, 1.0 );
	glPushMatrix();
	glTranslatef ( current.x_pos, current.y_pos, 0 );
	glBindTexture ( GL_TEXTURE_2D, explosionTexture[0] );
	for ( int i=0; i<5; i++ ) {
		current.frame++;
		if ( current.frame >=i*30 && current.frame <=200 ) {
			glBindTexture ( GL_TEXTURE_2D, explosionTexture[i] );
		}
		if ( current.frame >=200 ) {
			current.x_pos = -500;
			current.y_pos = -500;
		}
	}
	if ( current.frame>500 )
		current.frame=500;
	glEnable ( GL_ALPHA_TEST );
	glAlphaFunc ( GL_GREATER, 0.0f );
	glColor4ub ( 255,255,255,255 );
	glBegin ( GL_QUADS );
	glTexCoord2f ( 0.0f, 1.0f );
	glVertex2i ( -wid,-wid );
	glTexCoord2f ( 0.0f, 0.0f );
	glVertex2i ( -wid, wid );
	glTexCoord2f ( 1.0f, 0.0f );
	glVertex2i ( wid, wid );
	glTexCoord2f ( 1.0f, 1.0f );
	glVertex2i ( wid,-wid );
	glEnd();
	glPopMatrix();
	glDisable ( GL_ALPHA_TEST );
	glDisable ( GL_TEXTURE_2D );
	glBlendFunc ( GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA );
	glEnable ( GL_BLEND );
	glDisable ( GL_BLEND );
	glEnable ( GL_TEXTURE_2D );
}
// end explosion render ========================================

// =============================================================
void RocketPack::render ( void )
{
	float wid = 30.0f; // size of splash sprite
	glColor3f ( 1.0, 1.0, 1.0 );
	glPushMatrix();
	glTranslatef ( current.x_pos, current.y_pos, 0 );
	glBindTexture ( GL_TEXTURE_2D, rocketPackTexture[0] );
	for ( int i=0; i<5; i++ ) {
		current.frame++;
		if ( current.frame >=i*30 && current.frame <=200 ) {
			glBindTexture ( GL_TEXTURE_2D, rocketPackTexture[0] );
		}
		if ( current.frame >=500 ) {
			current.x_pos = -500;
			current.y_pos = -500;
		}
	}
	if ( current.frame>1000 )
		current.frame=1000;
	if ( rand() %1000 == 1 ) {
		current.x_pos = WIDTH/2;
		current.y_pos = HEIGHT/2;
		current.frame =0;
	}
	glEnable ( GL_ALPHA_TEST );
	glAlphaFunc ( GL_GREATER, 0.0f );
	glColor4ub ( 255,255,255,255 );
	glBegin ( GL_QUADS );
	glTexCoord2f ( 0.0f, 1.0f );
	glVertex2i ( -wid,-wid );
	glTexCoord2f ( 0.0f, 0.0f );
	glVertex2i ( -wid, wid );
	glTexCoord2f ( 1.0f, 0.0f );
	glVertex2i ( wid, wid );
	glTexCoord2f ( 1.0f, 1.0f );
	glVertex2i ( wid,-wid );
	glEnd();
	glPopMatrix();
	glDisable ( GL_ALPHA_TEST );
	glDisable ( GL_TEXTURE_2D );
	glBlendFunc ( GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA );
	glEnable ( GL_BLEND );
	glDisable ( GL_BLEND );
	glEnable ( GL_TEXTURE_2D );
}
// end rocketpack render ===========================================



void Turtle::render ( void )
{
	int r = rand() %600+1;
	int goldenChance = rand() %2000+1;
	current.x_pos += current.x_vel;
	current.y_pos += current.y_vel;
	//std::cout<<"turtle="<<current.frame<<","<<current.y_pos<< std::endl;
	if ( current.x_pos > WIDTH +100 ) {
		current.x_pos = 0-100;
		current.y_pos = HEIGHT-r;
	}
	if ( goldenChance==1 || goldenFrames>0 ) {
		golden = true;
		goldenFrames++;
	}
	if ( goldenFrames>500 ) {
		golden = false;
		goldenFrames =0;
	}
	float wid = 40.0f; // size of turtlesprite
	glColor3f ( 1.0, 1.0, 1.0 );
	glPushMatrix();
	glTranslatef ( current.x_pos, current.y_pos, 0 );
	glBindTexture ( GL_TEXTURE_2D, turtleTexture[0] );
	if (  current.frame <20 ) {
		if ( !golden )
			glBindTexture ( GL_TEXTURE_2D, turtleTexture[0] );
		else
			glBindTexture ( GL_TEXTURE_2D, turtleTexture[6] );
		current.frame++;
	}
	if ( current.frame>=20 && current.frame <=30 ) {
		if ( !golden )
			glBindTexture ( GL_TEXTURE_2D, turtleTexture[1] );
		else
			glBindTexture ( GL_TEXTURE_2D, turtleTexture[7] );
		current.frame++;
	}
	if ( current.frame>=30 && current.frame <=40 ) {
		if ( !golden )
			glBindTexture ( GL_TEXTURE_2D, turtleTexture[2] );
		else
			glBindTexture ( GL_TEXTURE_2D, turtleTexture[8] );
		current.frame++;
	}
	if ( current.frame>=40 && current.frame <=50 ) {
		if ( !golden )
			glBindTexture ( GL_TEXTURE_2D, turtleTexture[3] );
		else
			glBindTexture ( GL_TEXTURE_2D, turtleTexture[9] );
		current.frame++;
	}
	if ( current.frame>=50 && current.frame <=60 ) {
		if ( !golden )
			glBindTexture ( GL_TEXTURE_2D, turtleTexture[4] );
		else
			glBindTexture ( GL_TEXTURE_2D, turtleTexture[10] );
		current.frame++;
	}
	if ( current.frame>=60 ) {
		if ( !golden )
			glBindTexture ( GL_TEXTURE_2D, turtleTexture[5] );
		else
			glBindTexture ( GL_TEXTURE_2D, turtleTexture[11] );
		current.frame++;
	}
	if ( current.frame>100 )
		current.frame=0;
	glEnable ( GL_ALPHA_TEST );
	glAlphaFunc ( GL_GREATER, 0.0f );
	glColor4ub ( 255,255,255,255 );
	glBegin ( GL_QUADS );
	glTexCoord2f ( 0.0f, 1.0f );
	glVertex2i ( -wid,-wid );
	glTexCoord2f ( 0.0f, 0.0f );
	glVertex2i ( -wid, wid );
	glTexCoord2f ( 1.0f, 0.0f );
	glVertex2i ( wid, wid );
	glTexCoord2f ( 1.0f, 1.0f );
	glVertex2i ( wid,-wid );
	glEnd();
	glPopMatrix();
	glDisable ( GL_ALPHA_TEST );
	glDisable ( GL_TEXTURE_2D );
	glBlendFunc ( GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA );
	glEnable ( GL_BLEND );
	glDisable ( GL_BLEND );
	glEnable ( GL_TEXTURE_2D );
}
// end Turtle render ===============================================


// Render fly =============================================
void Fly::render ( float sze )
{
	float wid = sze; //20.0f; // size of fly sprite
	glColor3f ( 1.0, 1.0, 1.0 );
	if ( alive ) {
		if ( current.y_pos<30 ) {
			current.y_pos=40;
			current.y_vel=1;
		}
		if ( current.y_pos  > HEIGHT+100 ) {
			current.y_pos   =  rand() %HEIGHT+1;
			current.x_pos   =  WIDTH - rand() %WIDTH+1;
		}
		if ( current.x_pos<0 ) {
			current.x_pos=30;
		}
		if ( current.x_pos>WIDTH ) {
			current.x_pos=WIDTH-20;
		}
	} else {
		current=previous;
		current.x_vel = -200;
		current.y_vel = -200;
		//draw a line for frogs toungue
		glLineWidth ( 2.5 );
		glColor3f ( 200.0, 0.0, 0.0 );
		glBindTexture ( GL_TEXTURE_2D, 0 );
		glBegin ( GL_LINES );
		// fly position
		glVertex2f ( current.x_pos,current.y_pos );
		//frogs position
		glVertex2f ( frog.x_pos, frog.y_pos+10 );
		glEnd();
		deadTime++;
		if ( deadTime>8 ) {
			int r = rand() %600+1;
			alive = true;
			current.x_pos=WIDTH-r;
			current.y_pos= HEIGHT;
			current.frame =0;
			current.x_vel=0.1;
			current.y_vel=0.1;
			deadTime=0;
		}
	}
	previous=current;
	glPushMatrix();
	glTranslatef ( current.x_pos, current.y_pos, 0 );
	glBindTexture ( GL_TEXTURE_2D, flyTexture[0] );
	// ===========================
	current.frame++;
	if ( current.frame >= 200 )
		current.frame =0;
	//randomize flipping
	int r = rand() %200+1;
	if ( r == 3 && flip <= 0 )
		flip=100;
	if ( current.frame <= 40 ) {
		glBindTexture ( GL_TEXTURE_2D, flyTexture[1] );
	}
	if ( current.frame >= 20 && current.frame<=80 ) {
		glBindTexture ( GL_TEXTURE_2D, flyTexture[2] );
	}
	if ( flip>0 ) {
		flip--;
		if ( current.frame > 30 && current.frame <= 50 ) {
			glBindTexture ( GL_TEXTURE_2D, flyTexture[3] );
		}
		if ( current.frame > 50 && current.frame <= 70 ) {
			glBindTexture ( GL_TEXTURE_2D, flyTexture[4] );
			current.frame+=5;
		}
		if ( current.frame > 70 && current.frame <= 90 ) {
			glBindTexture ( GL_TEXTURE_2D, flyTexture[5] );
			current.frame+=5;
		}
		if ( current.frame > 90 && current.frame<=100 ) {
			glBindTexture ( GL_TEXTURE_2D, flyTexture[6] );
			current.frame+=5;
		}
		if ( current.frame > 100 && current.frame<=120 ) {
			glBindTexture ( GL_TEXTURE_2D, flyTexture[7] );
			current.frame+=5;
		}
		if ( current.frame > 120 && current.frame<=140 ) {
			glBindTexture ( GL_TEXTURE_2D, flyTexture[8] );
			current.frame+=5;
		}
		if ( current.frame > 140 && current.frame<=150 ) {
			glBindTexture ( GL_TEXTURE_2D, flyTexture[9] );
			current.frame+=5;
		}
		if ( current.frame>160 ) {
			glBindTexture ( GL_TEXTURE_2D, flyTexture[10] );
			current.frame+=5;
		}
	}
	previous = current;
	glEnable ( GL_ALPHA_TEST );
	glAlphaFunc ( GL_GREATER, 0.0f );
	glColor4ub ( 255,255,255,255 );
	glBegin ( GL_QUADS );
	glTexCoord2f ( 0.0f, 1.0f );
	glVertex2i ( -wid,-wid );
	glTexCoord2f ( 0.0f, 0.0f );
	glVertex2i ( -wid, wid );
	glTexCoord2f ( 1.0f, 0.0f );
	glVertex2i ( wid, wid );
	glTexCoord2f ( 1.0f, 1.0f );
	glVertex2i ( wid,-wid );
	glEnd();
	glPopMatrix();
	glDisable ( GL_ALPHA_TEST );
	glDisable ( GL_TEXTURE_2D );
	glBlendFunc ( GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA );
	glEnable ( GL_BLEND );
	glDisable ( GL_BLEND );
	glEnable ( GL_TEXTURE_2D );
} //end fly render=============================================


void Meter::render ( void )
{
	if ( current.x_pos<-100 ) {
		//meter is offscreen
	}
	float wid = 80.0f; // size of meter sprite
	glColor3f ( 1.0, 1.0, 1.0 );
	glPushMatrix();
	glTranslatef ( current.x_pos, current.y_pos, 0 );
	glBindTexture ( GL_TEXTURE_2D, meterTexture[0] );
	glBindTexture ( GL_TEXTURE_2D, meterTexture[0] );
	glEnable ( GL_ALPHA_TEST );
	glAlphaFunc ( GL_GREATER, 0.0f );
	glColor4ub ( 255,255,255,255 );
	glBegin ( GL_QUADS );
	glTexCoord2f ( 0.0f, 1.0f );
	glVertex2i ( -wid,-wid );
	glTexCoord2f ( 0.0f, 0.0f );
	glVertex2i ( -wid, wid );
	glTexCoord2f ( 1.0f, 0.0f );
	glVertex2i ( wid, wid );
	glTexCoord2f ( 1.0f, 1.0f );
	glVertex2i ( wid,-wid );
	glEnd();
	glPopMatrix();
	glDisable ( GL_ALPHA_TEST );
	glDisable ( GL_TEXTURE_2D );
	glBlendFunc ( GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA );
	glEnable ( GL_BLEND );
	glDisable ( GL_BLEND );
	glEnable ( GL_TEXTURE_2D );
}
// end meter render ===========================================

//read high scores XML ===============================
std::string loadScores ( int player )
{
	int counter=0;
	XMLDocument* doc = new XMLDocument();
	doc->LoadFile ( "highscore.xml" );
	int errorID = doc->ErrorID();
	std::string err = doc->ErrorName();
	std::string playerString[20][3];
	XMLPrinter printer;
	//std::cout << "Test file loaded. ErrorID = "<<errorID<<" "<<err<<std::endl;
	if ( errorID!=0 )
		return "";
	XMLNode* root = doc->FirstChildElement ( "players" );
	if ( root == NULL ) {
		std::cout<<"error xml root"<<std::endl;
		return "";
	}
	XMLElement* node = root->FirstChildElement ( "player" )->ToElement();
	if ( node == NULL ) {
		std::cout<<"error xml"<<std::endl;
		return "";
	}
	while ( node->NextSiblingElement() !=NULL && counter<=player ) {
		XMLElement* element = node->FirstChildElement ( "name" )->ToElement();
		if ( element == NULL ) {
			std::cout<<"error xml"<<std::endl;
			return "";
		}
		playerString[counter][0] = element->GetText();
		//std::cout <<"playerstring="<<playerString[counter][0]<<std::endl;
		element = element->NextSiblingElement ( "score" )->ToElement();
		if ( element == NULL ) {
			std::cout<<"error xml"<<std::endl;
			return "";
		}
		playerString[counter][1] = element->GetText();
		element = element->NextSiblingElement ( "mode" )->ToElement();
		if ( element == NULL ) {
			std::cout<<"error xml"<<std::endl;
			return "";
		}
		playerString[counter][2] = element->GetText();
		node = node->NextSiblingElement ( "player" )->ToElement();
		if ( node == NULL ) {
			std::cout<<"error xml"<<std::endl;
			return "";
		}
		counter++;
	}
	return playerString[player][0]+", "+playerString[player][1]+", "
		+playerString[player][2];
}
// end read high scores =========================

// =============================================================
unsigned char *buildAlphaData ( Ppmimage *img )
{
	//add 4th component to RGB stream...
	int i;
	int a,b,c;
	unsigned char *newdata, *ptr;
	unsigned char *data = ( unsigned char * ) img->data;
	newdata = ( unsigned char * ) malloc ( img->width * img->height * 4 );
	ptr = newdata;
	for ( i=0; i<img->width * img->height * 3; i+=3 ) {
		a = * ( data+0 );
		b = * ( data+1 );
		c = * ( data+2 );
		* ( ptr+0 ) = a;
		* ( ptr+1 ) = b;
		* ( ptr+2 ) = c;
		* ( ptr+3 ) = ( a|b|c );
		ptr += 4;
		data += 3;
	}
	return newdata;
}
//end johnH.cpp =================================================
