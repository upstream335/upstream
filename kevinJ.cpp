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
//

Game *g;
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
	else {
		printf("That Sound File Doesn't exist\n\n");
		return -1;
	}
}

void playSounds(const char * sound, float gain, bool loop, bool muted)
{
	//Set volume and pitch to normal, no looping of sound.
	//gain is volume of sound
	int index = getSource(sound);
	std::cout << "index: " << index << "\n";
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
