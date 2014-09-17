#include <stdio.h>
#include <stdlib.h>
#include "motu.h"
#include <unistd.h>

int main() {
     int count = 0;
     int inputBufferSize = 400;
     int outputBufferSize = 1600;
     int samplingRate = 16000;
     int numInputChannels = 8;
     MOTU * motu;

     motu = StartMOTU(inputBufferSize, outputBufferSize,samplingRate,numInputChannels);     
     while(count < 100) {
       sleep(1);
       count++;
     }
     StopMOTU(motu);
     printf("\n\n");
     return 0;
}
