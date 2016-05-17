#ifndef _KEVINJ_H_
#define _KEVINJ_H_

#include <AL/al.h>
#include <AL/alut.h>
#include "game.h"
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
using namespace std;

#define HOST "coding.debuntu.org"
#define PAGE "/"
#define PORT 80
#define USERAGENT "HTMLGET 1.0"
 
extern void playSounds(const char * sound, float gain, bool loop, bool muted);
extern void initSounds();
extern void cleanUpSound();
extern void initBuffer(const char * sound);
extern void initSource(ALuint buffer);
extern void stopMusic(void); 
extern void playMusic(void); 
extern void maxScore(Game *game);
extern void sendScoresToPHP(char playerName[], int score, int diff);

int create_tcp_socket();
char *get_ip(char *host);
char *build_get_query(char *host, char *page);
void usage();
 
#endif

