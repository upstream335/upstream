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
using namespace std;

#include "io.h"
#include "quyN.h"
#include "kevinJ.h"

#include "collision.h"

//macros
#define random(a) (rand()%a)
#define GRAVITY 0.1
#define random(a) (rand()%a)

//X Windows variables
Display *dpy;
Window win;
GLXContext glc;
GC gc;
typedef double Flt;
typedef double Vec[3];
typedef Flt Matrix[4][4];
struct timespec timeStart, timeCurrent;
struct timespec timePause;
const double physicsRate = 1.0 / 30.0;
double physicsCountdown=0.0;
const double oobillion = 1.0 / 1e9;
double timeSpan=0.0;
unsigned int upause=0;

//Function prototypes
extern void initXWindows(Game *game);
extern void cleanupXWindows(void);
extern void checkResize(XEvent *e, Game *game);
extern void physics(Game *game);
extern void render(Game *game);
extern void init_opengl(Game *game);
extern void timeCopy(struct timespec *dest, struct timespec *source);
extern double timeDiff(struct timespec *start, struct timespec *end);
extern void setupScreenRes(const int w, const int h, Game *game);
extern void reshapeWindow(int width, int height, Game *game);
