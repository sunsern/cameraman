#ifndef MOTU_H
#define MOTU_H

//#define RECORD

#include "portaudio.h"
#include <pthread.h>
#include <semaphore.h>

typedef struct
{
    PaStream * inputStream;   /* stream pointer */
    PaStream * outputStream;   /* stream pointer */
    volatile int inputFramesPerBuffer; /* frames per buffer to fill */
    volatile int outputFramesPerBuffer; /* frames per buffer to fill */
    volatile int sampleRate;      /* sample rate of MOTU */  
    volatile int numChannels;     /* number of channels MOTU is asked to record */
    
    pthread_mutex_t input_lock; /*mutex for input variables */
    pthread_cond_t input_cond; /*condition variable to wake up threads for input*/
    float ** inputBuffer;
    volatile int headInput;  /*points to the index of the front of inputBuffer*/
    volatile int beamformPos; /*points to the index where beamforming must continue from */
    volatile int inputBufferSize; /*How long is each inputBuffer*/

    pthread_mutex_t output_lock; /*mutex for input variables */
    pthread_cond_t output_cond; /*condition variable to wake up threads for input*/
    float * outputBuffer;
    volatile int outputBufferSize;
    volatile int freshOutput; /*0 if output isn't fresh, 1 if it is*/
#ifdef RECORD
    FILE * file; 
#endif
}
MOTU;

/* Starts the MOTU recording
   callback - pointer to the callback fcn that gets called 
              whenever a new window arrives 

   framesPerBuffer - how big the buffer should be in frames 
   sampleRate - how fast the MOTU should sample at (e.g. 48000, 44100, 32000, 16000, etc.)*/
MOTU * StartMOTU(int , int , int, int);

/* Stops the MOTU * stream started with Start() */
char StopMOTU(MOTU * motu);

#endif
