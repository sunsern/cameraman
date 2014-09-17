/* cameracontroller.c -- output the maximum of a group of numbers ------- */

#include "ext.h"
#include <unistd.h>
#include <sys/types.h>
#include "PHAT.h"
#include "rtwtypes.h"
#include <stdio.h>
#include <sys/time.h>
#include "motu.h"
#include <pthread.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <sched.h>
#include <semaphore.h>
#include "TdoaTable.h"

int gNUM_CHANNELS=7;
#define OUTPUT_BUFFER_SIZE 1600
static boolean_T OverrunFlag=0;     

typedef struct _controller
{
    void * m_panout;                                                           
    void * m_tiltout;                                                         
    void * m_rtout;                                                          
    void * m_energy;    
    double * pan_buff;
    double * tilt_buff;
    double * time_buff;
    double * energy_buff;
    int * index_buff;
    int audio_buff_pointer;
    int audio_buff_size;
    double current_pan;
    double current_tilt;
    int num_table_elems;
    double start_time;
    MOTU * motu;
    int kill_flag;
    int tdoa_array[7];
    float ** channel;
    FILE * scorefile;
} t_controller;

int doSrpPhat(t_controller *, float *);
void smoothPhats(t_controller *);
void smoothSeries(real32_T *,int);
void Run_Localization(t_controller * x, int samples);
void controller_free(t_controller * x);
void Init(t_controller * x);
void Go(t_controller * x);

void rt_OneStep(void)
{
    /* Disable interrupts here */
    /* Check for overrun */
    if (OverrunFlag++) {
	rtmSetErrorStatus(PHAT_M, "Overrun"); 
	return;                                
    }   

    /* Save FPU context here (if necessary) */   
    /* Re-enable timer or interrupt here */
    /* Set model inputs here */
    PHAT_step();

    /* Get model outputs here */ 
    OverrunFlag--; 

    /* Disable interrupts here */
    /* Restore FPU context here (if necessary) */ 
    /* Enable interrupts here */   
}

int main() {
    /*Initialize tables*/
    t_controller * x = (t_controller *) malloc(sizeof(t_controller));
    Init(x);

    /*read in audio file, and do SRP-PHAT*/
    Go(x);
    
    /*clean up*/
    controller_free(x);
    return 0;
}

void Go(t_controller * x) {
	int i,j;
	FILE * fid = fopen("/Users/cameraman/Desktop/cameraman/mac/MaxAudioLocalizer/MaxMSP_UB_SDK/evans-externs/controller/motu_recording.dat","rb");
    if(!fid) {
	printf("Error: cannot find motu_recording.dat\n");
	exit(0);
    }
    int samples = 0;
    float temp;
    for(i=0; i < 1600; i++) {
	for(j=0; j < 8; j++) {	    
	    fscanf(fid, "%f ", &temp);
	    if(j != 0) 
		x->channel[j-1][i] = temp;
	}
    }
    samples = 1600;
    Run_Localization(x, samples);
    while(1) {
	for(j=0; j < 7; j++) {
	    memcpy(&(x->channel[j][400]), &(x->channel[j][0]), sizeof(float)*1200);
	}
	for(i=0; i < 400; i++) {
	    for(j=0; j < 8; j++) {
		if(fscanf(fid, "%f ", &temp) == EOF)
		    return;
		if(j != 0)
		    x->channel[j-1][i] = temp;
	    }
	}
	samples += 400;
	Run_Localization(x,samples);
    }
}

void Init(t_controller * x)
{	
	struct timeval tim;
	gettimeofday(&tim,NULL);
	double t1=tim.tv_sec+(tim.tv_usec/1000000.0);
	x->start_time = t1;       
	x->kill_flag = 1;
	x->audio_buff_size = 50;
	x->audio_buff_pointer = 0;
	x->pan_buff = (double *) malloc(sizeof(double)*x->audio_buff_size);
	x->tilt_buff = (double *) malloc(sizeof(double)*x->audio_buff_size);
	x->time_buff = (double *) malloc(sizeof(double)*x->audio_buff_size);
	x->energy_buff = (double *) malloc(sizeof(double)*x->audio_buff_size);
	x->index_buff = (int *) malloc(sizeof(int)*x->audio_buff_size);
	x->current_pan = 0;
	x->current_tilt = 0;
	x->channel = (float **) malloc(sizeof(float *)*gNUM_CHANNELS);
	x->scorefile = fopen("./scorefile.dat","wb");
	int i;
	for(i = 0; i < gNUM_CHANNELS; i++) {
	    x->tdoa_array[i] = 0;
	    x->channel[i] = (float *) malloc(sizeof(float)*1600);
	}
}

void controller_free(t_controller *x) {
    fclose(x->scorefile);
    free(x->pan_buff);
    free(x->tilt_buff);
    free(x->time_buff);
    free(x->energy_buff);
	int i;
	for(i=0; i < gNUM_CHANNELS; i++) {
		free(x->channel[i]);
	}
	free(x->channel);
	free(x);
    PHAT_terminate();
}


static float large_smoother[21] = {7.4336e-07,7.9919e-06,6.6915e-05,0.00043634,0.0022159,0.0087642,0.026995,0.064759,0.12099,0.17603,0.19947,0.17603,0.12099,0.064759,0.026995,0.0087642,0.0022159,0.00043634,6.6915e-05,7.9919e-06,7.4336e-07};  
static int large_smoother_size = 21;

void smoothSeries(real32_T * buf, int size) {
    //Do gaussian smoothing 
    int i,j;
    int mid = (int) large_smoother_size/2;
    real32_T temp_buf[size];
    
    for(i=0; i < size; i++) {
	temp_buf[i] = 0; 
	for(j=0; j < large_smoother_size; j++) {
	    if(j < mid) {
		int diff = mid - j;
		temp_buf[i] += large_smoother[j]*((i >= diff) ? buf[i - diff] : buf[i]);
	    }   
	    else if(j > mid) {
		int diff = j - mid;
		temp_buf[i] += large_smoother[j]*                      
                    ((i < size - diff) ? buf[i + diff] : buf[i]);
	    }   
	    else {
		temp_buf[i] += large_smoother[j]*buf[i];                
	    } 
	}    
    }
    memcpy(&(buf[0]), &(temp_buf[0]), sizeof(real32_T)*size); 
}

void smoothPhats(t_controller * x) {
    int PHAT_SIZE = 1001;
    fwrite(&PHAT_Y.R12, sizeof(float), 1001, x->scorefile);
    smoothSeries(PHAT_Y.R12, PHAT_SIZE);
    fwrite(&PHAT_Y.R12, sizeof(float), 1001, x->scorefile);
    fwrite(&PHAT_Y.R13, sizeof(float), 1001, x->scorefile);
    smoothSeries(PHAT_Y.R13, PHAT_SIZE);
    fwrite(&PHAT_Y.R13, sizeof(float), 1001, x->scorefile);
    fwrite(&PHAT_Y.R14, sizeof(float), 1001, x->scorefile);
    smoothSeries(PHAT_Y.R14, PHAT_SIZE);
    fwrite(&PHAT_Y.R14, sizeof(float), 1001, x->scorefile);
    fwrite(&PHAT_Y.R15, sizeof(float), 1001, x->scorefile);
    smoothSeries(PHAT_Y.R15, PHAT_SIZE); 
    fwrite(&PHAT_Y.R15, sizeof(float), 1001, x->scorefile);
    fwrite(&PHAT_Y.R16, sizeof(float), 1001, x->scorefile);
    smoothSeries(PHAT_Y.R16, PHAT_SIZE); 
    fwrite(&PHAT_Y.R16, sizeof(float), 1001, x->scorefile);
    fwrite(&PHAT_Y.R17, sizeof(float), 1001, x->scorefile);
    smoothSeries(PHAT_Y.R17, PHAT_SIZE);                   
    fwrite(&PHAT_Y.R17, sizeof(float), 1001, x->scorefile);
    fwrite(&PHAT_Y.R23, sizeof(float), 1001, x->scorefile);            
    smoothSeries(PHAT_Y.R23, PHAT_SIZE);                   
    fwrite(&PHAT_Y.R23, sizeof(float), 1001, x->scorefile);
    fwrite(&PHAT_Y.R24, sizeof(float), 1001, x->scorefile);             
    smoothSeries(PHAT_Y.R24, PHAT_SIZE);                   
    fwrite(&PHAT_Y.R24, sizeof(float), 1001, x->scorefile);
    fwrite(&PHAT_Y.R25, sizeof(float), 1001, x->scorefile);           
    smoothSeries(PHAT_Y.R25, PHAT_SIZE);          
    fwrite(&PHAT_Y.R25, sizeof(float), 1001, x->scorefile);
    fwrite(&PHAT_Y.R26, sizeof(float), 1001, x->scorefile);
    smoothSeries(PHAT_Y.R26, PHAT_SIZE);                   
    fwrite(&PHAT_Y.R26, sizeof(float), 1001, x->scorefile);
    fwrite(&PHAT_Y.R27, sizeof(float), 1001, x->scorefile);             
    smoothSeries(PHAT_Y.R27, PHAT_SIZE);                   
    fwrite(&PHAT_Y.R27, sizeof(float), 1001, x->scorefile);
    fwrite(&PHAT_Y.R34, sizeof(float), 1001, x->scorefile);           
    smoothSeries(PHAT_Y.R34, PHAT_SIZE);                   
    fwrite(&PHAT_Y.R34, sizeof(float), 1001, x->scorefile);
    fwrite(&PHAT_Y.R35, sizeof(float), 1001, x->scorefile); 
    smoothSeries(PHAT_Y.R35, PHAT_SIZE);                   
    fwrite(&PHAT_Y.R35, sizeof(float), 1001, x->scorefile);
    fwrite(&PHAT_Y.R36, sizeof(float), 1001, x->scorefile);
    smoothSeries(PHAT_Y.R36, PHAT_SIZE);                   
    fwrite(&PHAT_Y.R36, sizeof(float), 1001, x->scorefile);
    fwrite(&PHAT_Y.R37, sizeof(float), 1001, x->scorefile);     
    smoothSeries(PHAT_Y.R37, PHAT_SIZE);                   
    fwrite(&PHAT_Y.R37, sizeof(float), 1001, x->scorefile);
    fwrite(&PHAT_Y.R45, sizeof(float), 1001, x->scorefile);           
    smoothSeries(PHAT_Y.R45, PHAT_SIZE);                   
    fwrite(&PHAT_Y.R45, sizeof(float), 1001, x->scorefile);
    fwrite(&PHAT_Y.R46, sizeof(float), 1001, x->scorefile);             
    smoothSeries(PHAT_Y.R46, PHAT_SIZE);                   
    fwrite(&PHAT_Y.R46, sizeof(float), 1001, x->scorefile);
    fwrite(&PHAT_Y.R47, sizeof(float), 1001, x->scorefile);   
    smoothSeries(PHAT_Y.R47, PHAT_SIZE);                   
    fwrite(&PHAT_Y.R47, sizeof(float), 1001, x->scorefile);
    fwrite(&PHAT_Y.R56, sizeof(float), 1001, x->scorefile);            
    smoothSeries(PHAT_Y.R56, PHAT_SIZE);                   
    fwrite(&PHAT_Y.R56, sizeof(float), 1001, x->scorefile);
    fwrite(&PHAT_Y.R57, sizeof(float), 1001, x->scorefile);            
    smoothSeries(PHAT_Y.R57, PHAT_SIZE);                   
    fwrite(&PHAT_Y.R57, sizeof(float), 1001, x->scorefile);
    fwrite(&PHAT_Y.R67, sizeof(float), 1001, x->scorefile);      
    smoothSeries(PHAT_Y.R67, PHAT_SIZE);                   
    fwrite(&PHAT_Y.R67, sizeof(float), 1001, x->scorefile);      
}                                                 

void getTDOA(int * tdoa,  int a, int b, int c) {
	int i;
    for(i=0; i < 21; i++) {
	tdoa[i] = (int) (((double)a)*TDOA_pcs[0][i] + 
			 ((double)b)*TDOA_pcs[1][i] + 
			 ((double)c)*TDOA_pcs[2][i]) + 501;	
    }
}

int doSrpPhat(t_controller * x, float * maxVal) { 
    //Smooth PHAT-correlation series                                  
    smoothPhats(x);     
    int i,j,k; int retIndex = 0; float max = -10000000; float curr = 0;             
    int temp_tdoa[21];                                     
    int jump = 1;
    for(i=TDOA_pcs_bounds[0][0]; i <= TDOA_pcs_bounds[0][1]; i+=jump) {                                
	for(j=TDOA_pcs_bounds[1][0]; j <= TDOA_pcs_bounds[1][1]; j+=jump) {                                
	    for(k=TDOA_pcs_bounds[2][0]; k <= TDOA_pcs_bounds[2][1]; k+=jump) {                                
		getTDOA(temp_tdoa,i,j,k);                          
		curr =    PHAT_Y.R12[temp_tdoa[0]] +                   
		    PHAT_Y.R13[temp_tdoa[1]] +                   
		    PHAT_Y.R14[temp_tdoa[2]] +                        
		    PHAT_Y.R15[temp_tdoa[3]] +                    
		    PHAT_Y.R16[temp_tdoa[4]] +                         
		    PHAT_Y.R17[temp_tdoa[5]] +                           
		    PHAT_Y.R23[temp_tdoa[6]] +                     
		    PHAT_Y.R24[temp_tdoa[7]] +                        
		    PHAT_Y.R25[temp_tdoa[8]] +                    
		    PHAT_Y.R26[temp_tdoa[9]] +                
		    PHAT_Y.R27[temp_tdoa[10]] +                     
		    PHAT_Y.R34[temp_tdoa[11]] +                         
		    PHAT_Y.R35[temp_tdoa[12]] +                      
		    PHAT_Y.R36[temp_tdoa[13]] +                   
		    PHAT_Y.R37[temp_tdoa[14]] +                     
		    PHAT_Y.R45[temp_tdoa[15]] +                    
		    PHAT_Y.R46[temp_tdoa[16]] +                       
		    PHAT_Y.R47[temp_tdoa[17]] +                     
		    PHAT_Y.R56[temp_tdoa[18]] +                   
		    PHAT_Y.R57[temp_tdoa[19]] +                
		    PHAT_Y.R67[temp_tdoa[20]];  
		if(curr > max) {                                      
		    max = curr;                                               
		    retIndex = i;                                                
		}
		fwrite(&curr, sizeof(float), 1, x->scorefile);
	    }
	}
    }
    *maxVal = max;
    return retIndex;                                                
}                               

void Run_Localization(t_controller * x, int samples) {
    struct timeval tim;
    gettimeofday(&tim, NULL);
    double t1=tim.tv_sec+(tim.tv_usec/1000000.0) - x->start_time;
    
    memcpy(&(PHAT_U.Channel1[0]), &(x->channel[0][0]),
	   sizeof(float)*OUTPUT_BUFFER_SIZE);
    memcpy(&(PHAT_U.Channel2[0]), &(x->channel[1][0]),
	   sizeof(float)*OUTPUT_BUFFER_SIZE);
    memcpy(&(PHAT_U.Channel3[0]), &(x->channel[2][0]),
	   sizeof(float)*OUTPUT_BUFFER_SIZE);
    memcpy(&(PHAT_U.Channel4[0]), &(x->channel[3][0]),
	   sizeof(float)*OUTPUT_BUFFER_SIZE);
    memcpy(&(PHAT_U.Channel5[0]), &(x->channel[4][0]),
	   sizeof(float)*OUTPUT_BUFFER_SIZE);
    memcpy(&(PHAT_U.Channel6[0]), &(x->channel[5][0]),
	   sizeof(float)*OUTPUT_BUFFER_SIZE);
    memcpy(&(PHAT_U.Channel7[0]), &(x->channel[6][0]),
	   sizeof(float)*OUTPUT_BUFFER_SIZE);
    
    //Calculate energy
    double energy = 0;
    int i;
    for(i=0; i < OUTPUT_BUFFER_SIZE; i++) {
	energy += pow(PHAT_U.Channel1[i],2);
    }
    
    if(energy < 0.7) return;
    fwrite(&samples, sizeof(int), 1, x->scorefile);
    float f_energy = (float) energy;
    fwrite(&f_energy, sizeof(float), 1, x->scorefile);
    //Calculate PHAT time-series
    rt_OneStep();
    
    int ret;
    float maxVal;
    ret = doSrpPhat(x, &maxVal);
    
    gettimeofday(&tim, NULL);
    double t2=tim.tv_sec+(tim.tv_usec/1000000.0) - x->start_time;
    printf("time took: %2.2f ms\n", (t2-t1)*1000.0);
}
