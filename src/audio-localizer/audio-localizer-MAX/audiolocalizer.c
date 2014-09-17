/* audiolocalizer.c -- Run PHAT on 7 channels of audio and predict pan/tilt for camera ------- */

#include "ext.h"
#include <unistd.h>
#include <sys/types.h>
#include "CalcPhat.h"
#include "FFT.h"
#include "rtwtypes.h"
#include <stdio.h>
#include <sys/time.h>
#include "motu.h"
#include <pthread.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sched.h>
#include <semaphore.h>
#include "TdoaMap.h"
#include "NH_pf.h"

#define ENERGY_THRESHOLD 0.3
#define MAXSIZE 256
#define OUTPUT_BUFFER_SIZE 1600
double rel_energies[7] = {1.0,1.0,1.0,1.0,1.0,1.0,1.0}; 
double energy_mean = 0.2;
double energy_std = 0.05;
int gNUM_CHANNELS=7;

typedef struct _controller
{
    struct object m_ob;
    void * m_panout;                                                           
    void * m_tiltout;                                                         
    void * m_rtout;                                                          
    void * m_energy;   
    void * m_messageout;
    void * m_numparticles;
    void * m_processing;
    int audio_buff_pointer;
    int audio_buff_size;
    double start_time;
    MOTU * motu;
    int kill_flag;
    int tdoa_array[7];
    int audio_record;
    int audio_save;
    char audio_filename[64];
    FILE * audio_file;
    pthread_mutex_t lock;
} t_controller;

void *controller_class;

void controller_assist(t_controller *x, void *b, long m, long a, char *s);
void *controller_new(t_symbol *s, short ac, t_atom *av);
void audio_event(t_controller *x, double pan, double tilt, double time);
void controller_free(t_controller *x);
void controller_int(t_controller *x, long n);
void controller_message(t_controller *x, t_symbol * s, short argc, t_atom * argv);

int doSrpPhat(t_controller *, float *);
void smoothPhats(void);
void smoothSeries(real32_T *,int);
void *Run_Bform(t_controller * x);
void *Run_Localization(t_controller * x);

static boolean_T FFTOverrunFlag=0;     
void Run_FFT(void)
{
    if (FFTOverrunFlag++) {
	rtmSetErrorStatus(FFT_M, "Overrun"); 
	return;                                
    }   
    FFT_step();
    FFTOverrunFlag--; 
}

static boolean_T PHATOverrunFlag=0;     
void Run_PHAT(void)
{
    if (PHATOverrunFlag++) {
	rtmSetErrorStatus(CalcPhat_M, "Overrun"); 
	return;                                
    }   
    CalcPhat_step();
    PHATOverrunFlag--; 
}

int main() {
    setup((t_messlist **)&controller_class, (method)controller_new, (method)controller_free, (short)sizeof(t_controller), 0L, A_GIMME, 0);
    addmess((method)controller_assist,"assist",A_CANT,0);
    addint((method)controller_int);
    addmess((method) controller_message,"start_recording",A_GIMME,0);
    addmess((method) controller_message,"stop_recording",A_GIMME,0);
    addmess((method) controller_message,"save_recording",A_GIMME,0);
    return 0;
}

void ExecCmd(char * cmd) {
    pid_t pid = fork();
    if(pid==0) {
	execl("/bin/sh","sh","-c",cmd,NULL);
	exit(127); //Shouldn't get here if exec worked
    }
    signal(SIGCHLD, SIG_IGN); //Ignore the SIGCHILD signal when child exits so that we clean up zombie process
}

void controller_message(t_controller * x, t_symbol * s, short argc, t_atom * argv) {
    if(s == gensym("start_recording")) {
	post("Audio recording started");
	struct timeval tim;
	gettimeofday(&tim, NULL);
	pthread_mutex_lock(&x->lock);
	x->audio_record = 1;
	sprintf(x->audio_filename, "/Users/cameraman/Documents/Audio_Recordings/%d.dat", (int) tim.tv_sec);
	x->audio_file = fopen(x->audio_filename,"w");
	pthread_mutex_unlock(&x->lock);
    }
    else if(s == gensym("stop_recording")) {
	post("Audio recording stopped");
	pthread_mutex_lock(&x->lock);
	x->audio_record = 0;
	fclose(x->audio_file);
	x->audio_file = NULL;
/* 	if(!x->audio_save) { */
/* 	    unlink(x->audio_filename); */
/* 	} */
/* 	else { */
	char cmd[2048];
	sprintf(cmd, "/usr/bin/scp %s cameraman@seed.ucsd.edu:/mnt/usbdrive1/audio_recordings/ > /dev/null; rm /Users/cameraman/Documents/Audio_Recordings/*.dat;", x->audio_filename);
	ExecCmd(cmd);
/* 	} */
	x->audio_save = 0;
	pthread_mutex_unlock(&x->lock);
    }
    else if(s == gensym("save_recording")) {
	post("Going to save this audio recording!");
	pthread_mutex_lock(&x->lock);
	x->audio_save = 1;
	pthread_mutex_unlock(&x->lock);
    }
}

void Start(t_controller * x) {
	x->motu = StartMOTU(400/*INPUT_BUFFER_SIZE*/, 1600 /*OUTPUT_BUFFER_SIZE*/, 16000.0/*SAMPLING_RATE*/, 8/*NUM_CHANNELS*/);
	if(x->motu == NULL) {
	    post("Error starting MOTU!!!!!\n");
	}
	post("MOTU recording...\n");  
	
	InitPf();

	pthread_t threads[3];
	pthread_attr_t attr;
	if(pthread_attr_init(&attr)) {
	    post("ERROR: Can't init pthread attr\n");
	    exit(-1);
	}
	if(pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED)) {
	    post("ERROR: Can't set inherit pthread attr\n");
	    exit(-1);
	}
	if(pthread_attr_setschedpolicy(&attr, SCHED_RR)) {
	    post("ERROR: Can't set schedpolicy pthread attr\n");
	    exit(-1);
	}
	struct sched_param schedParam;
	schedParam.sched_priority = sched_get_priority_max(SCHED_RR);
	if (pthread_attr_setschedparam(&attr, &schedParam)) {
	    post("Failed to set scheduler parameters\n");
	    exit(-1);
	}
	
	int rc;
	/*Fire up beamforming thread*/
	rc = pthread_create(&threads[0], &attr, (void *) Run_Bform, x);
	if(rc) {
	    post("ERROR: Creating beamforming thread!");
	    exit(-1);
	}
	
	schedParam.sched_priority = sched_get_priority_max(SCHED_RR) - 1;
	if (pthread_attr_setschedparam(&attr, &schedParam)) {
	    post("Failed to set scheduler parameters\n");
	    exit(-1);
	}
	
	/*Fire up localization thread*/
	rc = pthread_create(&threads[1], &attr, (void *) Run_Localization, x);
	if(rc) {
	    post("ERROR: Creating localization thread!");
	    exit(-1);
	}
}

void controller_int(t_controller *x, long n) {
    switch(n) {
    case 0:
	x->kill_flag = 1;
	StopMOTU(x->motu);
	x->audio_record = 0;
	x->audio_save = 0;
	outlet_anything(x->m_messageout, gensym("stop"), 0,NULL);
	break;
    case 1:
	outlet_anything(x->m_messageout, gensym("start"), 0,NULL);
	x->kill_flag = 0;
	Start(x);
	break;
    default:
	break;
    }
}

void SetTdoa(t_controller * x, double * tdoa) {
    int max = 0;
    int i;

    pthread_mutex_lock(&x->lock);
    x->tdoa_array[0] = 0;
    for(i=0; i < 6; i++) {
	int r_tdoa = (int) round(tdoa[i]);
	x->tdoa_array[i+1] = r_tdoa;
	max = (r_tdoa > max) ? r_tdoa : max;
    }
    for(i=0; i < 7; i++) {
	x->tdoa_array[i] -= max;
    }
    pthread_mutex_unlock(&x->lock);
}

void controller_assist(t_controller *x, void *b, long m, long a, char *s)
{
	if (m==ASSIST_INLET) {
		switch (a) {
		case 0: sprintf(s,"Starts on 1 received, stops on 0"); break;	
		case 1: sprintf(s,"Receives messages to start/stop recording audio to file"); 
		}
	}
	else {
	    switch (a) {                                                    
	    case 0: sprintf(s,"(float) estimated pan"); break;              
	    case 1: sprintf(s,"(float) estimated tilt"); break;             
	    case 2: sprintf(s,"(float) how many ms it took to process the last 500ms of audio that we ran localization alg. on"); break;                   
	    case 3: sprintf(s,"(float) Mean square energy of last 500ms of audio"); break;    
	    case 4: sprintf(s,"(list) Message for controller of the form <pan> <tilt> <time>");
	    case 5: sprintf(s,"(list) Message for multislider display to show how many particles are each tree level");
	    }
	}
}

void *controller_new(t_symbol *s, short ac, t_atom *av)
{
	t_controller *x;	
	x = (t_controller *)newobject(controller_class);
	inlet_new(x,0L);
	struct timeval tim;
	gettimeofday(&tim,NULL);
	double t1=tim.tv_sec+(tim.tv_usec/1000000.0);
	x->start_time = t1;       
	x->kill_flag = 1;
	x->m_processing = outlet_new(x,0L);
	x->m_numparticles = outlet_new(x,0L);
	x->m_messageout = outlet_new(x,0L);
	x->m_energy = floatout(x);                                       
	x->m_rtout = floatout(x);                                         
	x->m_tiltout = floatout(x);                                     
	x->m_panout = floatout(x);      
	x->audio_record = 0;
	x->audio_save = 0;
	int i;
	for(i = 0; i < gNUM_CHANNELS; i++) {
	    x->tdoa_array[i] = 0;
	}
	pthread_mutex_init(&x->lock,NULL);

	return x;
}

void controller_free(t_controller *x) {
    FFT_terminate();
    CalcPhat_terminate();
}


void pcaTreePredict(int * tdoa, double * pan, double * tilt) {
    int nodeNum = 1;
    int i,j,n;

    while(nodeNum <= pcaTree.num/2) {
	n = nodeNum - 1;
	/*Subtract mean and project*/
	double p = 0.0;
	for(i=0; i < 21; i++) {
	    p += ((double) tdoa[i] - pcaTree.Node[n].mean[i])*pcaTree.Node[n].pcs[0][i];
	}
	nodeNum = (p - pcaTree.Node[n].thresh >= 0.0) ? nodeNum*2 + 1 : nodeNum*2;
    } 
    nodeNum--;

    /*Predict with linear regressor at leaf*/
    double c[3] = {0.0,0.0,0.0};
    for(i=0; i < 21; i++) {
	for(j=0; j < 3; j++) {
	    c[j] += (tdoa[i] - pcaTree.Node[nodeNum].mean[i])*pcaTree.Node[nodeNum].pcs[j][i];
	}
    }
    *pan = pcaTree.Node[nodeNum].beta[0][0] + c[0]*pcaTree.Node[nodeNum].beta[0][1] + 
	c[1]*pcaTree.Node[nodeNum].beta[0][2] + c[2]*pcaTree.Node[nodeNum].beta[0][3];
    *tilt = pcaTree.Node[nodeNum].beta[1][0] + c[0]*pcaTree.Node[nodeNum].beta[1][1] + 
	c[1]*pcaTree.Node[nodeNum].beta[1][2] + c[2]*pcaTree.Node[nodeNum].beta[1][3];
}

#define PHAT_BUFFER_SIZE 8000
#define FFT_BUFFER_SIZE 8192
#define NUM_CHANNELS 7

void *Run_Localization(t_controller * x) {
    post("Localization started!\n");
    MOTU * motu = (MOTU *) x->motu;
    creal32_T ** fftBuffers;
    real32_T ** channelBuffers;
    int * tdoa_vec;
    int i,j,k;
    double pf_tdoa[21];
    int num_depth[3] = {0,0,0};
    float projected_center[3] = {0.0,0.0,0.0};

    tdoa_vec = (int *) malloc(sizeof(int)*21);
    fftBuffers = (creal32_T **) malloc(sizeof(creal32_T *)*NUM_CHANNELS);
    channelBuffers = (real32_T **) malloc(sizeof(real32_T *)*NUM_CHANNELS);
    for(i=0; i < NUM_CHANNELS; i++) {
	fftBuffers[i] = (creal32_T *) malloc(sizeof(creal32_T)*FFT_BUFFER_SIZE);
	channelBuffers[i] = (real32_T *) malloc(sizeof(real32_T)*PHAT_BUFFER_SIZE);
    }

    while(!x->kill_flag) {
	pthread_mutex_lock(&motu->input_lock);
	pthread_cond_wait(&motu->input_cond, &motu->input_lock);

	struct timeval tim;
	gettimeofday(&tim, NULL);
	double t1=tim.tv_sec+(tim.tv_usec/1000000.0) - x->start_time;

	/*Copy audio into buffers that we can manipulate*/
	if(motu->headInput >= PHAT_BUFFER_SIZE) {
	    int start = motu->headInput - PHAT_BUFFER_SIZE;
	    for(i=0; i < NUM_CHANNELS; i++) {
		memcpy(&(channelBuffers[i][0]),&(motu->inputBuffer[i+1][start]),
		       sizeof(real32_T)*PHAT_BUFFER_SIZE);
	    }
	}
	else {
	    int start = motu->inputBufferSize + (motu->headInput - PHAT_BUFFER_SIZE); 
	    int jagInd = motu->inputBufferSize - start;
	    int remain = PHAT_BUFFER_SIZE - jagInd;
	    for(i=0; i < NUM_CHANNELS; i++) {
		memcpy(&(channelBuffers[i][0]), &(motu->inputBuffer[i+1][start]), 
		       sizeof(real32_T)*jagInd);
		memcpy(&(channelBuffers[i][jagInd]), &(motu->inputBuffer[i+1][0]), 
		       sizeof(real32_T)*remain);
	    }
	}   
	pthread_mutex_unlock(&motu->input_lock);
	
	/*Calculate the Energy in Mic 1*/
	float energy[7] = {0,0,0,0,0,0,0};
	for(i=0; i < PHAT_BUFFER_SIZE; i++) {
	    for(j=0; j < NUM_CHANNELS; j++) {
		energy[j] += channelBuffers[j][i]*channelBuffers[j][i];
	    }
	}
	float sum_energy = 0.0;
	for(i = 0; i < NUM_CHANNELS; i++) {
	    energy[i] = sqrt(energy[i]);
	    sum_energy += energy[i];
	    if(i > 0)
		rel_energies[i] = 0.01*energy[0]/energy[i] + 0.99*rel_energies[i];
	}
/*	double energy_zscore = (energy[0] - energy_mean)/sqrt(energy_std); */
	outlet_float(x->m_energy, energy[0]);
	if(energy[0] < ENERGY_THRESHOLD) {
/*	    energy_mean = .99*energy_mean + 0.01*energy[0];
	    energy_std = .99*energy_std + 0.01*pow(energy[0] - energy_mean,2); */
	    StepPf(x->m_numparticles, x->m_processing, pf_tdoa, NULL, 0, num_depth, projected_center);	    
	    continue;
	}
/*	post("audio (energies): (%2.2f, %2.2f, %2.2f, %2.2f, %2.2f, %2.2f)", rel_energies[1], 
	rel_energies[2], rel_energies[3], rel_energies[4], rel_energies[5], rel_energies[6]); */
	/* Calculate the FFTs */
	for(i=0; i < NUM_CHANNELS; i++) {
	    memcpy(&FFT_U.audioIn[0],&channelBuffers[i][0],sizeof(real32_T)*PHAT_BUFFER_SIZE);
	    Run_FFT();
	    memcpy(&(fftBuffers[i][0]),&(FFT_Y.fftOut[0]),sizeof(creal32_T)*FFT_BUFFER_SIZE);
	}

	/* Calculate PHATs and build Zscore vector */
	int count = 0;
	Zscore * zptrs[NUM_PAIRS];
	int n_zs[NUM_PAIRS];
	int tot_zs = 0;
	for(i=0; i < NUM_CHANNELS; i++) {
	    for(j=i+1; j < NUM_CHANNELS; j++) {
		memcpy(&CalcPhat_U.fftIn1[0],&fftBuffers[i][0],sizeof(creal32_T)*FFT_BUFFER_SIZE);
		memcpy(&CalcPhat_U.fftIn2[0],&fftBuffers[j][0],sizeof(creal32_T)*FFT_BUFFER_SIZE);
		Run_PHAT();
/*		CalcPhat_Y.phatOut[200] = 0; CalcPhat_Y.phatOut[201] = 0; CalcPhat_Y.phatOut[202] = 0;*/
		float sum_x = 0.0;
		float sum_x2 = 0.0;
		float max = CalcPhat_Y.phatOut[0];
		int max_ind = 0;
		for(k=0; k < 401; k++) {
		    sum_x += CalcPhat_Y.phatOut[k];
		    sum_x2 += pow(CalcPhat_Y.phatOut[k],2);
		    if(CalcPhat_Y.phatOut[k] > max) {
			max = CalcPhat_Y.phatOut[k];
			max_ind = k;
		    }
		}
		pf_tdoa[count] = max_ind - 201;
		float mean = sum_x/401.0;
		float sigma = sqrt(sum_x2/401.0 - pow(mean,2));
		n_zs[count] = 0;
		for(k = 0; k < 401; k++) {
		    float zscore = (CalcPhat_Y.phatOut[k] - mean)/sigma;
		    if(zscore > 4) {
			CalcPhat_Y.phatOut[k] = zscore - 4.0;
			n_zs[count]++;
			tot_zs++;
		    }
		    else {
			CalcPhat_Y.phatOut[k] = 0.0;
		    }		    
		}
		zptrs[count] = malloc(sizeof(Zscore)*n_zs[count]);
		int ii = 0;
		for(k = 0; k < n_zs[count]; k++) {
		    while(CalcPhat_Y.phatOut[ii] == 0.0 && ii < 401) ii++;
		    zptrs[count][k].zscore = CalcPhat_Y.phatOut[ii];
		    zptrs[count][k].tdoa = ii - 201;
		    zptrs[count][k].pair = count;
		}
		count++;
	    }
	}

	/* Build Zscore structure */
	Zscore * z = malloc(sizeof(Zscore)*tot_zs);
	count = 0;
	for(i = 0; i < NUM_PAIRS; i++) {
	    for(j = 0; j < n_zs[i]; j++) {
		z[count].zscore = zptrs[i][j].zscore;
		z[count].pair = zptrs[i][j].pair;
		z[count].tdoa = zptrs[i][j].tdoa;
		count++;
	    }
	    free(zptrs[i]);
	}	
	

	/* Do NH_PF Step */
	ReInitPf(pf_tdoa);	
	StepPf(x->m_numparticles, x->m_processing, pf_tdoa, z, tot_zs, num_depth, projected_center);
	free(z);
	
	SetTdoa(x, pf_tdoa);
	
	for(i = 0; i < 21; i++) {
	    tdoa_vec[i] = round(pf_tdoa[i]);
	}
	double pan = 0;
	double tilt = 0;
	pcaTreePredict(tdoa_vec, &pan, &tilt);

	gettimeofday(&tim, NULL);
	double t2=tim.tv_sec+(tim.tv_usec/1000000.0) - x->start_time;
	outlet_float(x->m_rtout, (t2-t1)*1000.0);
	
	outlet_float(x->m_panout, pan);
	outlet_float(x->m_tiltout, tilt);

	t_atom myList[36];
	SETFLOAT(myList+0,pan);
	SETFLOAT(myList+1,tilt);
	SETFLOAT(myList+2,t1*1000.0);
	for(i=0; i < 21; i++) {
	    SETFLOAT(myList+3+i,pf_tdoa[i]);
	}
	for(i=1; i < NUM_CHANNELS; i++) {
	    SETFLOAT(myList+24+i-1,rel_energies[i]);
	}
	for(i=0; i < 3; i++) {
	    SETFLOAT(myList+30+i,projected_center[i]);
	    SETFLOAT(myList+33+i,(float) num_depth[i]);
	}
	outlet_anything(x->m_messageout, gensym("audio"), 30, (void *) &myList);
    }
    free(tdoa_vec);
    for(i=0; i < NUM_CHANNELS; i++) {
	free(fftBuffers[i]);
	free(channelBuffers[i]);
    }
    free(fftBuffers);
    free(channelBuffers);
    post("Localization killed!\n");
    pthread_exit(NULL);
}

void *Run_Bform(t_controller * x) {
    post("Beamformer started!\n");
    MOTU * motu = (MOTU*) x->motu;
    while(!x->kill_flag) {
	pthread_mutex_lock(&motu->input_lock);
	int start = motu->beamformPos;
	int end = motu->headInput;
	int size = (motu->inputBufferSize + (end - start)) % motu->inputBufferSize;
	/*Wait until there's enough input to bform*/
	while(size < OUTPUT_BUFFER_SIZE) {
	    pthread_cond_wait(&motu->input_cond, &motu->input_lock);
	    start = motu->beamformPos;
	    end = motu->headInput;
	    size = (motu->inputBufferSize + (end - start)) % motu->inputBufferSize;
	}
	/*Beamform here, and copy result into outputBuffer to be copied to out device*/
	pthread_mutex_lock(&motu->output_lock);
 	pthread_mutex_lock(&x->lock);
	int i;
	for(i=0; i < OUTPUT_BUFFER_SIZE; i++) {
	    motu->outputBuffer[i] =   
	        rel_energies[0]*motu->inputBuffer[1][(start + x->tdoa_array[0] + i + motu->inputBufferSize) % motu->inputBufferSize] +
		rel_energies[1]*motu->inputBuffer[2][(start + x->tdoa_array[1] + i + motu->inputBufferSize) % motu->inputBufferSize] +
		rel_energies[2]*motu->inputBuffer[3][(start + x->tdoa_array[2] + i + motu->inputBufferSize) % motu->inputBufferSize] +
		rel_energies[3]*motu->inputBuffer[4][(start + x->tdoa_array[3] + i + motu->inputBufferSize) % motu->inputBufferSize] +
		rel_energies[4]*motu->inputBuffer[5][(start + x->tdoa_array[4] + i + motu->inputBufferSize) % motu->inputBufferSize] +
		rel_energies[5]*motu->inputBuffer[6][(start + x->tdoa_array[5] + i + motu->inputBufferSize) % motu->inputBufferSize] +
		rel_energies[6]*motu->inputBuffer[7][(start + x->tdoa_array[6] + i + motu->inputBufferSize) % motu->inputBufferSize];	    
	}
 	if(x->audio_record) {
 	    int i,j;
 	    for(i=0; i < OUTPUT_BUFFER_SIZE; i++) {
 		for(j=1; j < 8; j++) {
 		    fwrite(&motu->inputBuffer[j][(start + i) % motu->inputBufferSize], sizeof(float), 1, x->audio_file);
 		}
		fwrite(&motu->outputBuffer[i], sizeof(float), 1, x->audio_file);
 	    }
 	}
 	pthread_mutex_unlock(&x->lock);
	motu->beamformPos += OUTPUT_BUFFER_SIZE;
	motu->beamformPos %= motu->inputBufferSize;
	motu->freshOutput = 1;
	pthread_cond_broadcast(&motu->output_cond);
	pthread_mutex_unlock(&motu->output_lock);
	pthread_mutex_unlock(&motu->input_lock);
    }
    post("Beamformer killed!\n");
    pthread_exit(NULL);
}
