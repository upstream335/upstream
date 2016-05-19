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
	game->lives = 3;
	game->difficulty = MED;
	game->demo.on = false;
	game->demo.jump     =   0;
	game->demo.moveLeft =   0;
	game->demo.moveRight =  0;
	game->score = 0;
	game->scoreCount = 0;
	game->muted = false;
	game->n = 0;
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
}

void IntroBG::render(void)
{
	float wid = 300.0f;
	glColor3f ( 1.0, 1.0, 1.0 );
	glPushMatrix();

	glTranslatef ( current.x_pos-300, current.y_pos-300, 0 );
	glBindTexture ( GL_TEXTURE_2D, introbgTexture[0] );
	glEnable ( GL_ALPHA_TEST );
	glAlphaFunc ( GL_GREATER, 0.0f );
	glColor4ub ( 255,255,255,255 );

	glBegin ( GL_QUADS );
	glTexCoord2f ( 0.0f, 1.0f );
	glVertex2i ( -wid,-wid-150);
	glTexCoord2f ( 0.0f, 0.0f );
	glVertex2i ( -wid, wid+150 );
	glTexCoord2f ( 1.0f, 0.0f );
	glVertex2i ( wid, wid+150 );
	glTexCoord2f ( 1.0f, 1.0f );
	glVertex2i ( wid,-wid-150 );
	glEnd();

	glPopMatrix();

	glDisable ( GL_ALPHA_TEST );
	glDisable ( GL_TEXTURE_2D );
	glBlendFunc ( GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA );
	glEnable ( GL_BLEND );
	glDisable ( GL_BLEND );
	glEnable ( GL_TEXTURE_2D );
}

void PausedBG::render(void)
{
	float wid = 250.0f;
	glColor3f ( 1.0, 1.0, 1.0 );
	glPushMatrix();

	glTranslatef ( current.x_pos-300, current.y_pos-300, 0 );
	glBindTexture ( GL_TEXTURE_2D, pausedbgTexture );
	glEnable ( GL_ALPHA_TEST );
	glAlphaFunc ( GL_GREATER, 0.0f );
	glColor4ub ( 255,255,255,255 );

	glBegin ( GL_QUADS );
	glTexCoord2f ( 0.0f, 1.0f );
	glVertex2i ( -wid,-wid);
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
	//initialize all in-game buttons
	game->nbuttons=0;
	/************MAIN-MENU BOTTONS******************/

	int n = 0;

	//Main menu bottons
	for (int i=0; i<5; i++) {
		game->button[i].r.width = 120;
		game->button[i].r.height = 40;
		game->button[i].r.left = 50;
		game->button[i].r.bot = 480-n;
		game->button[i].r.right =
			game->button[i].r.left +
			game->button[i].r.width;
		game->button[i].r.top =
			game->button[i].r.bot +
			game->button[i].r.height;
		game->button[i].r.centerx =
			(game->button[i].r.left +
			 game->button[i].r.right) / 2;
		game->button[i].r.centery =
			(game->button[i].r.bot +
			 game->button[i].r.top) / 2;
		game->button[i].down = 0;
		game->button[i].click = 0;
		game->button[i].color[0] = 0.3f;
		game->button[i].color[1] = 0.7f;
		game->button[i].color[2] = 0.3f;
		game->button[i].text_color = 0x00ffffff;
		game->nbuttons++;
		n+=50;
	}

	strcpy(game->button[0].text, "PLAY");
	strcpy(game->button[1].text, "MEDIUM");
	strcpy(game->button[2].text, "HIGHSCORE");
	strcpy(game->button[3].text, "CREDITS");
	strcpy(game->button[4].text, "EXIT");

	n = 0;
	//Sub-menu buttons
	for (int i=5; i<10; i++) {
		game->button[i].r.width = 120;
		game->button[i].r.height = 40;
		game->button[i].r.left = 250;
		game->button[i].r.bot = 475-n;
		game->button[i].r.right =
			game->button[i].r.left +
			game->button[i].r.width;
		game->button[i].r.top =
			game->button[i].r.bot +
			game->button[i].r.height;
		game->button[i].r.centerx =
			(game->button[i].r.left +
			 game->button[i].r.right) / 2;
		game->button[i].r.centery =
			(game->button[i].r.bot +
			 game->button[i].r.top) / 2;
		game->button[i].down = 0;
		game->button[i].click = 0;
		game->button[i].color[0] = 0.3f;
		game->button[i].color[1] = 0.7f;
		game->button[i].color[2] = 0.3f;
		game->button[i].text_color = 0x00ffffff;
		game->nbuttons++;
		n+=50;
	}

	n = 0;
	//Sub-menu buttons
	for (int i=10; i<13; i++) {
		game->button[i].r.width = 120;
		game->button[i].r.height = 40;
		game->button[i].r.left = 250;
		game->button[i].r.bot = 475-n;
		game->button[i].r.right =
			game->button[i].r.left +
			game->button[i].r.width;
		game->button[i].r.top =
			game->button[i].r.bot +
			game->button[i].r.height;
		game->button[i].r.centerx =
			(game->button[i].r.left +
			 game->button[i].r.right) / 2;
		game->button[i].r.centery =
			(game->button[i].r.bot +
			 game->button[i].r.top) / 2;
		game->button[i].down = 0;
		game->button[i].click = 0;
		game->button[i].color[0] = 0.3f;
		game->button[i].color[1] = 0.7f;
		game->button[i].color[2] = 0.3f;
		game->button[i].text_color = 0x00ffffff;
		game->nbuttons++;
		n+=50;
	}

	strcpy(game->button[0].text, "PLAY");
	strcpy(game->button[1].text, "MEDIUM");
	strcpy(game->button[2].text, "HIGHSCORE");
	strcpy(game->button[3].text, "CREDITS");
	strcpy(game->button[4].text, "EXIT");

	strcpy(game->button[5].text, "RESUME");
	strcpy(game->button[6].text, "SOUND");
	strcpy(game->button[7].text, "RESTART");
	strcpy(game->button[8].text, "MAIN MENU");
	strcpy(game->button[9].text, "EXIT");

	strcpy(game->button[10].text, "PLAY AGAIN");
	strcpy(game->button[11].text, "MAIN MENU");
	strcpy(game->button[12].text, "QUIT");
}

void render_main_menu(Game *game)
{
	glClear ( GL_COLOR_BUFFER_BIT );
	game->introbg->render();
	game->frog->render();
	render_main_menu_buttons(game);
}

void render_sub_menu(Game *game)
{
	game->pausedbg->render();
	render_sub_menu_buttons(game);
}

void render_gameover_menu(Game *game)
{
	game->pausedbg->render();
	render_gameover_menu_buttons(game);
}

void render_main_menu_buttons(Game *game)
{
	Rect r;
	//draw main menu buttons
	glBindTexture(GL_TEXTURE_2D, 0);
	for (int i=0; i<5; i++) {
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
		ggprint16(&r, 0, game->button[i].text_color, game->button[i].text);
	}
}

void render_sub_menu_buttons(Game *game)
{
	Rect r;
	//draw paused menu buttons
	glBindTexture(GL_TEXTURE_2D, 0);
	for (int i=5; i<10; i++) {
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
		ggprint16(&r, 0, game->button[i].text_color, game->button[i].text);
	}
}

void render_gameover_menu_buttons(Game *game)
{
	Rect r;
	//draw gameover menu buttons
	glBindTexture(GL_TEXTURE_2D, 0);
	for (int i=10; i<13; i++) {
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
		ggprint16(&r, 0, game->button[i].text_color, game->button[i].text);
	}
}
