//Author: Kevin Jenkin
//Project: Upstream
//May 2016
//Basic Function: Play and Cleanup sound files && how to score the game

#include "kevinJ.h"

//Global Sound Variable to set Background music source and buffer.
//buffer/source[0] = background music
//buffer/source[1] = boing
//buffer/source[2] = tick
//buffer/source[3] = splash
//buffer/source[4] = rocket

struct Game *g;
ALuint source[10];
ALuint buffer[10];
ALuint state;
ALuint playBuffer[10];
ALuint playSource[10];
int bufferCount = 0;
int sourceCount = 0;

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
	cout << "Sound: " << sound << " index: " << index << "\n";
	if (muted) {
		alSourceStop(source[index]);
		printf("MUTED\n");
		return;
	}
	alSourcef(source[index], AL_GAIN, gain);
	alSourcef(source[index], AL_PITCH, 1.0f);
	alSourcei(source[index], AL_LOOPING, loop);
	alSourcePlay(source[index]);
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
	//fprintf(stderr, "IP is %s\n", ip);
	remote = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in *));
	remote->sin_family = AF_INET;
	tmpres = inet_pton(AF_INET, ip, (void *)(&(remote->sin_addr.s_addr)));
	if (tmpres < 0) {
		perror("Can't set remote->sin_addr.s_addr");
		exit(1);
	} else if (tmpres == 0) {
		fprintf(stderr, "%s is not a valid IP address\n", ip);
		exit(1);
	}
	remote->sin_port = htons(PORT);

	if (connect(sock, (struct sockaddr *)remote,
				sizeof(struct sockaddr)) < 0) {
		perror("Could not connect");
		exit(1);
	}
	get = build_get_query(host, page);
	//fprintf(stderr, "Query is:\n<<START>>\n%s<<END>>\n", get);

	//Send the query to the server
	unsigned int sent = 0;
	while (sent < strlen(get)) {
		tmpres = send(sock, get+sent, strlen(get)-sent, 0);
		if (tmpres == -1) {
			perror("Can't send query");
			exit(1);
		}
		sent += tmpres;
	}
	//now it is time to receive the page
	memset(buf, 0, sizeof(buf));
	int htmlstart = 0;
	char * htmlcontent;
	while ((tmpres = recv(sock, buf, BUFSIZ, 0)) > 0) {
		if (htmlstart == 0) {
			/* Under certain conditions this will not work.
			 * If the \r\n\r\n part is splitted into two messages
			 * it will fail to detect the beginning of HTML content
			 */
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
		exit(1);
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
		exit(1);
	}
	if (inet_ntop(AF_INET, (void *)hent->h_addr_list[0], ip, iplen+1) == NULL) {
		perror("Can't resolve host");
		exit(1);
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
		//		page, getpage);
	}
	// -5 is to consider the %s %s %s in tpl and the ending \0
	query = (char *)malloc(strlen(host)+strlen(getpage)
			+strlen(USERAGENT)+strlen(tpl)-5);
	sprintf(query, tpl, getpage, host, USERAGENT);
	return query;
}
