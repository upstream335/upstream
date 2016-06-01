//Author: Kevin Jenkin
//Project: Upstream
//May 2016
//Basic Function: Play and Cleanup sound files && how to score the game
//My file also connects to our website and send scores to the PhP file, so they
//can be displayed.
//For my button press I added a Monster mode with animation and bullets. If the
//frog collides with a bullet, you explode.
//
//Global Sound Variable to set Background music source and buffer.
//buffer/source[0] = background music
//buffer/source[1] = boing
//buffer/source[2] = tick
//buffer/source[3] = splash
//buffer/source[4] = rocket
//buffer/source[5] = money sound
//buffer/source[6] = explosion
//buffer/source[7] = haha
//buffer/source[8] = wow
//buffer/source[9] = introsound
//buffer/source[10] = gameoversound

#include "kevinJ.h"
#define HOST "coding.debuntu.org"
#define PAGE "/"
#define PORT 80
#define USERAGENT "HTMLGET 1.0"
#define rnd() (((double)rand())/(double)RAND_MAX)
#define PI 3.141592

struct Game *g;
ALuint source[10];
ALuint buffer[10];
ALuint state;
ALuint playBuffer[10];
ALuint playSource[10];
int bufferCount = 0;
int sourceCount = 0;
const double oobillion = 1.0 / 1e9;

void initBuffer(const char * sound)
{
	alGetError();
	buffer[bufferCount] = alutCreateBufferFromFile(sound);
	initSource(buffer[bufferCount]);
	bufferCount++;
}

void initSource(ALuint buffer)
{
	alGenSources(1, &source[sourceCount]);
	alSourcei(source[sourceCount], AL_BUFFER, buffer);
	alSourcef(source[sourceCount], AL_GAIN, 1.0f);
	alSourcef(source[sourceCount], AL_PITCH, 1.0f);
	alSourcei(source[sourceCount], AL_LOOPING, false);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: setting source\n");
		return;
	}
	sourceCount++;
}

void initSounds()
{
	alutInit(0, NULL);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: alutInit()\n");
		return;
	}
	alGetError();
	//Setup the listener.
	//Forward and up vectors are used.
	float vec[6] = {0.0f,0.0f,1.0f, 0.0f,1.0f,0.0f};
	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
	alListenerfv(AL_ORIENTATION, vec);
	alListenerf(AL_GAIN, 1.0f);
}

int getSource(const char * sound)
{
	if (strcmp(sound,"./wav/background.wav") == 0) {
		return 0;
	}
	else if (strcmp(sound,"./wav/boing2.wav") == 0) {
		return 1;
	}
	else if (strcmp(sound,"./wav/tick.wav") == 0) {
		return 2;
	}
	else if (strcmp(sound,"./wav/fishsplash.wav") == 0) {
		return 3;
	}
	else if (strcmp(sound,"./wav/rocket.wav") == 0) {
		return 4;
	}
	else if (strcmp(sound,"./wav/money.wav") == 0) {
		return 5;
	}
	else if (strcmp(sound,"./wav/explosion.wav") == 0) {
		return 6;
	}
	else if (strcmp(sound,"./wav/haha.wav") == 0) {
		return 7;
	}
	else if (strcmp(sound,"./wav/wow.wav") == 0) {
		return 8;
	}
	else if (strcmp(sound,"./wav/introSound.wav") == 0) {
		return 9;
	}
	else if (strcmp(sound,"./wav/gameoverSound.wav") == 0) {
		return 10;
	}
	else {
		printf("%s hasn't been initialized\n\n", sound);
		return -1;
	}
}

void playSounds(const char * sound, float gain, bool loop, bool muted)
{
	//Set volume and pitch to normal, no looping of sound.
	//gain is volume of sound
	int index = getSource(sound);
	//cout << "Sound: " << sound << " index: " << index << "\n";
	if (muted) {
		alSourceStop(source[index]);
		//printf("MUTED\n");
		return;
	}
	alSourcef(source[index], AL_GAIN, gain);
	alSourcef(source[index], AL_PITCH, 1.0f);
	alSourcei(source[index], AL_LOOPING, loop);
	alSourcePlay(source[index]);
}

void muteSounds(Game *game)
{
	game->muted ^= 1;
	if ( game->muted ) {
		stopMusic();
		printf ( "Sounds OFF\n" );
	} else {
		playMusic();
		printf ( "Sounds ON\n" );
	}
}

void muteIntroSound(Game *game)
{
	game->intromuted ^= 1;
	if ( game->intromuted ) {
		stopIntroSound();
		printf ( "Sounds OFF\n" );
	} else {
		playIntroSound();
		printf ( "Sounds ON\n" );
	}
}

void cleanUpSound()
{
	for (int i = 0; i < 10; i++) {
		alSourceStop(source[i]);
		alDeleteSources(1, &source[i]);
		alDeleteBuffers(1, &buffer[i]);
		ALCcontext *Context = alcGetCurrentContext();
		ALCdevice *Device = alcGetContextsDevice(Context);
		alcMakeContextCurrent(NULL);
		alcDestroyContext(Context);
		alcCloseDevice(Device);
	}
	alutExit();
}

void stopMusic()
{
	alSourceStop(source[0]);
}

void playMusic()
{
	alSourcePlay(source[0]);
}

void stopIntroSound()
{
	alSourceStop(source[9]);
}

void playIntroSound()
{
	alSourcePlay(source[9]);
}

double timeDiff(struct timespec *start, struct timespec *end)
{
	return (double)(end->tv_sec - start->tv_sec) + 
		(double)(end->tv_nsec - start->tv_nsec) * oobillion;
}

void timeCopy(struct timespec *dest, struct timespec *source)
{
	memcpy(dest, source, sizeof(struct timespec));
}

void maxScore(Game *game)
{
	int max = game->highscore[0];
	for (int i = 0; i < game->scoreCount; i++) {
		if (game->highscore[i] > max) {
			max = game->highscore[i];
		}
	}
	game->highscore[0] = max;
}

void sendScoresToPHP(char playerName[], int gameScore, int gameDiff)
{
	struct sockaddr_in *remote;
	int sock;
	int tmpres;
	char *ip;
	char *get;
	char buf[BUFSIZ+1];
	char *host, *tpage;
	char page[1000], score[100], diff[100];

	host = (char*)"sleipnir.cs.csub.edu";
	tpage = (char*)
		"/~jhargreaves/upstream/scores.php?param=upstream54321,";
	strcpy(page,tpage);
	cout << playerName << endl;
	strcat(page, playerName);
	strcat(page, ",");
	snprintf(score, 100, "%d", gameScore);
	strcat(page, score);
	strcat(page, ",");
	snprintf(diff, 100, "%d", gameDiff);
	strcat(page, diff);
	cout << host << "\n";
	cout << page << "\n";

	sock = create_tcp_socket();
	ip = get_ip(host);
	if (ip == 0)
		return;
	//fprintf(stderr, "IP is %s\n", ip);
	remote = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in *));
	remote->sin_family = AF_INET;
	tmpres = inet_pton(AF_INET, ip, (void *)(&(remote->sin_addr.s_addr)));
	if (tmpres < 0) {
		perror("Can't set remote->sin_addr.s_addr");
		return;
	} else if (tmpres == 0) {
		fprintf(stderr, "%s is not a valid IP address\n", ip);
		return;
	}
	remote->sin_port = htons(PORT);

	if (connect(sock, (struct sockaddr *)remote,
				sizeof(struct sockaddr)) < 0) {
		perror("Could not connect");
		return;
	}
	get = build_get_query(host, page);
	//fprintf(stderr, "Query is:\n<<START>>\n%s<<END>>\n", get);

	//Send the query to the server
	unsigned int sent = 0;
	while (sent < strlen(get)) {
		tmpres = send(sock, get+sent, strlen(get)-sent, 0);
		if (tmpres == -1) {
			perror("Can't send query");
			return;
		}
		sent += tmpres;
	}
	//now it is time to receive the page
	memset(buf, 0, sizeof(buf));
	int htmlstart = 0;
	char * htmlcontent;
	while ((tmpres = recv(sock, buf, BUFSIZ, 0)) > 0) {
		if (htmlstart == 0) {
			//Under certain conditions this will not work.
			//If the \r\n\r\n part is splitted into two messages
			//it will fail to detect the beginning of HTML content
			htmlcontent = strstr(buf, "\r\n\r\n");
			if (htmlcontent != NULL) {
				htmlstart = 1;
				htmlcontent += 4;
			}
		} else {
			htmlcontent = buf;
		}
		if (htmlstart) {
			//  fprintf(stdout,"%s", htmlcontent);
		}

		memset(buf, 0, tmpres);
	}
	if (tmpres < 0) {
		perror("Error receiving data");
	}
	free(get);
	free(remote);
	free(ip);
	close(sock);
	return;
}

void usage()
{
	fprintf(stderr, "USAGE: htmlget host [page]\n\
			\thost: the website hostname. ex: coding.debuntu.org\n\
			\tpage: the page to retrieve. ex: index.html, default: /\n");
}

int create_tcp_socket()
{
	int sock;
	if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		perror("Can't create TCP socket");
		return 0;
	}
	return sock;
}

char *get_ip(char *host)
{
	struct hostent *hent;
	int iplen = 15;
	char *ip = (char *)malloc(iplen+1);
	memset(ip, 0, iplen+1);
	if ((hent = gethostbyname(host)) == NULL) {
		herror("Can't get IP");
		return ip;
	}
	if (inet_ntop(AF_INET, (void *)hent->h_addr_list[0], ip, iplen+1) == NULL) {
		perror("Can't resolve host");
	}
	return ip;
}

char *build_get_query(char *host, char *page)
{
	char *query;
	char *getpage = page;
	char *tpl = (char *)
		"GET /%s HTTP/1.0\r\nHost: %s\r\nUser-Agent: %s\r\n\r\n";
	if (getpage[0] == '/') {
		getpage = getpage + 1;
		//fprintf(stderr,"Removing leading \"/\", converting %s to %s\n",
		//page, getpage);
	}
	// -5 is to consider the %s %s %s in tpl and the ending \0
	query = (char *)malloc(strlen(host)+strlen(getpage)
			+strlen(USERAGENT)+strlen(tpl)-5);
	sprintf(query, tpl, getpage, host, USERAGENT);
	return query;
}

void helpBG::render(void)
{
	float wid = 250.0f;
	glColor3f ( 1.0, 1.0, 1.0 );
	glPushMatrix();

	glTranslatef ( current.x_pos-300, current.y_pos-300, 0 );
	glBindTexture ( GL_TEXTURE_2D, helpbgTexture );
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

void render_help_menu(Game *game)
{
	game->helpbg->render();
	glBindTexture ( GL_TEXTURE_2D, 0 );
	Rect r;
	r.bot = game->windowHeight - 170;
	r.left = 300;
	ggprint17 ( &r, 40, 0, "Help Page" );
	r.left = 290;
	ggprint13 ( &r, 40, 0, 
			"The Object Of The Game Is To Get The Highest Score" );
	r.left = 260;
	ggprint13 ( &r, 40, 0, "Jumping On A Lilypad Earns Player 10 Points" );
	r.left = 305;
	ggprint13 ( &r, 40, 0, 
			"Log Is 2 Points Per Tick. Eating A Fly Is Up To 450 Points" );
	r.left = 300;
	ggprint13 ( &r, 40, 0, 
			"The Alligator Will Kill You Unless You Jump On His Back" );
	r.left = 295;
	ggprint13 ( &r, 50, 0, 
			"Jumping On The Golden Turtle Will Earn You 500 Points" );
	r.left = 250;
	ggprint13 ( &r, 30, 0, "Left Click -- Enable / Disabled Rocket Mode" );
	r.left = 290;
	ggprint13 ( &r, 30, 0, 
			"J -- Troll Lilypads (Lilypads move in random direction)" );
	r.left = 155;
	ggprint13 ( &r, 30, 0, "K -- Stress Test" );
	r.left = 235;
	ggprint13 ( &r, 30, 0, "S -- Swarm (Spawns Mass Baby Flys)" );
	r.left = 197;
	ggprint13 ( &r, 30, 0, "B -- Boss Mode (Very Hard)" );
	//ggprint40(&r, 50, 0, "FROG Y: %f", game->windowHeight - game->c.newPosY);
	//ggprint40(&r, 50, 0, "FROG x: %f", game->windowWidth - game->c.newPosX);
}

void check_help_mouse(XEvent *e, Game *game)
{
	if (e->type == ButtonRelease) {
		return;
	}
	if (e->type == ButtonPress) {
		if (e->xbutton.x >= 98 && e->xbutton.x <= 130 &&
				e->xbutton.y >= 130 && e->xbutton.y <= 155) {
			game->help_menu ^= 1;
		}
	}
}

void Monster::render ( void )
{
	float wid = 40.0f; // size of monster sprite
	if (current.x_pos == 0)
		current.x_vel = 2;

	if (current.x_pos == 600)
		current.x_vel = -2;

	current.x_pos += current.x_vel;

	glColor3f ( 1.0, 1.0, 1.0 );
	glPushMatrix();
	glTranslatef ( current.x_pos, current.y_pos, 0 );
	glBindTexture ( GL_TEXTURE_2D, monsterTexture[0] );
	if (  current.frame <20 ) {
		glBindTexture ( GL_TEXTURE_2D, monsterTexture[0] );
		current.frame++;
	}
	if ( current.frame>=20 && current.frame <=30 ) {
		glBindTexture ( GL_TEXTURE_2D, monsterTexture[1] );
		current.frame++;
	}
	if ( current.frame>=30 && current.frame <=40 ) {
		glBindTexture ( GL_TEXTURE_2D, monsterTexture[2] );
		current.frame++;
	}
	if ( current.frame>=40 && current.frame <=50 ) {
		glBindTexture ( GL_TEXTURE_2D, monsterTexture[3] );
		current.frame++;
	}
	if ( current.frame>=50 && current.frame <=60 ) {
		glBindTexture ( GL_TEXTURE_2D, monsterTexture[4] );
		current.frame++;
	}
	if ( current.frame>=60 && current.frame <=70 ) {
		glBindTexture ( GL_TEXTURE_2D, monsterTexture[5] );
		current.frame++;
	}
	if ( current.frame>=70 && current.frame <=80 ) {
		glBindTexture ( GL_TEXTURE_2D, monsterTexture[6] );
		current.frame++;
	}
	if ( current.frame>=80 && current.frame <=90 ) {
		glBindTexture ( GL_TEXTURE_2D, monsterTexture[2] );
		current.frame++;
	}
	if ( current.frame>90 )
		current.frame=0;
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

void drawBullet(Game *g)
{
	spawnBullet(g);
	for (int i=0; i<g->nbullets; i++) {
		Bullet *b = &g->barr[i];
		//black square to represent bullet
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_QUADS);
		glVertex2f(b->pos[0] - 5, b->pos[1] - 5);
		glVertex2f(b->pos[0] + 5, b->pos[1] - 5);
		glVertex2f(b->pos[0] + 5, b->pos[1] + 5);
		glVertex2f(b->pos[0] - 5, b->pos[1] + 5);
		glEnd();
	}
}

void deleteBullet(Game *g, Bullet *b)
{
	b = &g->barr[1 - 1];
	g->nbullets--;
}

void updateBullet(Game *game)
{
	struct timespec bt;
	clock_gettime(CLOCK_REALTIME, &bt);
	for (int i=0; i<game->nbullets; i++) {
		Bullet *b = &game->barr[i];
		//move the bullet
		b->pos[0] += b->vel[0];
		b->pos[1] += b->vel[1];
		//Check for collision with window edges
		if (b->pos[0] < 0.0f) {
			b->vel[0] *= -1;
		}
		else if (b->pos[0] > (float)game->windowWidth) {
			b->vel[0] *= -1;
		}
		else if (b->pos[1] < 0.0f) {
			deleteBullet(game, b);
		}
		else if (b->pos[1] > (float)game->windowHeight) {
			deleteBullet(game, b);
		}
	}
}

void spawnBullet(Game *g)
{
	//spawn another bullet after its off the screen
	//only one bullet on the screen at a time
	if (g->nbullets < 1) {
		//shoot a bullet from monster position
		Bullet *b = &g->barr[g->nbullets];
		b->pos[0] = g->monster->getXpos();
		b->pos[1] = g->monster->getYpos();
		b->vel[0] = g->monster->getXvel();
		b->vel[1] = g->monster->getYvel();
		b->vel[1] += -3;
		b->pos[1] += b->vel[1];
		b->color[0] = 0.0f;
		b->color[1] = 0.0f;
		b->color[2] = 0.0f;
		g->nbullets++;
	}
}

