/*     __  __           __
 *    / / / /___  _____/ /_________  ____ _____ ___
 *   / / / / __ \/ ___/ __/ ___/ _ \/ __ `/ __ `__ \
 *  / /_/ / /_/ (__  ) /_/ /  /  __/ /_/ / / / / / /
 *  \____/ .___/____/\__/_/   \___/\__,_/_/ /_/ /_/
 *      /_/
 */
///////////////////////////////////////////////////////////////////////
// CONTRIBUTORS: JONATHAN DINH
//               QUY NGUYEN
//               JOHN HARGREAVES
//               KEVIN JENKIN
///////////////////////////////////////////////////////////////////////
#include "upstream.h"

int main ( void )
{
	srand ( time ( NULL ) );
	srand ( time ( 0 ) );
	//declare game object
	Game game;
	initXWindows ( &game );
	init_opengl ( &game );
	init_game ( &game );
	init_buttons ( &game );
	//Initializing sound sources && buffers
	initSounds();
	initBuffer("./wav/background.wav");
	initBuffer("./wav/boing2.wav");
	initBuffer("./wav/tick.wav");
	initBuffer("./wav/fishsplash.wav");
	initBuffer("./wav/rocket.wav");
	playSounds ( "./wav/background.wav", 0.1f, true, game.muted );

	while ( !game.done ) {
		while ( XPending ( dpy ) ) {
			XEvent e;
			XNextEvent ( dpy, &e );
			checkResize ( &e, &game );
			check_mouse ( &e, &game );
			check_keys ( &e, &game );
		}
		//Game Introduction - Main Menu
		while ( !game.main_menu ) {
			while ( XPending ( dpy ) ) {
				XEvent e;
				XNextEvent ( dpy, &e );
				checkResize ( &e, &game );
				check_menu_mouse ( &e, &game );
			}
			demo ( &game );
			physics ( &game );
			render_main_menu ( &game );
			glXSwapBuffers ( dpy, win );
		}
		//In game - Paused Menu
		while ( !game.sub_menu ) {
			while ( XPending ( dpy ) ) {
				XEvent e;
				XNextEvent ( dpy, &e );
				checkResize ( &e, &game );
				check_paused_mouse ( &e, &game );
				check_keys ( &e, &game );
			}
			render_sub_menu ( &game );
			glXSwapBuffers ( dpy, win );
		}
		while (game.isHighScore) {
			while ( XPending ( dpy ) ) {
				XEvent e;
				XNextEvent ( dpy, &e );
				checkResize ( &e, &game );
				check_gameover_mouse ( &e, &game );
				if (game.isHighScore) {
					getName(&e, &game);
				} else {
					check_keys ( &e, &game );
				}
			}
			if (game.isHighScore)
				drawHighScoreBox(&game);
			glXSwapBuffers ( dpy, win );
			glClearColor(0.0f,0.0f,0.0f,1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		while (game.gameover) {
			while ( XPending ( dpy ) ) {
				XEvent e;
				XNextEvent ( dpy, &e );
				checkResize ( &e, &game );
				check_gameover_mouse ( &e, &game );
			}
			render_gameover_menu ( &game );
			glXSwapBuffers ( dpy, win );
		}
		if ( game.demo.on ) {
			demo ( &game );
		}
		physics ( &game );
		render ( &game );
		glXSwapBuffers ( dpy, win );
	}
	cleanUpSound();
	cleanupXWindows();
	return 0;
}

double timeDiff ( struct timespec *start, struct timespec *end )
{
	return ( double ) ( end->tv_sec - start->tv_sec ) +
		( double ) ( end->tv_nsec - start->tv_nsec ) * oobillion;
}

void timeCopy ( struct timespec *dest, struct timespec *source )
{
	memcpy ( dest, source, sizeof ( struct timespec ) );
}

void set_title ( void )
{
	//Set the window title bar.
	XMapWindow ( dpy, win );
	XStoreName ( dpy, win, "Testing framework for Upstream" );
}

void cleanupXWindows ( void )
{
	//do not change
	XDestroyWindow ( dpy, win );
	XCloseDisplay ( dpy );
}

void initXWindows ( Game *game )
{
	//do not change
	GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
	int w=game->windowWidth, h=game->windowHeight;
	dpy = XOpenDisplay ( NULL );
	if ( dpy == NULL ) {
		//std::cout << "\n\tcannot connect to X server\n" << std::endl;
		exit ( EXIT_FAILURE );
	}
	Window root = DefaultRootWindow ( dpy );
	XVisualInfo *vi = glXChooseVisual ( dpy, 0, att );
	if ( vi == NULL ) {
		std::cout << "\n\tno appropriate visual found\n" << std::endl;
		exit ( EXIT_FAILURE );
	}
	Colormap cmap = XCreateColormap ( dpy, root, vi->visual, AllocNone );
	XSetWindowAttributes swa;
	swa.colormap = cmap;
	swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
		ButtonPress | ButtonReleaseMask |
		PointerMotionMask |
		StructureNotifyMask | SubstructureNotifyMask;
	win = XCreateWindow ( dpy, root, 0, 0, w, h, 0, vi->depth,
			InputOutput, vi->visual, CWColormap | CWEventMask, &swa );
	set_title();
	glc = glXCreateContext ( dpy, vi, NULL, GL_TRUE );
	glXMakeCurrent ( dpy, win, glc );
}

void setupScreenRes ( const int w, const int h, Game *game )
{
	game->windowWidth = w;
	game->windowHeight = h;
}

void reshapeWindow ( int width, int height, Game *game )
{
	//window has been resized.
	//setupScreenRes(width, height, game);
	glViewport ( 0, 0, ( GLint ) width, ( GLint ) height );
	glMatrixMode ( GL_PROJECTION );
	glLoadIdentity();
	glMatrixMode ( GL_MODELVIEW );
	glLoadIdentity();
	glOrtho ( 0, game->windowWidth, 0, game->windowHeight, -1, 1 );
	set_title();
}

void checkResize ( XEvent *e, Game *game )
{
	//The ConfigureNotify is sent by the server if the window is resized.
	if ( e->type != ConfigureNotify )
		return;
	XConfigureEvent xce = e->xconfigure;
	if ( xce.width != game->windowWidth || xce.height != game->windowHeight ) {
		//Window size did change.
		reshapeWindow ( xce.width, xce.height, game );
	}
}

void init_opengl ( Game *game )
{
	//OpenGL initialization
	glViewport ( 0, 0, game->windowWidth, game->windowHeight );
	//Initialize matrices
	glMatrixMode ( GL_PROJECTION );
	glLoadIdentity();
	glMatrixMode ( GL_MODELVIEW );
	glLoadIdentity();
	//Set 2D mode (no perspective)
	glOrtho ( 0, game->windowWidth, 0, game->windowHeight, -1, 1 );
	glDisable ( GL_LIGHTING );
	glDisable ( GL_DEPTH_TEST );
	glDisable ( GL_FOG );
	glDisable ( GL_CULL_FACE );
	//Set the screen background color
	glClearColor ( 0.1, 40.1, 200.1, 1.0 );
	//Do this to allow fonts
	glEnable ( GL_TEXTURE_2D );
	initialize_fonts();
	glDisable ( GL_LIGHTING );
	glDisable ( GL_DEPTH_TEST );
	glDisable ( GL_FOG );
	glDisable ( GL_CULL_FACE );
}

void physics ( Game *game )
{
	collision ( game );
	if ( game->demo.on ) {
		game->c.newPosX =game->frog->getXpos();
		game->c.newPosY =game->frog->getYpos();
	}
	std::cout<<"main menu=" << game->main_menu<<std::endl;
	if ( game->main_menu ) {
        game->frog->meterOn();
			}

	//move frog
	if ( game->playing==true && game->splash->getFrame() >=400 ) {
		if ( !game->frog->rocket() && !game->demo.on ) {
			game->frog->move ( ( float ) game->c.center[0],
					( float ) game->c.center[1],
					( float ) game->c.velocity[0],
					( float ) game->c.velocity[1] );
		} else {
			game->frog->move ( ( float ) game->c.center[0],
					game->frog->getYpos(), game->frog->getXvel(),
					game->frog->getYvel() );
			game->c.center[0] = game->frog->getXpos();
			game->c.center[1] = game->frog->getYpos();
			game->c.velocity[0] = game->frog->getXvel();
			game->c.velocity[1] = game->frog->getYvel();
			game->c.isJumping = true;
			game->c.isStanding = false;
		}
		//move fly
		int x=rand() %10+1;
		int y =rand() %10+1;
		x=6-x;
		y=6-y;
		game->fly->move ( game->fly->getXpos()+x/2,
				game->fly->getYpos()+y,x/10,y/10 );
		checkLilies ( game );
	}
}

void drawCircle ( float x, float y, float radius, int detail )
{
	float radian = 2.0 * 3.14;
	glPushMatrix();
	glColor3ub ( 90,140,90 );
	glBegin ( GL_TRIANGLE_FAN );
	for ( int i = 0; i <= detail; i++ ) {
		glVertex2f (
				x + ( radius * cos ( i * radian / detail ) ),
				y + ( radius * sin ( i * radian / detail ) )
				);
	}
	glEnd();
	glPopMatrix();
}

void render ( Game *game )
{
	glClear ( GL_COLOR_BUFFER_BIT );
	game->water[0]->render();
	game->water[1]->render();
	game->water[2]->render();
	game->turtle->render();
	drawLilies ( game );
	//drawHighScoreBox(game);
	screenUpdate ( game );
	for ( int i=0; i<4; i++ ) {
		game->log[i]->render();
	}
	//drawBubble(game);
	game->splash->render();
	game->gator->render();
	game->bridge->render();
	game->frog->render();
	game->fly->render();
	game->hud->render();
	game->rocketPack->render();
	game->meter->render();
	drawScore ( game->score, game, 20,60,game->windowHeight-100);
	// TEXT ====================================
	std::string mode;
	if ( game->difficulty==EASY )
		mode ="EASY";
	if ( game->difficulty==MED )
		mode ="Regular";
	if ( game->difficulty==HARD )
		mode ="HARD";
	glBindTexture ( GL_TEXTURE_2D, 0 );
	Rect r;
	r.bot = game->windowHeight - 120;
	r.left = 420;
	//r.center = 600;
	//ggfrog40b ( &r, 50, 0, "UPSTREAM!" );
	//ggprint40 ( &r,50,0,"Current Score: %d Mode: %s",game->score,mode.c_str());
	ggprint17 ( &r,100,0,"%d    %d",game->lives,game->frog->getNumberRockets() );
	//std::cout<<" Score: "<<game->score<<" Mode: "<<mode<<std::endl;
	// std::cout<<"  "<<game->frog->numberRockets<<" "<<mode<<std::endl;
	maxScore ( game );
	r.left = 300;
	if ( game->frog->getYpos() <=50 )
		ggprint40 ( &r, 50, 0, "High Score: %d", game->highscore[0] );
	//std::cout<<game->highscore[0]<<std::endl;
	//ggprint40(&r, 0, 0, "FROG Y: %f", game->windowHeight - game->c.newPosY);
	//ggprint40(&r, 0, 0, "FROG x: %f", game->c.newPosX);
	// ==========================================================
}
