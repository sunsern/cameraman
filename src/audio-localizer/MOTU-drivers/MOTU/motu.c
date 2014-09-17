/** MOTU IOCNTRL **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <motu.h>
#include "portaudio.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>                     /* This ert_main.c example uses printf/fflush */

/* Select sample format. */

#define PA_SAMPLE_TYPE  paFloat32
typedef float SAMPLE;
#define SAMPLE_SILENCE  (0.0f)
#define PRINTF_S_FORMAT "%.8f"
#define FALSE 0
#define TRUE 1

int num_channels;

/* This routine will be called by the PortAudio engine when audio is needed.
** It may be called at interrupt level on some machines so don't do anything
** that could mess up the system like calling malloc() or free().
*/
static int recordCallback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData )
{    
  (void) outputBuffer; /* Prevent unused variable warnings. */
  float * in = (float *) inputBuffer;
  (void) timeInfo;
  (void) statusFlags;
  //Fill Input Buffer
  MOTU * motu = (MOTU*) userData;
  int i,j;
  pthread_mutex_lock(&motu->input_lock);
  for(i=0; i < framesPerBuffer; i++) {
      for(j=0;j< motu->numChannels;j++) {
	  motu->inputBuffer[j][motu->headInput] = (float) *in;
#ifdef RECORD
	  fprintf(motu->file, "%f ", (float) *in);
#endif
	  in++;
      }
   motu->headInput++;
   motu->headInput %= motu->inputBufferSize;
//      printf("\n");
  }
  /*Signal sleeping threads that new input is available*/
  pthread_cond_broadcast(&motu->input_cond);
  pthread_mutex_unlock(&motu->input_lock);
  return paContinue;
}

static int playbackCallback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData )
{    
    (void) inputBuffer;
    (void) timeInfo;
    (void) statusFlags;
    //Write the output buffer to Soundflower
    int num = (int) framesPerBuffer;
    MOTU * motu = (MOTU *) userData;
    float * out = (float *) outputBuffer;
    pthread_mutex_lock(&motu->output_lock);
    /*Is new data ready to send? If not wait till signaled*/
    while(!motu->freshOutput)
	pthread_cond_wait(&motu->output_cond,&motu->output_lock);
    int i;
/*     post("Sending to soundflower: %f", motu->outputBuffer[0]); */
    for(i=0; i < num; i++) {
	*out = motu->outputBuffer[i];
	motu->outputBuffer[i] = 0;
	out++;
    }
    motu->freshOutput=0;
    pthread_mutex_unlock(&motu->output_lock);
    return paContinue;
}

MOTU * StartMOTU(int inputFramesPerBuffer, 
		 int outputFramesPerBuffer, 
		 int sampleRate, 
		 int numChannels) 
{
  PaStreamParameters inputParameters, outputParameters;
  PaError err = paNoError;
  MOTU * motu;
  
  motu = (MOTU *) malloc(sizeof(MOTU));
#ifdef RECORD
  post("RECORDING TO FILE...");
  motu->file = fopen("/Users/cameraman/Desktop/motu_recording.dat","wb");
  if(!motu->file) {
      post("ERROR opening file!");
  }
#endif
  motu->inputStream = (PaStream *) malloc(sizeof(PaStream*));
  motu->outputStream = (PaStream *) malloc(sizeof(PaStream*));

  /* Set the global userCallback to callback*/
  motu->inputFramesPerBuffer = inputFramesPerBuffer;
  motu->outputFramesPerBuffer = outputFramesPerBuffer;
  motu->sampleRate = sampleRate;
  motu->numChannels = numChannels;
  motu->inputBuffer = (float **) malloc(sizeof(float*)*numChannels);
  motu->inputBufferSize = 10*sampleRate;
  int i;
  for(i=0; i < numChannels; i++) {
      motu->inputBuffer[i] = (float *) malloc(sizeof(float)*motu->inputBufferSize);
  }
  motu->outputBuffer = (float*) malloc(sizeof(float)*outputFramesPerBuffer);
  motu->outputBufferSize = outputFramesPerBuffer;
  motu->headInput = motu->beamformPos = 0;
  pthread_mutex_init(&motu->input_lock,NULL);
  pthread_mutex_init(&motu->output_lock,NULL);
  pthread_cond_init(&motu->input_cond, NULL);
  pthread_cond_init(&motu->output_cond, NULL);

  /* Init portaudio */
  err = Pa_Initialize();
  if( err != paNoError ) return NULL; 

  /*Find the appropriate device number*/
  int numDevices = Pa_GetDeviceCount();
  const PaDeviceInfo * deviceInfo;
  inputParameters.device = -1;
  outputParameters.device = -1;
  for(i = 0; i < numDevices; i++) {
      deviceInfo = Pa_GetDeviceInfo(i);
      if(strstr(deviceInfo->name, "MOTU")) {
	  inputParameters.device = i;
      }
      if(strstr(deviceInfo->name, "Soundflower (2ch)")) {
	  outputParameters.device = i;
      }
  }
  if(inputParameters.device == -1) {
      post("Can't open portaudio stream to MOTU!\n");
      exit(0);
  }
  if(outputParameters.device == -1) {
      post("Can't open portaudio stream to Soundflower!\n");
      exit(0);
  }

  /* How many channels are hooked up to record? */
  inputParameters.channelCount = numChannels;
  /*float samples */
  inputParameters.sampleFormat = PA_SAMPLE_TYPE;
  inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
  inputParameters.hostApiSpecificStreamInfo = NULL;
  
  /* Fill up output device info */
  outputParameters.sampleFormat = PA_SAMPLE_TYPE;
  outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
  outputParameters.hostApiSpecificStreamInfo = NULL;
  outputParameters.channelCount = 1;

  /* Record some audio. -------------------------------------------- */
  err = Pa_OpenStream(
          &motu->inputStream,
          &inputParameters,
          NULL,                  /* &outputParameters, */
          sampleRate,
          motu->inputFramesPerBuffer,
          paClipOff,      /* we won't output out of range samples so don't bother clipping them */
          recordCallback,
          motu);
  if( err != paNoError ) {
      post("Error opening input stream!!!!\n");
      return NULL;
  }
  err = Pa_StartStream( motu->inputStream );
  if( err != paNoError ) {
      post("Error starting input stream!!!\n");
      return NULL;
  }
  err = Pa_OpenStream(
          &motu->outputStream,
          NULL,
          &outputParameters,                  /* &outputParameters, */
          sampleRate,
          motu->outputFramesPerBuffer,
          paClipOff,      /* we won't output out of range samples so don't bother clipping them */
          playbackCallback,
          motu);
  if( err != paNoError ) {
      post("Error opening output stream!!!\n");
      return NULL;
  }
  
 
  err = Pa_StartStream( motu->outputStream );
  if( err != paNoError ) {
      post("Error starting output stream!!!\n");
      return NULL;
  }
  
 
  
  return motu;
}

char StopMOTU(MOTU * motu)
{
  PaError err = paNoError;

/* TODO: WHY CAN'T I STOP SOUNDFLOWER!!!!*/
  err = Pa_CloseStream( motu->outputStream );     
  if( err != paNoError ) return FALSE; 

  err = Pa_CloseStream( motu->inputStream );     
  if( err != paNoError ) return FALSE;

  Pa_Terminate();

#ifdef RECORD
  fclose(motu->file);
#endif  

  int i;
  for(i = 0; i < motu->numChannels; i++) {
      free(motu->inputBuffer[i]);
  }
  free(motu->inputBuffer);
  free(motu->outputBuffer);
  free(motu->inputStream);
  free(motu->outputStream);
  free(&motu->input_lock);
  free(motu);
  motu = NULL;
  return TRUE;
}     
