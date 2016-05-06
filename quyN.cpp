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
// 1st goal (week 6): Game Menu and Prompts
// 2nd goal (week 6): Game Introduction
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

void init_menu_opengl(Game *game)
{
	//OpenGL initialization
	glViewport(0, 0, game->windowWidth, game->windowHeight);
	//Initialize matrices
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//Set 2D mode (no perspective)
	glOrtho(0, game->windowWidth, 0, game->windowHeight, -1, 1);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_FOG);
	glDisable(GL_CULL_FACE);
	//Set the screen background color
	glClearColor(0.1, 40.1, 200.1, 1.0);
	//Do this to allow fonts
	glEnable(GL_TEXTURE_2D);
	initialize_fonts();

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_FOG);
	glDisable(GL_CULL_FACE);
}

void init_menu(Game *game)
{
	game->boardDim = game->gridDim * 10;
	//initialize buttons...
	game->nbuttons=0;

	//size and position
	game->button[game->nbuttons].r.width = 140;
	game->button[game->nbuttons].r.height = 60;
	game->button[game->nbuttons].r.left = 20;
	game->button[game->nbuttons].r.bot = 320;
	game->button[game->nbuttons].r.right =
		game->button[game->nbuttons].r.left + game->button[game->nbuttons].r.width;
	game->button[game->nbuttons].r.top =
		game->button[game->nbuttons].r.bot + game->button[game->nbuttons].r.height;
	game->button[game->nbuttons].r.centerx =
		(game->button[game->nbuttons].r.left + game->button[game->nbuttons].r.right) / 2;
	game->button[game->nbuttons].r.centery =
		(game->button[game->nbuttons].r.bot + game->button[game->nbuttons].r.top) / 2;
	strcpy(game->button[game->nbuttons].text, "Restart");
	game->button[game->nbuttons].down = 0;
	game->button[game->nbuttons].click = 0;
	game->button[game->nbuttons].color[0] = 0.4f;
	game->button[game->nbuttons].color[1] = 0.4f;
	game->button[game->nbuttons].color[2] = 0.7f;
	game->button[game->nbuttons].dcolor[0] = game->button[game->nbuttons].color[0] * 0.5f;
	game->button[game->nbuttons].dcolor[1] = game->button[game->nbuttons].color[1] * 0.5f;
	game->button[game->nbuttons].dcolor[2] = game->button[game->nbuttons].color[2] * 0.5f;
	game->button[game->nbuttons].text_color = 0x00ffffff;
	game->nbuttons++;

	game->button[game->nbuttons].r.width = 140;
	game->button[game->nbuttons].r.height = 60;
	game->button[game->nbuttons].r.left = 20;
	game->button[game->nbuttons].r.bot = 160;
	game->button[game->nbuttons].r.right =
		game->button[game->nbuttons].r.left + game->button[game->nbuttons].r.width;
	game->button[game->nbuttons].r.top = game->button[game->nbuttons].r.bot +
		game->button[game->nbuttons].r.height;
	game->button[game->nbuttons].r.centerx = (game->button[game->nbuttons].r.left +
			game->button[game->nbuttons].r.right) / 2;
	game->button[game->nbuttons].r.centery = (game->button[game->nbuttons].r.bot +
			game->button[game->nbuttons].r.top) / 2;
	strcpy(game->button[game->nbuttons].text, "Quit");
	game->button[game->nbuttons].down = 0;
	game->button[game->nbuttons].click = 0;
	game->button[game->nbuttons].color[0] = 0.3f;
	game->button[game->nbuttons].color[1] = 0.3f;
	game->button[game->nbuttons].color[2] = 0.6f;
	game->button[game->nbuttons].dcolor[0] = game->button[game->nbuttons].color[0] * 0.5f;
	game->button[game->nbuttons].dcolor[1] = game->button[game->nbuttons].color[1] * 0.5f;
	game->button[game->nbuttons].dcolor[2] = game->button[game->nbuttons].color[2] * 0.5f;
	game->button[game->nbuttons].text_color = 0x00ffffff;
	game->nbuttons++;
}

void render_menu(Game *game)
{
	int x = 500;
	int y = 400;
	int w = 100;
	int h = 100;
	glClear(GL_COLOR_BUFFER_BIT);
	unsigned int rgba = 0xff0000ff; // red, no alpha
	glBegin(GL_QUADS);
	glColor4f(((rgba>>24)&0xff)/255.0f,
			((rgba>>16)&0xff)/255.0f, 
			((rgba>>8)&0xff)/255.0f,
			(rgba&0xff)/255.0f);
	glVertex3f(x,y,0);
	glVertex3f(x+w,y,0);
	glVertex3f(x+w,y+h,0);
	glVertex3f(x,y+h,0);
	glEnd();
	glColor4f(1, 1, 1, 1);
}
