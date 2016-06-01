#ifndef _KEVINJ_H_
#define _KEVINJ_H_

#include <AL/al.h>
#include <AL/alut.h>
#include "game.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netdb.h>
using namespace std;

extern void muteIntroSound(Game *game);
extern void stopIntroSound(void);
extern void playIntroSound(void);

extern void playSounds(const char * sound, float gain, bool loop, bool muted);
extern void muteSounds(Game *game);
extern void initSounds();
extern void cleanUpSound();
extern void initBuffer(const char * sound);
extern void initSource(ALuint buffer);
extern void stopMusic(void); 
extern void playMusic(void); 
extern void maxScore(Game *game);
extern void sendScoresToPHP(char playerName[], int score, int diff);
extern void render_help_menu(Game *game);
extern void check_help_mouse(XEvent *e, Game *game);
extern double timeDiff(struct timespec *start, struct timespec *end);
extern void updateBullet(Game *game);
extern void spawnBullet(Game *g);
extern void deleteBullet(Game *g, Bullet *b);
extern void drawBullet(Game *g);

int create_tcp_socket();
char *get_ip(char *host);
char *build_get_query(char *host, char *page);
void usage();

#endif

