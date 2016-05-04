//cs335 john hargreaves
//animation classes for
//frog,log,gator,bridge
//splash and background water
#include "johnH.h"
Ppmimage* get_image(std::string filename)
{
	std::string filename2 = filename;
	filename2 = filename + ".ppm";
	std::string command = "convert " + filename + ".png " + filename2;
	system( (command).c_str() );
	Ppmimage *tempimage = ppm6GetImage( (filename2).c_str() );
	command ="rm " + filename2;
	system( (command).c_str() );
	return tempimage;
}
Position update_position(Position *c,float xp,
                         float yp,float xv,float yv)
{
	c->x_pos = xp;
	c->y_pos = yp;
	c->x_vel = xv;
	c->y_vel = yv;
	c->x_pos += c->x_vel;
	c->y_pos += c->y_vel;
	return *c;
}
// Render frog =============================================
void Frog::render(void)
{
//std::cout<<"frog position x,y ="<<current.x_pos<<","<<current.y_pos<<std::endl;
//std::cout<<"frog velocity x,y ="<<current.x_vel<<","<<current.y_vel<<std::endl;
//std::cout<<"frog prev position x,y ="<<previous.x_pos<<","<<previous.y_pos<<std::endl;
//std::cout<<"frog prev velocity x,y ="<<previous.x_vel<<","<<previous.y_vel<<std::endl;
//std::cout<<"frog current frame, prev frame ="<<current.frame<<","<<previous.frame<<std::endl;
	float wid = 30.0f; // size of frog sprite
	glColor3f(1.0, 1.0, 1.0);
	if (current.y_pos<30)
		{
		current.y_pos=40;
		current.y_vel=0;
		}
	if (current.y_pos>100) 
		{
		current.x_vel=0;
		}
	glPushMatrix();
	glTranslatef(current.x_pos, current.y_pos, 0);
	glBindTexture(GL_TEXTURE_2D, frogTexture[0]);
// JUMPING UP===========================
	if (current.frame >= 30 && current.y_vel>0)
		current.frame =0;
	if (current.x_vel <= 0 && current.frame <= 5 && current.y_vel>0) 
		{
		glBindTexture(GL_TEXTURE_2D, frogTexture[1]);
		current.frame++;
		//std::cout<<"up one"<<std::endl;
		}
	if ( current.x_vel <= 0 && current.frame <= 10 &&
	    current.frame > 5 && current.y_vel>0) 
			{
		glBindTexture(GL_TEXTURE_2D, frogTexture[2]);
		current.frame++;
		//std::cout<<"up two"<<std::endl;
		}
	if (current.x_vel <= 0 && current.frame <= 20 &&
	   current.frame > 10 && current.y_vel>0) 
		   {
		glBindTexture(GL_TEXTURE_2D, frogTexture[3]);
		current.frame++;
		//std::cout<<"up three"<<std::endl;
		}
// JUMPING DOWN ===================================
	if (current.x_vel <= 0 && current.frame >= 15 && current.y_vel<0) 
		{
		glBindTexture(GL_TEXTURE_2D, frogTexture[3]);
		current.frame--;
		//std::cout<<"down one"<<std::endl;
	}
	if (current.x_vel <= 0 && current.frame >= 5 &&
	   current.frame<=10 && current.y_vel<0) 
		   {
		glBindTexture(GL_TEXTURE_2D, frogTexture[4]);
		current.frame--;
		//std::cout<<"down two"<<std::endl;
		}
	if (current.x_vel <= 0 && current.frame <= 5 && current.y_vel<0) 
		{
		glBindTexture(GL_TEXTURE_2D, frogTexture[5]);
		current.frame--;
	// std::cout<<"down three"<<std::endl;
	}
// ==================================================
//moving along bridge
	bool going_left = true;
	if (previous.x_pos < current.x_pos ) 
		{ //&& current.y_pos <= 50 && current.frame <-30)
		going_left = false;
	}
	if (!going_left && current.y_vel<=0)
	{
		glBindTexture(GL_TEXTURE_2D, frogTexture[7]);
		current.frame++;
		if(current.frame >= 40) 
			{
			glBindTexture(GL_TEXTURE_2D, frogTexture[8]);
			if(current.frame > 80)
				current.frame = 0;
		}
	}
	if (going_left && current.y_vel<=0) 
		{
		glBindTexture(GL_TEXTURE_2D, frogTexture[9]);
		current.frame++;
		if (current.frame >= 40) 
			{
			glBindTexture(GL_TEXTURE_2D, frogTexture[10]);
			if (current.frame > 80)
				current.frame = 0;
		}
	}
//sitting still
	if (current.x_vel==0 && current.y_vel==0 && current.x_pos== previous.x_pos) 
		{
		glBindTexture(GL_TEXTURE_2D, frogTexture[0]);
		current.frame = 0;
	}
	if (current.x_vel==0 && current.y_vel<0)
		glBindTexture(GL_TEXTURE_2D, frogTexture[5]);
	previous = current;
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex2i(-wid,-wid);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2i(-wid, wid);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2i( wid, wid);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2i( wid,-wid);
	glEnd();
	glPopMatrix();
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
} //end frog render=============================================
void Bridge::render(void)
{
	if (current.x_pos<-100) 
		{
	}
	float wid = 500.0f; // size of bridge sprite
	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();
	glTranslatef(current.x_pos, current.y_pos, 0);
	glBindTexture(GL_TEXTURE_2D, bridgeTexture[0]);
	//std::cout << " going back pos[0]= " << x_pos << std::endl;
	glBindTexture(GL_TEXTURE_2D, bridgeTexture[0]);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
	glBegin(GL_QUADS);
	//if (x_vel > 0.0)
	//{
	glTexCoord2f(0.0f, 1.0f);
	glVertex2i(-wid,-wid);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2i(-wid, wid);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2i( wid, wid);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2i( wid,-wid);
	glEnd();
	glPopMatrix();
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
}
// end bridge render ===========================================
void Log::render(void)
{
	int r = rand()%600+1;
	current.x_pos += current.x_vel;
	current.y_pos += current.y_vel;
	if (current.y_pos<-100) {
		current.x_pos = 500-r;
		current.y_pos = 800;
	}
	float wid = 60.0f; // size of logsprite
	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();
	glTranslatef(current.x_pos, current.y_pos, 0);
	glBindTexture(GL_TEXTURE_2D, logTexture[0]);
//std::cout << " going back pos[0]= " << current.x_pos << std::endl;
	if (current.frame>=10) 
		{
		glBindTexture(GL_TEXTURE_2D, logTexture[0]);
		current.frame++;
		if (current.frame>20)
			current.frame=0;
	}
	if (current.frame<10) 
		{
		glBindTexture(GL_TEXTURE_2D, logTexture[1]);
		current.frame++;
	}
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
	glBegin(GL_QUADS);
	//if (x_vel > 0.0)
	//{
	glTexCoord2f(0.0f, 1.0f);
	glVertex2i(-wid,-wid);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2i(-wid, wid);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2i( wid, wid);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2i( wid,-wid);
	glEnd();
	glPopMatrix();
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
}
// end Log render ==============================================
void Gator::render(void)
{
	int r = rand()%800+1;
	current.x_pos += current.x_vel;
	current.y_pos += current.y_vel;
	if (current.x_pos<0) 
		{
		current.x_pos = 1000;
		current.y_pos = 1200-r;
	}
	float wid = 70.0f; // size of gator sprite
	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();
	glTranslatef(current.x_pos, current.y_pos, 0);
	glBindTexture(GL_TEXTURE_2D, gatorTexture[0]);
	if (current.frame>=10) 
		{
		glBindTexture(GL_TEXTURE_2D, gatorTexture[0]);
		current.frame++;
		if (current.frame>20)
			current.frame=0;
	}
	if (current.frame<10) 
		{
		glBindTexture(GL_TEXTURE_2D, gatorTexture[1]);
		current.frame++;
	}
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
	glBegin(GL_QUADS);
	//if (x_vel > 0.0)
	//{
	glTexCoord2f(0.0f, 1.0f);
	glVertex2i(-wid,-wid);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2i(-wid, wid);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2i( wid, wid);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2i( wid,-wid);
	glEnd();
	glPopMatrix();
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
}
// end Gator render ============================================
void Water::render(void)
{
	current.x_pos += current.x_vel;
	current.y_pos += current.y_vel;
	if (current.y_pos<0) 
		{
		current.x_pos = 300;
		current.y_pos = 800;
	}
	float wid = 600.0f; // size of water sprite
	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();
	glTranslatef(current.x_pos, current.y_pos, 0);
	glBindTexture(GL_TEXTURE_2D, waterTexture[0]);
	if (current.frame<=10 ) 
		{
		glBindTexture(GL_TEXTURE_2D, waterTexture[0]);
		current.frame++;
	}
	if (current.frame>10 && current.frame <=20 ) 
		{
		glBindTexture(GL_TEXTURE_2D, waterTexture[1]);
		current.frame++;
	}
	if (current.frame>=20)
		current.frame=0;
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
	glBegin(GL_QUADS);
	//if (x_vel > 0.0)
	//{
	glTexCoord2f(0.0f, 1.0f);
	glVertex2i(-wid,-wid);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2i(-wid, wid);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2i( wid, wid);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2i( wid,-wid);
	glEnd();
	glPopMatrix();
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
}
// end water render ============================================
void Splash::render(void)
{
	float wid = 50.0f; // size of splash sprite
	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();
	glTranslatef(current.x_pos, current.y_pos, 0);
	glBindTexture(GL_TEXTURE_2D, splashTexture[0]);
	for(int i=0; i<5; i++) {
		current.frame++;
	//std::cout<<"frame="<<frame<<std::endl;
		if (current.frame >=i*30 && current.frame <=200) 
			{
			glBindTexture(GL_TEXTURE_2D, splashTexture[i]);
		}
		if (current.frame >=200) 
			{
			current.x_pos = -500;
			current.y_pos = -500;
		}
	}
	if (current.frame>500)
		current.frame=500;
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex2i(-wid,-wid);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2i(-wid, wid);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2i( wid, wid);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2i( wid,-wid);
	glEnd();
	glPopMatrix();
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
}
// end splash render ===========================================
void LP::render(void)
{
	int r = rand()%600+1;
	current.x_pos += current.x_vel;
	current.y_pos += current.y_vel;
	if (current.y_pos<-100) 
		{
		current.x_pos = 500-r;
		current.y_pos = 1400-r;
	}
	float wid = 20.0f; // size of lpsprite
	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();
	glTranslatef(current.x_pos, current.y_pos, 0);
	glBindTexture(GL_TEXTURE_2D, lpTexture[0]);
	//std::cout << " going back pos[0]= " << current.x_pos << std::endl;
	if (current.frame>=10) {
		glBindTexture(GL_TEXTURE_2D, lpTexture[0]);
		current.frame++;
		if(current.frame>20)
			current.frame=0;
	}
	if (current.frame<10)
		{
		glBindTexture(GL_TEXTURE_2D, lpTexture[1]);
		current.frame++;
	}
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
	glBegin(GL_QUADS);
	//if (x_vel > 0.0)
	//{
	glTexCoord2f(0.0f, 1.0f);
	glVertex2i(-wid,-wid);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2i(-wid, wid);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2i( wid, wid);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2i( wid,-wid);
	glEnd();
	glPopMatrix();
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
}
// end LP render ==============================================
// =============================================================
unsigned char *buildAlphaData(Ppmimage *img)
{
//add 4th component to RGB stream...
	int i;
	int a,b,c;
	unsigned char *newdata, *ptr;
	unsigned char *data = (unsigned char *)img->data;
	newdata = (unsigned char *)malloc(img->width * img->height * 4);
	ptr = newdata;
	for (i=0; i<img->width * img->height * 3; i+=3) 
		{
		a = *(data+0);
		b = *(data+1);
		c = *(data+2);
		*(ptr+0) = a;
		*(ptr+1) = b;
		*(ptr+2) = c;
		*(ptr+3) = (a|b|c);
		ptr += 4;
		data += 3;
	}
	return newdata;
}
//end johnH.cpp =================================================
