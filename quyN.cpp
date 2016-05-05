// Author: Quy Nguyen
// Date written:  04-26-16
// Last modified: 05-02-16
// Program: Game inputs and outputs (keyboard functionality)
//          Game Menus and Prompts
//          Game Introduction
//          Game Attract Mode
// Purpose: I will be in charge of the game menu, introduction,
//          attract mode (hard, medium, easy), and keyboard
//          functionality. I will attempt to make a generic
//          function for each.
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
#include <AL/al.h>
#include <AL/alut.h>
#include "ppm.h"
#include "log.h"
#include "fonts.h"

#include "quyN.h"
#include "kevinJ.h"
#include "fonts.h"

void check_mouse(XEvent *e, Game *game)
{
	static int savex = 0;
	static int savey = 0;
	static int n = 0;
	if (e->type == ButtonRelease) {
		return;
	}
	if (e->type == ButtonPress) {
		if (e->xbutton.button==1) {
			//Left button was pressed
			if (e->xbutton.x >= 465 && e->xbutton.x <= 500 &&
					e->xbutton.y >= 10 && e->xbutton.y <= 30) {
				printf("Sounds Muted\n");
				game->muted ^= 1;
				if (game->muted)
					stopMusic();
				else
					playMusic();
			}
			if (!game->c.isJumping) {
				game->c.isJumping = true;
				game->c.isStanding = false;
				game->c.velocity[1] = 15.0;
			}
			return;
		}
		if (e->xbutton.button==3) {
			//Right button was pressed
			return;
		}
	}
	//Did the mouse move?
	if (savex != e->xbutton.x || savey != e->xbutton.y) {
		savex = e->xbutton.x;
		savey = game->windowHeight - e->xbutton.y;
		if (++n < 10)
			return;
		game->c.newPosX = savex;
		game->c.newPosY = savey;
	}
}

int check_keys(XEvent *e, Game *game)
{
	//Was there input from the keyboard?
	if (e->type == KeyPress) {
		int key = XLookupKeysym(&e->xkey, 0);
		switch (key) {
			case XK_f:
				break;
			case XK_b:
				break;
			case XK_s:
				return game->menu_done=1;
				break;
			case XK_Escape:
				break;
				return 1;
		}
	}
	return 0;
}

// void init_menu_opengl(Game *menu)
// {
// 	//OpenGL initialization
// 	glViewport(0, 0, menu->windowHeight, menu->xres);
// 	//Initialize matrices
// 	glMatrixMode(GL_PROJECTION);
// 	glLoadIdentity();
// 	glMatrixMode(GL_MODELVIEW);
// 	glLoadIdentity();
// 	//Set 2D mode (no perspective)
// 	glOrtho(0, menu->windowHeight, 0, menu->xres, -1, 1);
// 	glDisable(GL_LIGHTING);
// 	glDisable(GL_DEPTH_TEST);
// 	glDisable(GL_FOG);
// 	glDisable(GL_CULL_FACE);
// 	//Set the screen background color
// 	glClearColor(0.1, 40.1, 200.1, 1.0);
// 	//Do this to allow fonts
// 	glEnable(GL_TEXTURE_2D);
// 	initialize_fonts();
// 
// 	glDisable(GL_LIGHTING);
// 	glDisable(GL_DEPTH_TEST);
// 	glDisable(GL_FOG);
// 	glDisable(GL_CULL_FACE);
// }

void init_menu_opengl(Game *menu)
{
	//OpenGL initialization
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_COLOR_MATERIAL);
	//
	//choose one of these
	//glShadeModel(GL_FLAT);
	glShadeModel(GL_SMOOTH);
	glDisable(GL_LIGHTING);
	glBindTexture(GL_TEXTURE_2D, 0);
	//
	glEnable(GL_TEXTURE_2D);
	//marble_texture = loadBMP("marble.bmp");
	glBindTexture(GL_TEXTURE_2D, 0);
	//
	//load the image file into a ppm structure.
	//
	menu->bgImage = ppm6GetImage("./images/bg.ppm");
	//
	//create opengl texture elements
	glGenTextures(1, &menu->bgTexture);
	glBindTexture(GL_TEXTURE_2D, menu->bgTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
			menu->bgImage->width, menu->bgImage->height,
			0, GL_RGB, GL_UNSIGNED_BYTE, menu->bgImage->data);
}

void init_menu(Game *menu)
{
	menu->boardDim = menu->gridDim * 10;
	//initialize buttons...
	menu->nbuttons=0;
	//size and position
	menu->button[menu->nbuttons].r.width = 140;
	menu->button[menu->nbuttons].r.height = 60;
	menu->button[menu->nbuttons].r.left = 20;
	menu->button[menu->nbuttons].r.bot = 320;
	menu->button[menu->nbuttons].r.right =
		menu->button[menu->nbuttons].r.left + menu->button[menu->nbuttons].r.width;
	menu->button[menu->nbuttons].r.top =
		menu->button[menu->nbuttons].r.bot + menu->button[menu->nbuttons].r.height;
	menu->button[menu->nbuttons].r.centerx =
		(menu->button[menu->nbuttons].r.left + menu->button[menu->nbuttons].r.right) / 2;
	menu->button[menu->nbuttons].r.centery =
		(menu->button[menu->nbuttons].r.bot + menu->button[menu->nbuttons].r.top) / 2;
	strcpy(menu->button[menu->nbuttons].text, "Reset");
	menu->button[menu->nbuttons].down = 0;
	menu->button[menu->nbuttons].click = 0;
	menu->button[menu->nbuttons].color[0] = 0.4f;
	menu->button[menu->nbuttons].color[1] = 0.4f;
	menu->button[menu->nbuttons].color[2] = 0.7f;
	menu->button[menu->nbuttons].dcolor[0] = menu->button[menu->nbuttons].color[0] * 0.5f;
	menu->button[menu->nbuttons].dcolor[1] = menu->button[menu->nbuttons].color[1] * 0.5f;
	menu->button[menu->nbuttons].dcolor[2] = menu->button[menu->nbuttons].color[2] * 0.5f;
	menu->button[menu->nbuttons].text_color = 0x00ffffff;
	menu->nbuttons++;
	menu->button[menu->nbuttons].r.width = 140;
	menu->button[menu->nbuttons].r.height = 60;
	menu->button[menu->nbuttons].r.left = 20;
	menu->button[menu->nbuttons].r.bot = 160;
	menu->button[menu->nbuttons].r.right =
		menu->button[menu->nbuttons].r.left + menu->button[menu->nbuttons].r.width;
	menu->button[menu->nbuttons].r.top = menu->button[menu->nbuttons].r.bot +
		menu->button[menu->nbuttons].r.height;
	menu->button[menu->nbuttons].r.centerx = (menu->button[menu->nbuttons].r.left +
			menu->button[menu->nbuttons].r.right) / 2;
	menu->button[menu->nbuttons].r.centery = (menu->button[menu->nbuttons].r.bot +
			menu->button[menu->nbuttons].r.top) / 2;
	strcpy(menu->button[menu->nbuttons].text, "Quit");
	menu->button[menu->nbuttons].down = 0;
	menu->button[menu->nbuttons].click = 0;
	menu->button[menu->nbuttons].color[0] = 0.3f;
	menu->button[menu->nbuttons].color[1] = 0.3f;
	menu->button[menu->nbuttons].color[2] = 0.6f;
	menu->button[menu->nbuttons].dcolor[0] = menu->button[menu->nbuttons].color[0] * 0.5f;
	menu->button[menu->nbuttons].dcolor[1] = menu->button[menu->nbuttons].color[1] * 0.5f;
	menu->button[menu->nbuttons].dcolor[2] = menu->button[menu->nbuttons].color[2] * 0.5f;
	menu->button[menu->nbuttons].text_color = 0x00ffffff;
	menu->nbuttons++;
}

void render_menu(Game *menu)
{
	int i;
	Rect r;
	//This code is repeated several times in this program, so
	//it can be made more generic and cleaner with some work.
	//int b2 = menu->boardDim/2;
	//int s0 = menu->windowWidth>>1;
	//int s1 = menu->windowHeight>>1;
	//center of a grid
	//int cent[2];
	//bq is the width of one grid section
	//--------------------------------------------------------
	//start the opengl stuff
	//set the viewing area on screen
	glViewport(0, 0, menu->windowWidth, menu->windowHeight);
	//clear color buffer
	glClearColor(0.1f, 0.2f, 0.3f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	//init matrices
	glMatrixMode (GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	//this sets to 2D mode (no perspective)
	glOrtho(0, menu->windowWidth, 0, menu->windowHeight, -1, 1);
	//
	//screen background
	glColor3f(0.5f, 0.5f, 0.5f);
	glBindTexture(GL_TEXTURE_2D, menu->bgTexture);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex2i(0,0);
	glTexCoord2f(0.0f, 1.0f); glVertex2i(0, menu->windowHeight);
	glTexCoord2f(1.0f, 1.0f); glVertex2i(menu->windowWidth, menu->windowHeight);
	glTexCoord2f(1.0f, 0.0f); glVertex2i(menu->windowWidth, 0);
	glEnd();
	//draw all buttons
	for (i=0; i<menu->nbuttons; i++) {
		if (menu->button[i].over) {
			int w=2;
			glColor3f(1.0f, 1.0f, 0.0f);
			//draw a highlight around button
			glLineWidth(3);
			glBegin(GL_LINE_LOOP);
			glVertex2i(menu->button[i].r.left-w,  menu->button[i].r.bot-w);
			glVertex2i(menu->button[i].r.left-w,  menu->button[i].r.top+w);
			glVertex2i(menu->button[i].r.right+w, menu->button[i].r.top+w);
			glVertex2i(menu->button[i].r.right+w, menu->button[i].r.bot-w);
			glVertex2i(menu->button[i].r.left-w,  menu->button[i].r.bot-w);
			glEnd();
			glLineWidth(1);
		}
		if (menu->button[i].down) {
			glColor3fv(menu->button[i].dcolor);
		} else {
			glColor3fv(menu->button[i].color);
		}
		glBegin(GL_QUADS);
		glVertex2i(menu->button[i].r.left,  menu->button[i].r.bot);
		glVertex2i(menu->button[i].r.left,  menu->button[i].r.top);
		glVertex2i(menu->button[i].r.right, menu->button[i].r.top);
		glVertex2i(menu->button[i].r.right, menu->button[i].r.bot);
		glEnd();
		r.left = menu->button[i].r.centerx;
		r.bot  = menu->button[i].r.centery-8;
		r.center = 1;
		if (menu->button[i].down) {
			ggprint16(&r, 0, menu->button[i].text_color, "Pressed!");
		} else {
			ggprint16(&r, 0, menu->button[i].text_color, menu->button[i].text);
		}
	}
}
