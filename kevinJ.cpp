//Author: Kevin Jenkin
//Project: Upstream
//Basic Function: Play and Cleanup sound files && how to score the game

#include "kevinJ.h"

//Global Sound Variable to set Background music source and buffer.
ALuint musicSource;
ALuint musicBuffer;

void playSounds(const char * sound, float gain, bool loop, bool muted)
{
	 alutInit(0, NULL);
	 if (alGetError() != AL_NO_ERROR) {
		  printf("ERROR: alutInit()\n");
		  return;
	 }
	 //Clear error state.
	 alGetError();
	 //
	 //Setup the listener.
	 //Forward and up vectors are used.
	 float vec[6] = {0.0f,0.0f,1.0f, 0.0f,1.0f,0.0f};
	 alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
	 alListenerfv(AL_ORIENTATION, vec);
	 alListenerf(AL_GAIN, 1.0f);
	 ALuint alBuffer;
	 alBuffer = alutCreateBufferFromFile(sound);
	 //printf("sound: %s\n", sound);

	 //Generate a source, and store it in a buffer.
	 ALuint alSource;
	 alGenSources(1, &alSource);
	 alSourcei(alSource, AL_BUFFER, alBuffer);
	 //Set volume and pitch to normal, no looping of sound.
	 //gain is volume of sound
	 alSourcef(alSource, AL_GAIN, gain);
	 alSourcef(alSource, AL_PITCH, 1.0f);
	 alSourcei(alSource, AL_LOOPING, loop);
	 if (alGetError() != AL_NO_ERROR) {
		  printf("ERROR: setting source\n");
		  return;
	 }
	 alSourcePlay(alSource);
	 if (muted) {
		  alSourceStop(alSource);
	 }
	 if (strcmp(sound,"./wav/background.wav") == 0) { 
		  musicSource = alSource;
		  musicBuffer = alBuffer;
	 }
}

void stopMusic() 
{ 
	 alSourceStop(musicSource);
}

void playMusic() 
{ 
	 alSourcePlay(musicSource);
}
	
void maxScore(Game *game)
{
	 int max = game->highscore[0];
	 for (int i = 0; i < game->scoreCount; i++)
	 {
		  if (game->highscore[i] > max)
		  {
				max = game->highscore[i];
		  }
	 }
	 game->highscore[0] = max;
	 //return max;
}
