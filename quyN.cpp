// Author: Quy Nguyen
// Date written:  04-26-16
// Last modified: 05-02-16
// Program: Game Menus and Prompts
//          Game Introduction
//          Game Attract Mode
//
// Purpose: I will be in charge of the game menu, introduction,
//          attract mode (hard, medium, easy). I will attempt to 
//          make a generic function for each.
//
// 1st goal (week 6): Game Menu and Prompts (checked)
// 2nd goal (week 6): Game Introduction (in-progress)
// 3rd goal (week 7): Game Attract Mode
// 4th goal (week 8-9): added later...
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <cstring>
#include <stdio.h>
#include <cmath>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <stdlib.h>

#include "ppm.h"
#include "log.h"
#include "fonts.h"

#include "quyN.h"
#include "kevinJ.h"

void reset_game(Game *game)
{
	//reset the game
	game->difficulty = MED;
	game->demo.on = false;
	game->demo.jump     =   0;
	game->demo.moveLeft =   0;
	game->demo.moveRight =  0;
	game->score = 0;
	game->scoreCount=0;
	game->muted = false;
	game->n=0;
	game->ihead = NULL;
	game->nlily = 0;
	game->timer = 0;
	game->maxtimer = 35;
	game->lilytimer = 35;
	game->lilyspawnpoint = game->windowHeight + 15;
	game->c.radius = 15.0;
	game->c.center[0] = game->windowWidth/2.0;
	game->c.center[1] = 15;
	game->c.detail = 150;
	game->c.jumpSpeed = 0;
	game->c.jumpSpeedMax = 25;
	game->playing=true;
	for ( int i = 0; i < 100; i++ ) {
		game->highscore[i] = 0;
	}
}

void IntroBG::render(void)
{
	float wid = 300.0f; // size of introbg
	glColor3f ( 1.0, 1.0, 1.0 );
	glPushMatrix();

	glTranslatef ( current.x_pos-300, current.y_pos-300, 0 );
	glBindTexture ( GL_TEXTURE_2D, introbgTexture[0] );
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

void init_buttons(Game *game)
{
	//initialize buttons
	game->nbuttons=0;
	//size and position

	//Play button
	strcpy(game->button[game->nbuttons].text, "PLAY");
	game->button[game->nbuttons].r.width = 140;
	game->button[game->nbuttons].r.height = 60;
	game->button[game->nbuttons].r.left = 100;
	game->button[game->nbuttons].r.bot = 160;
	game->button[game->nbuttons].r.right =
		game->button[game->nbuttons].r.left +
		game->button[game->nbuttons].r.width;
	game->button[game->nbuttons].r.top =
		game->button[game->nbuttons].r.bot +
		game->button[game->nbuttons].r.height;
	game->button[game->nbuttons].r.centerx =
		(game->button[game->nbuttons].r.left +
		 game->button[game->nbuttons].r.right) / 2;
	game->button[game->nbuttons].r.centery =
		(game->button[game->nbuttons].r.bot +
		 game->button[game->nbuttons].r.top) / 2;
	game->button[game->nbuttons].down = 0;
	game->button[game->nbuttons].click = 0;
	game->button[game->nbuttons].color[0] = 0.4f;
	game->button[game->nbuttons].color[1] = 0.4f;
	game->button[game->nbuttons].color[2] = 0.7f;
	game->button[game->nbuttons].dcolor[0] =
		game->button[game->nbuttons].color[0] * 0.5f;
	game->button[game->nbuttons].dcolor[1] =
		game->button[game->nbuttons].color[1] * 0.5f;
	game->button[game->nbuttons].dcolor[2] =
		game->button[game->nbuttons].color[2] * 0.5f;
	game->button[game->nbuttons].text_color = 0x00ffffff;
	game->nbuttons++;

	//Quit button
	strcpy(game->button[game->nbuttons].text, "QUIT");
	game->button[game->nbuttons].r.width = 140;
	game->button[game->nbuttons].r.height = 60;
	game->button[game->nbuttons].r.left = 350;
	game->button[game->nbuttons].r.bot = 160;
	game->button[game->nbuttons].r.right =
		game->button[game->nbuttons].r.left +
		game->button[game->nbuttons].r.width;
	game->button[game->nbuttons].r.top =
		game->button[game->nbuttons].r.bot +
		game->button[game->nbuttons].r.height;
	game->button[game->nbuttons].r.centerx =
		(game->button[game->nbuttons].r.left +
		 game->button[game->nbuttons].r.right) / 2;
	game->button[game->nbuttons].r.centery =
		(game->button[game->nbuttons].r.bot +
		 game->button[game->nbuttons].r.top) / 2;
	game->button[game->nbuttons].down = 0;
	game->button[game->nbuttons].click = 0;
	game->button[game->nbuttons].color[0] = 0.3f;
	game->button[game->nbuttons].color[1] = 0.3f;
	game->button[game->nbuttons].color[2] = 0.6f;
	game->button[game->nbuttons].dcolor[0] =
		game->button[game->nbuttons].color[0] * 0.5f;
	game->button[game->nbuttons].dcolor[1] =
		game->button[game->nbuttons].color[1] * 0.5f;
	game->button[game->nbuttons].dcolor[2] =
		game->button[game->nbuttons].color[2] * 0.5f;
	game->button[game->nbuttons].text_color = 0x00ffffff;
	game->nbuttons++;

	//Continue button
	strcpy(game->button[game->nbuttons].text, "CONTINUE");
	game->button[game->nbuttons].r.width = 140;
	game->button[game->nbuttons].r.height = 60;
	game->button[game->nbuttons].r.left = 230;
	game->button[game->nbuttons].r.bot = 400;
	game->button[game->nbuttons].r.right =
		game->button[game->nbuttons].r.left +
		game->button[game->nbuttons].r.width;
	game->button[game->nbuttons].r.top =
		game->button[game->nbuttons].r.bot +
		game->button[game->nbuttons].r.height;
	game->button[game->nbuttons].r.centerx =
		(game->button[game->nbuttons].r.left +
		 game->button[game->nbuttons].r.right) / 2;
	game->button[game->nbuttons].r.centery =
		(game->button[game->nbuttons].r.bot +
		 game->button[game->nbuttons].r.top) / 2;
	game->button[game->nbuttons].down = 0;
	game->button[game->nbuttons].click = 0;
	game->button[game->nbuttons].color[0] = 0.3f;
	game->button[game->nbuttons].color[1] = 0.3f;
	game->button[game->nbuttons].color[2] = 0.6f;
	game->button[game->nbuttons].dcolor[0] =
		game->button[game->nbuttons].color[0] * 0.5f;
	game->button[game->nbuttons].dcolor[1] =
		game->button[game->nbuttons].color[1] * 0.5f;
	game->button[game->nbuttons].dcolor[2] =
		game->button[game->nbuttons].color[2] * 0.5f;
	game->button[game->nbuttons].text_color = 0x00ffffff;
	game->nbuttons++;
}

void render_main_menu(Game *game)
{
	glClear ( GL_COLOR_BUFFER_BIT );
	game->introbg->render();
	render_menu_buttons(game);
}

void render_sub_menu(Game *game)
{
	//glClear ( GL_COLOR_BUFFER_BIT );
	render_continue_button(game);
}

void render_menu_buttons(Game *game)
{
	Rect r;
	int i;
	//draw all buttons
	glBindTexture(GL_TEXTURE_2D, 0);
	for (i=0; i<game->nbuttons-1; i++) {
		if (game->button[i].over) {
			int w=2;
			glColor3f(1.0f, 1.0f, 0.0f);
			//draw a highlight around button
			glLineWidth(3);
			glBegin(GL_LINE_LOOP);
			glVertex2i(game->button[i].r.left-w,  game->button[i].r.bot-w);
			glVertex2i(game->button[i].r.left-w,  game->button[i].r.top+w);
			glVertex2i(game->button[i].r.right+w, game->button[i].r.top+w);
			glVertex2i(game->button[i].r.right+w, game->button[i].r.bot-w);
			glVertex2i(game->button[i].r.left-w,  game->button[i].r.bot-w);
			glEnd();
			glLineWidth(1);
		}
		if (game->button[i].down) {
			glColor3fv(game->button[i].dcolor);
		} else {
			glColor3fv(game->button[i].color);
		}
		glBegin(GL_QUADS);
		glVertex2i(game->button[i].r.left,  game->button[i].r.bot);
		glVertex2i(game->button[i].r.left,  game->button[i].r.top);
		glVertex2i(game->button[i].r.right, game->button[i].r.top);
		glVertex2i(game->button[i].r.right, game->button[i].r.bot);
		glEnd();
		r.left = game->button[i].r.centerx;
		r.bot  = game->button[i].r.centery-8;
		r.center = 1;
		if (game->button[i].down) {
			ggprint16(&r, 0, game->button[i].text_color, "Pressed!");
		} else {
			ggprint16(&r, 0, game->button[i].text_color, game->button[i].text);
		}	
	} 
}

void render_continue_button(Game *game)
{
	Rect r;
	int i;
	//draw all buttons
	glBindTexture(GL_TEXTURE_2D, 0);
	for (i=2; i<3; i++) {
		if (game->button[i].over) {
			int w=2;
			glColor3f(1.0f, 1.0f, 0.0f);
			//draw a highlight around button
			glLineWidth(3);
			glBegin(GL_LINE_LOOP);
			glVertex2i(game->button[i].r.left-w,  game->button[i].r.bot-w);
			glVertex2i(game->button[i].r.left-w,  game->button[i].r.top+w);
			glVertex2i(game->button[i].r.right+w, game->button[i].r.top+w);
			glVertex2i(game->button[i].r.right+w, game->button[i].r.bot-w);
			glVertex2i(game->button[i].r.left-w,  game->button[i].r.bot-w);
			glEnd();
			glLineWidth(1);
		}
		if (game->button[i].down) {
			glColor3fv(game->button[i].dcolor);
		} else {
			glColor3fv(game->button[i].color);
		}
		glBegin(GL_QUADS);
		glVertex2i(game->button[i].r.left,  game->button[i].r.bot);
		glVertex2i(game->button[i].r.left,  game->button[i].r.top);
		glVertex2i(game->button[i].r.right, game->button[i].r.top);
		glVertex2i(game->button[i].r.right, game->button[i].r.bot);
		glEnd();
		r.left = game->button[i].r.centerx;
		r.bot  = game->button[i].r.centery-8;
		r.center = 1;
		if (game->button[i].down) {
			ggprint16(&r, 0, game->button[i].text_color, "Pressed!");
		} else {
			ggprint16(&r, 0, game->button[i].text_color, game->button[i].text);
		}	
	} 
}

void init_sound()
{

}

void cleanup_sound()
{

}

void play_sound(ALuint source)
{

}

