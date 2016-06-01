// Author: Quy Nguyen
// Date written:  04-26-16
// Last modified: 05-30-16
// Program: Game Menus and Prompts
//          Game Introduction
//          Game Attract Mode
//
// Purpose: I will be in charge of the game menu, introduction,
//          attract mode (hard, medium, easy). I will attempt to
//          make a generic function for each.
//
// 1st goal (week 6): Game Menu and Prompts (checked)
// 2nd goal (week 7): Game Introduction (checked)
// 3rd goal (week 8): Game Load button textures (checked)
// 4th goal (week 9): Fix bugs; edit io.cpp; added introduction sound
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <string>
#include <sstream>
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
	game->stresstest=0;
	game->x = game->windowWidth/2;
	game->troll_lilypad=0;
	game->help_menu=false;
	game->bossGO = false;
	game->lives = 1;
	game->demo.on = false;
	game->demo.jump     =   0;
	game->demo.moveLeft =   0;
	game->demo.moveRight =  0;
	game->score = 0;
	game->scoreCount = 0;
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
	game->frog->resetRocket();
	game->frog->move(WIDTH/2,40,0,0);
}

void render_main_menu(Game *game)
{
	glClear ( GL_COLOR_BUFFER_BIT );
	game->introbg->render();
	game->frog->render();
	render_main_menu_buttons(game);
	if (game->highscoreboard)
		render_highscore(game);
	if (game->credits)
		render_credits(game);
}

void render_sub_menu(Game *game)
{
	game->pausedbg->render();
	render_sub_menu_buttons(game);
}

void render_gameover_menu(Game *game)
{
	game->gameoverbg->render();
	render_gameover_menu_buttons(game);
}

void render_highscore(Game *game)
{
	Rect r;
	stringstream ss;
	string s;
	const char* c;
	game->highscorebg->render();
	glBindTexture ( GL_TEXTURE_2D, 0 );
	r.bot = game->windowHeight - 220;
	r.left = 400;
	ggfrog40b( &r, 40, 0, "HIGHSCORES:TOP10");
	for ( int i=0; i<10; i++ ) {
		game->highScores[i] = loadScores ( i );
		ss.str("");
		ss<<"player#"<<i+1<<" = "<<game->highScores[i];
		s = ss.str();
		c = s.c_str();
		r.left = 350;
		ggprint13 ( &r, 30, 0, c);
	}
}

void render_credits(Game *game)
{
	Rect r;
	game->highscorebg->render();
	glBindTexture ( GL_TEXTURE_2D, 0 );
	r.bot = game->windowHeight - 220;
	r.left = 400;
	ggfrog40b(  &r, 40, 1, "CREDITS:");
	r.left = 400;
	ggprint13 ( &r, 40, 1, "JONATHAN DINH");
	r.left = 400;
	ggprint13 ( &r, 40, 1, "JOHN HARGREAVES");
	r.left = 400;
	ggprint13 ( &r, 40, 1, "QUY NGUYEN");
	r.left = 400;
	ggprint13 ( &r, 40, 1, "KEVIN JENKIN");
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

void GameoverBG::render(void)
{
	float wid = 250.0f;
	glColor3f ( 1.0, 1.0, 1.0 );
	glPushMatrix();
	glTranslatef ( current.x_pos-300, current.y_pos-300, 0 );
	glBindTexture ( GL_TEXTURE_2D, gameoverTexture );
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

void highscoreBG::render(void)
{
	float wid = 220.0f;
	glColor3f ( 1.0, 1.0, 1.0 );
	glPushMatrix();
	glTranslatef ( current.x_pos-220, current.y_pos-320, 0 );
	glBindTexture ( GL_TEXTURE_2D, highscorebgTexture );
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
			(game->button[i].r.left + game->button[i].r.right) / 2;
		game->button[i].r.centery =
			(game->button[i].r.bot + game->button[i].r.top) / 2;
		game->nbuttons++;
		n+=50;
	}

	n = 0;
	//sub-menu buttons
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
			(game->button[i].r.left + game->button[i].r.right) / 2;
		game->button[i].r.centery =
			(game->button[i].r.bot + game->button[i].r.top) / 2;
		game->nbuttons++;
		n+=50;
	}

	n = 0;
	//gameover menu buttons
	for (int i=10; i<13; i++) {
		game->button[i].r.width = 120;
		game->button[i].r.height = 40;
		game->button[i].r.left = 250;
		game->button[i].r.bot = 450-n;
		game->button[i].r.right =
			game->button[i].r.left +
			game->button[i].r.width;
		game->button[i].r.top =
			game->button[i].r.bot +
			game->button[i].r.height;
		game->button[i].r.centerx =
			(game->button[i].r.left + game->button[i].r.right) / 2;
		game->button[i].r.centery =
			(game->button[i].r.bot + game->button[i].r.top) / 2;
		game->nbuttons++;
		n+=50;
	}

	n = 0;
	//sound and help button in game
	for (int i=13; i<15; i++) {
		game->button[i].r.width = 35;
		game->button[i].r.height = 35;
		game->button[i].r.left = 25+(n);
		game->button[i].r.bot = 720;
		game->button[i].r.right =
			game->button[i].r.left +
			game->button[i].r.width;
		game->button[i].r.top =
			game->button[i].r.bot +
			game->button[i].r.height;
		game->button[i].r.centerx =
			(game->button[i].r.left + game->button[i].r.right) / 2;
		game->button[i].r.centery =
			(game->button[i].r.bot + game->button[i].r.top) / 2;
		game->button[i].color[0] = 0.3f;
		game->button[i].color[1] = 0.7f;
		game->button[i].color[2] = 0.3f;
		game->button[i].text_color = 0x00ffffff;
		game->nbuttons++;
		n+=50;
	}
	strcpy(game->button[13].text, "S");
	strcpy(game->button[14].text, "Help");
}

void render_main_menu_buttons(Game *game)
{
	for (int i=0; i<5; i++) {
		if (game->button[i].over) {
			int w=2;
			glColor3f(1.0f, 1.0f, 0.0f);
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
		if (i == 1) {
			i++;
			if (game->button[i].over) {
				int w=2;
				glColor3f(1.0f, 1.0f, 0.0f);
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
		}
		glBindTexture(GL_TEXTURE_2D, game->buttonT->buttonTexture[i]);
		glPushMatrix();
		glTranslatef(game->button[i].r.centerx, game->button[i].r.centery, 0);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		glColor4ub(255,255,255,255);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2i(-50, -50);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2i(-50, 50);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2i( 50, 50);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2i( 50,-50);
		glEnd();
		glPopMatrix();
		glDisable(GL_ALPHA_TEST);
		glDisable(GL_TEXTURE_2D);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_2D);
	}

	if (game->difficulty == 3) {
		glBindTexture(GL_TEXTURE_2D, game->buttonT->buttonTexture[15]);
		glPushMatrix();
		glTranslatef(game->button[1].r.centerx, game->button[1].r.centery, 0);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		glColor4ub(255,255,255,255);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2i(-50, -50);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2i(-50, 50);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2i( 50, 50);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2i( 50,-50);
		glEnd();
		glPopMatrix();
		glDisable(GL_ALPHA_TEST);
		glDisable(GL_TEXTURE_2D);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_2D);
	} else if (game->difficulty == 2) {
		glBindTexture(GL_TEXTURE_2D, game->buttonT->buttonTexture[1]);
		glPushMatrix();
		glTranslatef(game->button[1].r.centerx, game->button[1].r.centery, 0);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		glColor4ub(255,255,255,255);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2i(-50, -50);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2i(-50, 50);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2i( 50, 50);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2i( 50,-50);
		glEnd();
		glPopMatrix();
		glDisable(GL_ALPHA_TEST);
		glDisable(GL_TEXTURE_2D);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_2D);
	} else if (game->difficulty == 1) {
		glBindTexture(GL_TEXTURE_2D, game->buttonT->buttonTexture[14]);
		glPushMatrix();
		glTranslatef(game->button[1].r.centerx, game->button[1].r.centery, 0);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		glColor4ub(255,255,255,255);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2i(-50, -50);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2i(-50, 50);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2i( 50, 50);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2i( 50,-50);
		glEnd();
		glPopMatrix();
		glDisable(GL_ALPHA_TEST);
		glDisable(GL_TEXTURE_2D);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_2D);
	}
}

void render_sub_menu_buttons(Game *game)
{
	for (int i=5; i<10; i++) {
		if (game->button[i].over) {
			int w=2;
			glColor3f(1.0f, 1.0f, 0.0f);
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
		if (i == 6) {
			i++;
			if (game->button[i].over) {
				int w=2;
				glColor3f(1.0f, 1.0f, 0.0f);
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
		}
		glBindTexture(GL_TEXTURE_2D, game->buttonT->buttonTexture[i]);
		glPushMatrix();
		glTranslatef(game->button[i].r.centerx, game->button[i].r.centery, 0);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		glColor4ub(255,255,255,255);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2i(-40, -40);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2i(-40, 40);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2i( 40, 40);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2i( 40,-40);
		glEnd();
		glPopMatrix();
		glDisable(GL_ALPHA_TEST);
		glDisable(GL_TEXTURE_2D);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_2D);
	}

	if (game->muted) {
		glBindTexture(GL_TEXTURE_2D, game->buttonT->buttonTexture[13]);
		glPushMatrix();
		glTranslatef(game->button[6].r.centerx, game->button[6].r.centery, 0);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		glColor4ub(255,255,255,255);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2i(-40, -40);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2i(-40, 40);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2i( 40, 40);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2i( 40,-40);
		glEnd();
		glPopMatrix();
		glDisable(GL_ALPHA_TEST);
		glDisable(GL_TEXTURE_2D);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_2D);
	} else if (!game->muted) {
		glBindTexture(GL_TEXTURE_2D, game->buttonT->buttonTexture[6]);
		glPushMatrix();
		glTranslatef(game->button[6].r.centerx, game->button[6].r.centery, 0);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		glColor4ub(255,255,255,255);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2i(-40, -40);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2i(-40, 40);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2i( 40, 40);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2i( 40,-40);
		glEnd();
		glPopMatrix();
		glDisable(GL_ALPHA_TEST);
		glDisable(GL_TEXTURE_2D);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_2D);
	}
}

void render_gameover_menu_buttons(Game *game)
{
	for (int i=10; i<13; i++) {
		if (game->button[i].over) {
			int w=2;
			glColor3f(1.0f, 1.0f, 0.0f);
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
		glBindTexture(GL_TEXTURE_2D, game->buttonT->buttonTexture[i]);
		glPushMatrix();
		glTranslatef(game->button[i].r.centerx, game->button[i].r.centery, 0);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		glColor4ub(255,255,255,255);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2i(-40, -40);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2i(-40, 40);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2i( 40, 40);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2i( 40,-40);
		glEnd();
		glPopMatrix();
		glDisable(GL_ALPHA_TEST);
		glDisable(GL_TEXTURE_2D);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_2D);
	}
}

void render_ingame_buttons(Game *game)
{
	Rect r;
	//draw in-game sound and help buttons
	glBindTexture(GL_TEXTURE_2D, 0);
	for (int i=13; i<15; i++) {
		if (game->button[i].over) {
			int w=2;
			glColor3f(1.0f, 1.0f, 0.0f);
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
		glColor3fv(game->button[i].color);
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

void check_menu_mouse ( XEvent *e, Game *game )
{
	static int savex = 0;
	static int savey = 0;
	int i,x,y;
	int lbutton=0;
	//int rbutton=0;
	//
	if (e->type == ButtonRelease)
		return;
	if (e->type == ButtonPress) {
		if (e->xbutton.button==1) {
			//Left button is down
			if ( !game->c.isJumping ) {
				game->c.isJumping = true;
				game->c.isStanding = false;
				game->c.velocity[1] = 15.0;
			}
			lbutton=1;
		}
		if (e->xbutton.button==3) {
			//Right button is down
			//rbutton=1;
			//if (rbutton){}
		}
	}
	x = e->xbutton.x;
	y = e->xbutton.y;
	y = game->windowHeight - y;
	if (savex != e->xbutton.x || savey != e->xbutton.y) {
		//Mouse moved
		savex = e->xbutton.x;
		savey = e->xbutton.y;
	}
	for (i=0; i<game->nbuttons; i++) {
		game->button[i].over=0;
		if (x >= game->button[i].r.left &&
				x <= game->button[i].r.right &&
				y >= game->button[i].r.bot &&
				y <= game->button[i].r.top) {
			game->button[i].over=1;
			if (game->button[i].over) {
				if (lbutton) {
					switch (i) {
						case 0:
							//Play
							reset_game(game);
							if ( game->c.isJumping ) {
							  game->c.isJumping = false;
							  game->c.isStanding = true;
							  game->c.velocity[1] = 0.0;
							}
							game->c.isStanding = true;
							playSounds ( "./wav/tick.wav",1.0f,
									false, game->muted );
							game->main_menu^=true;
							muteIntroSound(game);
							game->credits=false;
							game->highscoreboard=false;
							break;
						case 1:
							//Difficulty
							playSounds ( "./wav/tick.wav",1.0f,
									false, game->muted );
							game->difficulty++;
							if (game->difficulty > 3)
								game->difficulty = EASY;
							game->count++;
							if (game->count == 1) {
								strcpy(game->button[1].text, "EASY");
								std::cout << "easy mode ON" << std::endl;
							}
							if (game->count == 2) {
								strcpy(game->button[1].text, "MEDIUM");
								std::cout << "medium mode ON" << std::endl;
							}
							if (game->count == 3) {
								strcpy(game->button[1].text, "HARD");
								std::cout << "hard mode ON" << std::endl;
								game->count = 0;
							}
							break;
						case 2:
							playSounds ( "./wav/tick.wav",1.0f,
									false, game->muted );
							//High Scores
							game->credits=false;
							game->highscoreboard^=true;
							break;
						case 3:
							playSounds ( "./wav/tick.wav",1.0f,
									false, game->muted );
							//Credits
							game->highscoreboard=false;
							game->credits^=true;
							break;
						case 4:
							playSounds ( "./wav/tick.wav",1.0f,
									false, game->muted );
							//Exit Game
							game->done^=true;
							game->main_menu^=true;
							break;
					}
				}
			}
		}
	}
	return;
}

void check_paused_mouse ( XEvent *e, Game *game )
{
	static int savex = 0;
	static int savey = 0;
	int i,x,y;
	int lbutton=0;
	int rbutton=0;
	//
	if (e->type == ButtonRelease)
		return;
	if (e->type == ButtonPress) {
		if (e->xbutton.button==1) {
			//Left button is down
			lbutton=1;
		}
		if (e->xbutton.button==3) {
			//Right button is down
			rbutton=1;
			if (rbutton){}
		}
	}
	x = e->xbutton.x;
	y = e->xbutton.y;
	y = game->windowHeight - y;
	if (savex != e->xbutton.x || savey != e->xbutton.y) {
		//Mouse moved
		savex = e->xbutton.x;
		savey = e->xbutton.y;
	}
	for (i=0; i<game->nbuttons; i++) {
		game->button[i].over=0;
		if (x >= game->button[i].r.left &&
				x <= game->button[i].r.right &&
				y >= game->button[i].r.bot &&
				y <= game->button[i].r.top) {
			game->button[i].over=1;
			if (game->button[i].over) {
				if (lbutton) {
					switch (i) {
						case 5:
							//Resmue
							playSounds ( "./wav/tick.wav",1.0f,
									false, game->muted );
							game->sub_menu^=true;
							break;
						case 6:
							//Sound
							playSounds ( "./wav/tick.wav",1.0f,
									false, game->muted );
							muteSounds(game);
							break;
						case 7:
							//restart game
							playSounds ( "./wav/tick.wav",1.0f,
									false, game->muted );
							reset_game(game);
							game->sub_menu^=true;
							break;
						case 8:
							//main menu
							playSounds ( "./wav/tick.wav",1.0f,
									false, game->muted );
							if ( game->frog->rocket() )
							    game->frog->resetRocket();
							game->sub_menu^=true;
							game->main_menu^=true;
							reset_game(game);
							muteIntroSound(game);
							break;
						case 9:
							//Exit Game
							playSounds ( "./wav/tick.wav",1.0f,
									false, game->muted );
							game->done^=true;
							game->sub_menu^=true;
							break;
					}
				}
			}
		}
	}
	return;
}

void check_gameover_mouse ( XEvent *e, Game *game )
{
	static int savex = 0;
	static int savey = 0;
	int i,x,y;
	int lbutton=0;
	int rbutton=0;
	//
	if (e->type == ButtonRelease)

		return;
	if (e->type == ButtonPress) {
		if (e->xbutton.button==1) {
			//Left button is down
			lbutton=1;
		}
		if (e->xbutton.button==3) {
			//Right button is down
			rbutton=1;
			if (rbutton){}
		}
	}
	x = e->xbutton.x;
	y = e->xbutton.y;
	y = game->windowHeight - y;
	if (savex != e->xbutton.x || savey != e->xbutton.y) {
		//Mouse moved
		savex = e->xbutton.x;
		savey = e->xbutton.y;
	}
	for (i=0; i<game->nbuttons; i++) {
		game->button[i].over=0;
		if (x >= game->button[i].r.left &&
				x <= game->button[i].r.right &&
				y >= game->button[i].r.bot &&
				y <= game->button[i].r.top) {
			game->button[i].over=1;
			if (game->button[i].over) {
				if (lbutton) {
					switch (i) {
						case 10:
							//Play again
							playSounds ( "./wav/tick.wav",1.0f,
									false, game->muted );
							game->gameover^=true;
							reset_game(game);
							break;
						case 11:
							//Main menu
							playSounds ( "./wav/tick.wav",1.0f,
									false, game->muted );
							muteIntroSound(game);
							game->gameover^=true;
							game->main_menu^=true;
							break;
						case 12:
							//Exit
							playSounds ( "./wav/tick.wav",1.0f,
									false, game->muted );
							game->done^=true;
							game->gameover^=true;
							break;
					}
				}
			}
		}
	}
	return;
}
