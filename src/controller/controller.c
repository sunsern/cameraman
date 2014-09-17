/* cameracontroller.c -- output the maximum of a group of numbers ------- */

#include "ext.h"
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/time.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <sched.h>
#include <semaphore.h>
#include <pthread.h>
#include <curl/curl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

#define NUM_MICS 7
#define GOOD_FACE_SIZE 200
#define FACE_SIZE_BUF_VAL 50

char * PD_LOG_FILE = "/Users/cameraman/Documents/pdtree_log.txt";

typedef struct _controller
{
    struct object m_ob;
    void * m_messageout;
    void * m_facetrackerout;
    void * m_handout;

    double * audio_pan_buff;
    double * audio_tilt_buff;
    double * audio_time_buff;
    float ** audio_tdoa_buff;
    float ** audio_energy_buff;
    int audio_buff_pointer;
    int audio_buff_size;

    int * face_row_buff;
    int * face_col_buff;
    int * face_size_buff;
    double * face_time_buff;
    int face_buff_pointer;
    int face_buff_size;

    double current_pan;
    double current_tilt;
    int current_face_size;
    double current_zoom;

    int tdoa_array[7];

    int face_mode;
    int face_centered;
    int recording_mode;
    int save_recording;
    int force_recording;
    double time_last_move;
    double time_last_audio_move;
    double time_last_picture_scan;
    int kill_flag;
    char movie_path[128];
    FILE * log_file;
    FILE * pd_log_file;
    int movie_time;
    int has_camera_moved;
    int zoom_mode;
    double zoom_time;
    int audio_switch;

    CURL * curl;
    pthread_mutex_t lock;
    pthread_mutex_t curl_lock;

    int light_mode;  /* 0 = green light detector off, 1 = on */
} t_controller;

void *controller_class;

void controller_assist(t_controller *x, void *b, long m, long a, char *s);
void *controller_new(t_symbol *s, short ac, t_atom *av);
void controller_free(t_controller *x);
void controller_message(t_controller *x, t_symbol *s, short argc, t_atom *argv);

void IncrementalZoom(t_controller * x, int start_stop);
void IncrementalPT(t_controller * x, int ud, int lr, double speed, double col, double row);
void InquirePanTilt(t_controller * x);
void InquireZoom(t_controller * x);
void CallPanTilt(t_controller * x, double pan, double tilt);
void CallZoom(t_controller * x, int z);
void UpdateTimeLastMove(t_controller * x);
void CurlCmd(t_controller * x, char * cmd);
void CheckZoomMode(t_controller *x);

void audio_event_register(t_controller *x, double pan, double tilt, double time, int * tdoa_vec, float * energies, float * pca_coords, int * num_depth);
void picture_control(t_controller * x);
void audio_control(t_controller * x);
void face_event_register(t_controller *x, int row, int col, int size, double time);
void face_control(t_controller * x);
void set_face_mode(t_controller * x);
int hexToTwoCompInt(char * c);
void StartRecording(t_controller * x);
void StopRecording(t_controller * x);
void SaveRecording(t_controller * x); 
void clear_face_buffer(t_controller * x);
void clear_audio_buffer(t_controller * x);

void Start(t_controller * x);
void *Run_Controller(t_controller * x);
void log_it(FILE * file, char * msg, int print_to_screen);

int main() {
    setup((t_messlist **)&controller_class, (method)controller_new, (method)controller_free, (short)sizeof(t_controller), 0L, A_GIMME, 0);
    addmess((method)controller_assist,"assist",A_CANT,0);
    addmess((method)controller_message,"audio",A_GIMME,0);
    addmess((method)controller_message,"face",A_GIMME,0);
    addmess((method)controller_message,"start",A_GIMME,0);
    addmess((method)controller_message,"stop",A_GIMME,0);
    addmess((method)controller_message,"hand_out",A_GIMME,0);
    addmess((method)controller_message,"hand_in",A_GIMME,0);
    addmess((method)controller_message,"verified",A_GIMME,0);
    addmess((method)controller_message,"not_verified",A_GIMME,0);
    addmess((method)controller_message,"stop_recording",A_GIMME,0);
    addmess((method)controller_message,"expcomp",A_GIMME,0);
    addmess((method)controller_message,"face_acquired",A_GIMME,0);
    addmess((method)controller_message,"face_lost",A_GIMME,0);
    addmess((method)controller_message,"light",A_GIMME,0);
    addmess((method)controller_message,"light_start",A_GIMME,0);
    addmess((method)controller_message,"light_stop",A_GIMME,0);
    addmess((method)controller_message,"force_recording_start",A_GIMME,0);
    addmess((method)controller_message,"force_recording_stop",A_GIMME,0);
    addmess((method)controller_message,"audio_switch_start",A_GIMME,0);
    addmess((method)controller_message,"audio_switch_stop",A_GIMME,0);
    return 0;
}

void log_it(FILE * file, char * msg, int print_to_screen) {
    if(print_to_screen) {
	post(msg);
    }
    if(file != NULL) {
	struct timeval tim;
	gettimeofday(&tim,NULL);
	fprintf(file, "%d %s\n", (int) tim.tv_sec, msg);
    }
}

void ExecCmd(char * cmd) {
    pid_t pid = fork();
    if(pid==0) {
	execl("/bin/sh","sh","-c",cmd,NULL);
	exit(127); //Shouldn't get here if exec worked
    }
    signal(SIGCHLD, SIG_IGN); //Ignore the SIGCHILD signal when child exits so that we clean up zombie process
}

void controller_message(t_controller *x, t_symbol *s, short argc, t_atom *argv) {
    if(s == gensym("audio")) {
	double pan = (double) argv[0].a_w.w_float;
	double tilt = (double) argv[1].a_w.w_float;
	char msg[128];
	sprintf(msg, "audio event (pan,tilt): (%2.2f,%2.2f)",pan,tilt);
	log_it(x->log_file, msg,0);
	struct timeval tim;
	gettimeofday(&tim,NULL);
	double time = tim.tv_sec*1000.0 + tim.tv_usec/1000.0;
	int tdoa_vec[21];
	float energy_vec[6];
	float pca_coords[3];
	int num_depth[3];
	int i;
	for(i=0; i < 21; i++) {
	    tdoa_vec[i] = (int) argv[3+i].a_w.w_long;	    
	}
	for(i=0; i < 6; i++) {
	    energy_vec[i] = (float) argv[24+i].a_w.w_float;
	}
	for(i=0; i < 3; i++) {
	    pca_coords[i] = (float) argv[30+i].a_w.w_float;
	    num_depth[i] = (int) ((float) argv[33+i].a_w.w_float);
	}
	audio_event_register(x, pan, tilt, time, tdoa_vec, energy_vec, pca_coords, num_depth);
    }
    else if(s == gensym("face") && argc > 0) {
	int i;
	for(i=0; i < argc; i+=5) {
	    int row = (int) argv[i+0].a_w.w_long;
	    int col = (int) argv[i+1].a_w.w_long;
	    int size = (int) argv[i+2].a_w.w_long;

	    long long time_top =  argv[i+3].a_w.w_long;
	    long long time_bottom =  argv[i+4].a_w.w_long;
	    long long long_time = ((time_top << 32) | (time_bottom & 0xffffffff));
	    double time = (double)long_time;
	    if(x->light_mode == 0) {  
		face_event_register(x, row, col, size, time);
	    }
	    char msg[128];
	    sprintf(msg, "face event (x,y,size): (%d,%d,%d)",row,col,size);
	    log_it(x->log_file, msg,0);
	}    
    }
    else if(s == gensym("start")) {
	x->kill_flag = 0;
	Start(x);
    }
    else if(s == gensym("stop")) {
	x->kill_flag = 1;	
 	char cmd[128]; 
 	sprintf(cmd, "/usr/bin/osascript /Users/cameraman/cameraman/controller/quit_quicktime.scpt"); 
 	ExecCmd(cmd); 
    }
    else if(s == gensym("stop_recording")) {
	StopRecording(x);
    }
    else if(s == gensym("verified")) {
	StartRecording(x);
	outlet_anything(x->m_handout,gensym("start_recording"), 0,NULL);
    }
    else if(s == gensym("force_recording_start")) {
	x->force_recording = 1;
	StartRecording(x);
	outlet_anything(x->m_handout,gensym("start_recording"), 0,NULL);
    }
    else if(s == gensym("force_recording_stop")) {
	x->force_recording = 0;
	StopRecording(x);
    }
    else if(s == gensym("audio_switch_start")) {
	x->audio_switch = 1;
    }
    else if(s == gensym("audio_switch_stop")) {
	x->audio_switch = 0;
    }
    else if(s == gensym("not_verified")) {
	pthread_mutex_lock(&x->lock);
	x->face_mode = 0;
	x->current_pan = 0; 
	x->current_tilt = -20;
	x->current_zoom = 0;
	if(x->light_mode == 2) {
	    x->light_mode = 1;
	}
	pthread_mutex_unlock(&x->lock);
	log_it(x->log_file, "Controller(not_verified) - User doesn't want to record, going to rest.",1);
 	outlet_anything(x->m_facetrackerout,gensym("face_stop"), 0,NULL); 
	CallPanTilt(x, x->current_pan,x->current_tilt);
	CallZoom(x,0);
    }
    else if(s == gensym("expcomp") && argc > 0) {
	char cmd[256];
	int exp = (int) argv[0].a_w.w_long;
	sprintf(cmd,"http://192.168.1.11/command/visca-gen.cgi?visca=8101044E0000000%XFF",exp);
	char msg[256];
	sprintf(msg, "Controller(expcomp) - Sending Exposure Comp = %d",exp);
	log_it(x->log_file, msg,1);
	CurlCmd(x,cmd);
    }
    else if(s == gensym("face_acquired")) {
	pthread_mutex_lock(&x->lock);
	x->face_mode = 1;		
	pthread_mutex_unlock(&x->lock); 	
    }
    else if(s == gensym("face_lost")) {
	pthread_mutex_lock(&x->lock);
	x->face_mode = 0;		
	pthread_mutex_unlock(&x->lock); 	
    }
    else if(s == gensym("light_start")) {
	pthread_mutex_lock(&x->lock);
	x->light_mode = 1;		
	pthread_mutex_unlock(&x->lock); 	
	log_it(x->log_file, "Light mode started!",1);
    }
    else if(s == gensym("light_stop")) {
	pthread_mutex_lock(&x->lock);
	x->light_mode = 0;		
	pthread_mutex_unlock(&x->lock); 	
	log_it(x->log_file, "Light mode stopped!",1);
    }
    else if(s == gensym("light")) {
	int i;
	for(i=0; i < argc; i+=5) {
	    int row = (int) argv[i+0].a_w.w_long;
	    int col = (int) argv[i+1].a_w.w_long;
	    int size = 1; /* (int) (15.0*argv[i+2].a_w.w_long); */
	    if(row != -1 && col != -1) {
		struct timeval tim;
		gettimeofday(&tim, NULL);
		double time = tim.tv_sec*1000.0 + tim.tv_usec/1000.0; 
		
		if(x->light_mode != 0) {
		    face_event_register(x, row, col, size, time);
		}
		char msg[128];
		if(tim.tv_sec % 10 == 0) {
		    sprintf(msg, "light event (x,y,size): (%d,%d,%d)",row,col,size);
		    log_it(x->log_file, msg,1);
		}
	    }
	}    
    }
}

void StartRecording(t_controller * x) {
    if(x->recording_mode)
	return;
    pthread_mutex_lock(&x->lock);
    x->recording_mode = 1;
    pthread_mutex_unlock(&x->lock);
    char cmd[128];
    struct timeval tim;
    gettimeofday(&tim, NULL);
    x->movie_time = (int) tim.tv_sec;
    char filename[128];
    sprintf(filename,"/Users/cameraman/Documents/Quicktime_Recordings/%d.log",x->movie_time);
    x->log_file = fopen(filename,"w");
    sprintf(cmd, "/usr/bin/osascript /Users/cameraman/cameraman/controller/start_recording.scpt");
    ExecCmd(cmd);
    outlet_anything(x->m_messageout,gensym("start_recording"), 0,NULL);
    log_it(x->log_file, "Controller(StartRecording): Starting a recording!",1);
}

void StopRecording(t_controller * x) {
    if(x->log_file == NULL || x->force_recording) 
	return;
    pthread_mutex_lock(&x->lock);
    x->recording_mode = 0;
    pthread_mutex_unlock(&x->lock);
    log_it(x->log_file, "Controller(StopRecording): Stopping a recording!",1);
    outlet_anything(x->m_handout,gensym("stop_recording"), 0,NULL);
    outlet_anything(x->m_messageout,gensym("stop_recording"), 0,NULL);
    fclose(x->log_file);
    x->log_file = NULL;
    /*Find and rename or delete the movie that was saved by iVeZeen*/
    DIR *pDIR;
    struct dirent *pDirEnt;
    int found = 0;
    char dirpath[128];
    sprintf(dirpath,"/Users/cameraman/Documents/Quicktime_Recordings/");
    
    pDIR = opendir(dirpath);
    if(pDIR == NULL) {
	log_it(x->log_file,"Controller: can't find iVeZeen recording directory in StopRecording()!",1);
    }
    int max_ind = 0;
    pDirEnt = readdir(pDIR);
    while(pDirEnt != NULL) {
	if(strncmp("Movie",pDirEnt->d_name,5) == 0) {
	    found = 1;
	    int temp_int = 0;
	    if(strcmp("Movie.mov",pDirEnt->d_name) != 0) {
		sscanf(pDirEnt->d_name, "Movie %d.mov",&temp_int);
	    }
	    if(temp_int > max_ind) {
		max_ind = temp_int;
	    }
	}
	pDirEnt = readdir(pDIR);
    }
    closedir(pDIR);
    if(found) {
	char cmd[2048];
	if(max_ind == 0) {
	    sprintf(cmd, "/usr/bin/osascript /Users/cameraman/cameraman/controller/stop_recording.scpt; mv /Users/cameraman/Documents/Quicktime_Recordings/Movie.mov /Users/cameraman/Documents/Quicktime_Recordings/%d.mov ; /usr/bin/scp /Users/cameraman/Documents/Quicktime_Recordings/%d.log cameraman@seed.ucsd.edu:/mnt/freundlabdisk2/disk2/video_recordings/ > /dev/null; /usr/bin/scp /Users/cameraman/Documents/Quicktime_Recordings/%d.mov cameraman@seed.ucsd.edu:/mnt/freundlabdisk2/disk2/video_recordings/ > /dev/null; rm -f /Users/cameraman/Documents/Quicktime_Recordings/* > /dev/null;", x->movie_time,x->movie_time,x->movie_time);
	}
	else {
	    sprintf(cmd, "/usr/bin/osascript /Users/cameraman/cameraman/controller/stop_recording.scpt; mv /Users/cameraman/Documents/Quicktime_Recordings/Movie %d.mov /Users/cameraman/Documents/Quicktime_Recordings/%d.mov ; /usr/bin/scp /Users/cameraman/Documents/Quicktime_Recordings/%d.log cameraman@seed.ucsd.edu:/mnt/freundlabdisk2/disk2/video_recordings/ > /dev/null; /usr/bin/scp /Users/cameraman/Documents/Quicktime_Recordings/%d.mov cameraman@seed.ucsd.edu:/mnt/freundlabdisk2/disk2/video_recordings/ > /dev/null; rm -f /Users/cameraman/Documents/Quicktime_Recordings/* > /dev/null;", max_ind, x->movie_time,x->movie_time,x->movie_time);
	}
	ExecCmd(cmd);
    }
    outlet_anything(x->m_handout,gensym("verify"), 0,NULL);
}

void SaveRecording(t_controller * x) { 
     if(!x->save_recording) { 
 	log_it(x->log_file, "Controller(SaveRecording): Saving this recording!",1); 
	pthread_mutex_lock(&x->lock);
	x->save_recording = 1;
	pthread_mutex_unlock(&x->lock);
	outlet_anything(x->m_messageout,gensym("save_recording"), 0,NULL);
    }
}

void CurlCmd(t_controller * x, char * cmd) {
    CURL * curl;
    CURLcode res;
    char msg[512];
    char ErrBuf[CURL_ERROR_SIZE] = {'\0'};
    sprintf(msg, "Controller(CurlCmd): Error in sending the curl cmd! -- %s", cmd);
    
    pthread_mutex_lock(&x->curl_lock);
    curl = x->curl;
    if(curl) {
	curl_easy_reset(curl);
	curl_easy_setopt(curl, CURLOPT_URL, cmd);
	curl_easy_setopt(curl, CURLOPT_USERPWD, "cameraman:cameraman");
	curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, ErrBuf);
	res = curl_easy_perform(curl);
	if(res != 0) {
	    log_it(x->log_file, msg,1);
	    log_it(x->log_file, ErrBuf, 1);
	}
    }
    else {
	log_it(x->log_file, "Controller(CurlCmd): Error sending command to camera!",1);
    }
    pthread_mutex_unlock(&x->curl_lock);
}

void CallZoom(t_controller * x, int z) {
    char cmd[256];
    switch(z) {
    case 0:
	sprintf(cmd, "http://192.168.1.11/command/ptzf.cgi?VISCA=8101044700080000FF");
	break;
    case 1:
	sprintf(cmd, "http://192.168.1.11/command/ptzf.cgi?VISCA=8101044701000000FF");
	break;
    case 2:
	sprintf(cmd, "http://192.168.1.11/command/ptzf.cgi?VISCA=8101044701080000FF");
	break;
    case 3:
	sprintf(cmd, "http://192.168.1.11/command/ptzf.cgi?VISCA=8101044702000000FF");
	break;
    default:
	break;
    }
    CurlCmd(x, cmd);
    InquireZoom(x);
    UpdateTimeLastMove(x);
}

void IncrementalZoom(t_controller * x, int zoom_cmd) {
    char cmd[256];
    x->has_camera_moved = 1;
    switch(zoom_cmd) {
    case -1:/*Start zooming out*/
	sprintf(cmd, "http://192.168.1.11/command/ptzf.cgi?relative=1003");
	CurlCmd(x,cmd);
	UpdateTimeLastMove(x);
	break;
    case 1:/*Start zooming in*/
	sprintf(cmd, "http://192.168.1.11/command/ptzf.cgi?relative=1103");
	CurlCmd(x,cmd);
	UpdateTimeLastMove(x);
	break;
    default:
	break;
    }
}

void CheckZoomMode(t_controller *x) {
    pthread_mutex_lock(&x->lock);
    if(abs(x->current_pan) <= 15 &&
       abs(x->current_tilt + 10) <= 10)
    {
	if(x->zoom_mode == 0) {
	    struct timeval tim;
	    gettimeofday(&tim,NULL);
	    x->zoom_time = tim.tv_sec*1000.0 + tim.tv_usec/1000.0; 
	    x->zoom_mode = 1;
	}
    }
    else {
	x->zoom_mode = 0;
	x->zoom_time = 0.0;
    }
    pthread_mutex_unlock(&x->lock);
    if(x->zoom_mode == 0) {
	CallZoom(x,0);
    }
}

void IncrementalPT(t_controller * x, int ud, int lr, double speed,double col,double row) {
    char cmd[256];
    double col_center = 320.0;
    double row_center = 240.0;
    speed /= 800.0; /*800.0 is the diagonal length of the 640x480 image, so a 10% displacement corresponds to an ~80 pixel displacement*/
    if(speed <= 0.025) {
	return;
    }

    double len = sqrt((col - col_center)*(col - col_center) + (row  - row_center)*(row - row_center));
    double scale = 800;
    if(len > scale) {
	col = scale*(col - col_center)/len + col_center;
	row = scale*(row - row_center)/len + row_center; 
    }
    sprintf(cmd,"http://192.168.1.11/command/ptzf.cgi?directPT=%d,%d",(int) (col), (int) (row));
    CurlCmd(x,cmd);
    UpdateTimeLastMove(x);
    t_atom modeNum[2];
    modeNum[0].a_type = A_LONG;
    modeNum[0].a_w.w_long = (int) (col - col_center);
    modeNum[1].a_type = A_LONG;
    modeNum[1].a_w.w_long = (int) (row - row_center);
    outlet_anything(x->m_facetrackerout,gensym("camera_move"), 2,(t_atom *) &modeNum);
    return;
}

void CallPanTilt(t_controller * x, double pan, double tilt) {
    int d_pan = (int) pan/170.0*2448.0;
    int d_tilt = (int) -828.0 + ((25-tilt)/(90.0 - -25.0)*(972.0- -828.0));
    char cmd[256];
    char panStr[256];
    char tiltStr[256];
    sprintf(panStr,"%02X%02X%02X%02X", (d_pan >> 12) & 0x000F, (d_pan >> 8) & 0x000F, (d_pan >> 4) & 0x000F, (d_pan >> 0) & 0x000F);
    sprintf(tiltStr,"%02X%02X%02X%02X", (d_tilt >> 12) & 0x000F, (d_tilt >> 8) & 0x000F, (d_tilt >> 4) & 0x000F, (d_tilt >> 0) & 0x000F);
    sprintf(cmd, "http://192.168.1.11/command/ptzf.cgi?VISCA=810106021814%s%sFF",panStr,tiltStr);
    CurlCmd(x,cmd);
    InquirePanTilt(x);
    UpdateTimeLastMove(x);
}

void UpdateTimeLastMove(t_controller * x) {
    struct timeval tim;
    gettimeofday(&tim,NULL);
    double time = tim.tv_sec*1000.0 + tim.tv_usec/1000.0;
    pthread_mutex_lock(&x->lock);
    x->time_last_move = time;
    x->has_camera_moved = 1;
    pthread_mutex_unlock(&x->lock);		
}

int hexToTwoCompInt(char * c) {
    int val = 0;

    int i;
    for(i=0; i < 4; i++) {
	if(c[i] >= '0' && c[i] <= '9') {
	    val = (val << 4) | (c[i] - '0');
	}
	else if (c[i] >= 'a' && c[i] <= 'f') {
	    val = (val << 4) | (c[i] - 'a' + 10);
	}
    }
    if(val & 0x00008000) {
	val |= 0xFFFF0000;
    }
    return val;
}

void InquireZoom(t_controller * x) {
    CURL * curl;
    CURLcode res;
    FILE * file;
    log_it(x->log_file, "Controller(InquireZoom)",0); 
    pthread_mutex_lock(&x->curl_lock);
    curl = x->curl;
    if(curl) {
	curl_easy_reset(curl);
	file = fopen("/Users/cameraman/cameraman/currentPT.curl","w+");
	if(file) {
	    curl_easy_setopt(curl, CURLOPT_URL, "http://192.168.1.11/command/visca-inquiry.cgi?VISCA=81090447FF");
	    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
	    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,NULL);
	    curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
	    res = curl_easy_perform(curl);
	    rewind(file);
	    char htmlres[256];
	    fread(&htmlres, sizeof(char), 14, file);
	    fclose(file);
	    char zoom[4]; 
	    zoom[0] = htmlres[5]; zoom[1] = htmlres[7]; zoom[2] = htmlres[9]; zoom[3] = htmlres[11];
	    int zoom_i = hexToTwoCompInt(zoom);
	    double zoom_d = (double) zoom_i/(double) hexToTwoCompInt("3000");
 	    pthread_mutex_lock(&x->lock);
 	    x->current_zoom = zoom_d; 
 	    pthread_mutex_unlock(&x->lock); 
	}
    }
    else {
	log_it(x->log_file, "Controller(InquireZoom): Error inquiring zoom!",1);
    }
    x->has_camera_moved = 0;    
    pthread_mutex_unlock(&x->curl_lock);
}

void InquirePanTilt(t_controller * x) {
    CURL * curl;
    CURLcode res;
    FILE * file;

    log_it(x->log_file, "Controller(InquirePanTilt)",0); 
    pthread_mutex_lock(&x->curl_lock);
    curl = x->curl;
    if(curl) {
	curl_easy_reset(curl);
	file = fopen("/Users/cameraman/cameraman/currentPT.curl","w+");
	if(file) {
	    curl_easy_setopt(curl, CURLOPT_URL, "http://192.168.1.11/command/visca-inquiry.cgi?VISCA=81090612FF");
	    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
	    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,NULL);
	    curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
	    res = curl_easy_perform(curl);
	    rewind(file);
	    char htmlres[256];
	    fread(&htmlres, sizeof(char), 22, file);
	    fclose(file);
	    char pan[4]; char tilt[4];
	    pan[0] = htmlres[5]; pan[1] = htmlres[7]; pan[2] = htmlres[9]; pan[3] = htmlres[11];
	    tilt[0] = htmlres[13]; tilt[1] = htmlres[15]; tilt[2] = htmlres[17]; tilt[3] = htmlres[19];
	    int  pan_i = hexToTwoCompInt(pan);
	    int tilt_i = hexToTwoCompInt(tilt);
	    double pan_d = (double) pan_i/2448.0*170.0;
	    double tilt_d = ((double) tilt_i + 828.0)/(972.0+828.0)*(90.0+25.0)*-1.0 + 25.0;
 	    pthread_mutex_lock(&x->lock);
 	    x->current_pan = pan_d; 
 	    x->current_tilt = tilt_d; 
 	    pthread_mutex_unlock(&x->lock); 
	}
    }
    else {
	log_it(x->log_file, "Controller(InquirePanTilt): Error inquiring pan and tilt!",1);
    }
    pthread_mutex_unlock(&x->curl_lock);
    x->has_camera_moved = 0;
}

int compare_double(const void * a, const void * b) {
    double * d_a = (double *) a;
    double * d_b = (double *) b;
    if(*d_a < *d_b) return -1;
    return *d_a == *d_b ? 0 : 1;
}

int compare_int(const void * a, const void * b) {
    int * d_a = (int *) a;
    int * d_b = (int *) b;
    if(*d_a < *d_b) return -1;
    return *d_a == *d_b ? 0 : 1;
}

void audio_event_register(t_controller *x, double pan, double tilt, double time, int * tdoa_vec,
			  float * energies, float * pca_coords, int * num_depth) {
    pthread_mutex_lock(&x->lock);
    int index = x->audio_buff_pointer;
    x->audio_pan_buff[index] = pan;
    x->audio_tilt_buff[index] = tilt;
    x->audio_time_buff[index] = time;
    memcpy(&(x->audio_tdoa_buff[index][0]),tdoa_vec, 21*sizeof(float)); 
    memcpy(&(x->audio_energy_buff[index][0]),energies, 6*sizeof(float));
    x->audio_buff_pointer = (x->audio_buff_pointer > x->audio_buff_size - 2) ? 0 : (x->audio_buff_pointer + 1);
    pthread_mutex_unlock(&x->lock);
    char msg[256];
    sprintf(msg, "###PD-Tree: %f, %f, %f, %d, %d, %d", pca_coords[0], pca_coords[1], pca_coords[2],
	    num_depth[0], num_depth[1], num_depth[2]);
    x->pd_log_file = fopen(PD_LOG_FILE, "a");
    log_it(x->pd_log_file, msg, 0);
    fclose(x->pd_log_file);
    log_it(x->log_file, msg, 0);
}


void face_event_register(t_controller *x, int row, int col, int size, double time) {
    pthread_mutex_lock(&x->lock);
    int index = x->face_buff_pointer;
    x->face_row_buff[index] = row;
    x->face_col_buff[index] = col;
    x->face_size_buff[index] = size;
    x->face_time_buff[index] = time;
    x->face_buff_pointer = x->face_buff_pointer > x->face_buff_size - 2 ? 0 : x->face_buff_pointer + 1;
    pthread_mutex_unlock(&x->lock);
}

void picture_control(t_controller * x) {
    struct timeval tim;
    gettimeofday(&tim,NULL);
    double time = tim.tv_sec*1000.0 + tim.tv_usec/1000.0;
    double pan[9] = {-90.0, -67.5, -45.0, -22.5, 0.0, 22.5, 45.0, 67.5, 90.0};
    double tilt[2] = {-20.0,0.0};
    char cmd[2048];
    char name[128];
    int i,j;
    if(time - x->time_last_picture_scan > 10*60*1000.0 && !x->recording_mode) {
	CallZoom(x,0);
	sprintf(cmd, "/usr/bin/curl http://cameraman:cameraman@192.168.1.11/command/visca-gen.cgi?visca=8101044E000000007FF");
	system(cmd);
	/* Go to different pan/tilt combos and take pictures to store on seed.ucsd.edu */
	/* P ={-90.0, -67.5, -45.0, -22.5, 0.0, 22.5, 45.0, 67.5, 90.0} and T ={-10,30}*/
	for(j = 0; j < 2; j++) {
	    for(i = 0; i < 9; i++) {
		CallPanTilt(x,pan[i],tilt[j]);	
		sprintf(name, "%d_%d_%d.jpg", (int) tim.tv_sec, (int) pan[i], (int) tilt[j]);
		sprintf(cmd, "/usr/bin/curl http://192.168.1.11/oneshotimage.jpg > /Users/cameraman/Documents/Picture_Scans/%s; /usr/bin/scp /Users/cameraman/Documents/Picture_Scans/%s cameraman@seed.ucsd.edu:/mnt/usbdrive1/picture_scans/ > /dev/null; rm -f /Users/cameraman/Documents/Picture_Scans/* > /dev/null;", name, name);
		system(cmd);
	    }
	}
	/* Also do 4 random pictures in pan \in [-90,90] & tilt \in [-20,0]*/
	for(i = 0; i < 4; i++) {
	    int p,t;
	    p = (rand() % 181) - 90;
	    t = (rand() % 21) - 20;
	    CallPanTilt(x,p,t);	
	    sprintf(name, "%d_%d_%d.jpg", (int) tim.tv_sec, p, t);
	    sprintf(cmd, "/usr/bin/curl http://192.168.1.11/oneshotimage.jpg > /Users/cameraman/Documents/Picture_Scans/%s; /usr/bin/scp /Users/cameraman/Documents/Picture_Scans/%s cameraman@seed.ucsd.edu:/mnt/usbdrive1/picture_scans/ > /dev/null; rm -f /Users/cameraman/Documents/Picture_Scans/* > /dev/null;", name, name);
	    system(cmd);
	}
	CallPanTilt(x,0,-20);
	sprintf(cmd, "/usr/bin/curl http://cameraman:cameraman@192.168.1.11/command/visca-gen.cgi?visca=8101044E000000007FF");
	system(cmd);
	gettimeofday(&tim,NULL);
	x->time_last_picture_scan = tim.tv_sec*1000.0 + tim.tv_usec/1000.0;
    }
}

void audio_control(t_controller * x) {
    double pan_temp[20];
    double tilt_temp[20];
    float tdoa_temp[20][21];
    float energy_temp[20][6];
    int size = 0;
    pthread_mutex_lock(&x->lock);
    int index = x->audio_buff_pointer == 0 ? (x->audio_buff_size - 1) : (x->audio_buff_pointer - 1);

    struct timeval tim;
    gettimeofday(&tim,NULL);
    double time = tim.tv_sec*1000.0 + tim.tv_usec/1000.0;
/* EVAN: This doesn't work to avoid rapid audio movements....it waits 5 seconds then moves to a random audio source, and you can't get it back*/
/*     if(time > x->time_last_audio_move && time - x->time_last_audio_move < 5000.0) { */
/* 	pthread_mutex_unlock(&x->lock); */
/* 	clear_audio_buffer(x); */
/* 	return; */
/*     } */

    while(time - x->audio_time_buff[index] < 350 && size < 20) {
	pan_temp[size] = x->audio_pan_buff[index];
	tilt_temp[size] = x->audio_tilt_buff[index];
	memcpy(&(tdoa_temp[size][0]),&(x->audio_tdoa_buff[index][0]),sizeof(float)*21);
	memcpy(&(energy_temp[size][0]),&(x->audio_energy_buff[index][0]),sizeof(float)*6);
	size++;
	index = index == 0 ? (x->audio_buff_size - 1) : (index - 1); 
    }
    pthread_mutex_unlock(&x->lock);
    if(size >= 5) {
	/* Calculate Medians */
	double sort_pan[20];
	double sort_tilt[20];
	memcpy(sort_pan, pan_temp, size*sizeof(double));
	memcpy(sort_tilt, tilt_temp, size*sizeof(double));
	qsort(sort_pan, size, sizeof(double), compare_double);
	qsort(sort_tilt, size, sizeof(double), compare_double);
	double pan_median = size % 2 == 1 ? sort_pan[(int) (1.0*(double) size/2.0)] : (sort_pan[(int) (1.0*(double) size/2.0)] + sort_pan[((int) (1.0*(double) size/2.0)) - 1])/2.0;
	double tilt_median = size % 2 == 1 ? sort_tilt[(int) (1.0*(double) size/2.0)] : (sort_tilt[(int) (1.0*(double) size/2.0)] + sort_tilt[((int) (1.0*(double) size/2.0)) - 1])/2.0;
	/*How many are close to the median */
	double pan_send = 0;
	double tilt_send = 0;
	float tdoa_send[21] = {0,0,0,0,0,0,0,
			     0,0,0,0,0,0,0,
			     0,0,0,0,0,0,0};
	float energy_send[6] = {0,0,0,0,0,0};
	int num = 0;
	int i,j;
	for(i=0; i < size; i++) {
	    if(abs(pan_temp[i] - pan_median) < 1.0 &&
	       abs(tilt_temp[i] - tilt_median) < 1.0)
	    {
		pan_send += pan_temp[i];
		tilt_send += tilt_temp[i];
		for(j=0; j < 21; j++) {
		    tdoa_send[j] += tdoa_temp[i][j];
		}
		for(j=0; j < 6; j++) { 
		    energy_send[j] += energy_temp[i][j];
		}
		num++;
	    }
	}
	if(num >= 10) {
	    pan_send = pan_send/(double) num;
	    tilt_send = tilt_send/(double) num;
	    double ch_pan = abs(x->current_pan - pan_send);
	    double ch_tilt = abs(x->current_tilt - tilt_send); 

	    /*If we want to direct camera sufficiently far from where it is, do it*/
	    /*Remove the !x->face_mode if you don't want the camera to LOCK onto a face*/
	    if((x->audio_switch || (!x->audio_switch && !x->face_mode)) &&
	       (ch_pan > 10.0 || ch_tilt > 10.0)) {
		log_it(x->log_file, "Controller(audio_control) - audio heard, moving to it",1); 
 		outlet_anything(x->m_facetrackerout,gensym("face_start"), 0,NULL);
		if(ch_pan > 10.0 || ch_tilt > 10.0) {
		    CallZoom(x,0);
		}
		if(((x->light_mode && !x->face_mode) || (!x->light_mode))) {
		    pthread_mutex_lock(&x->lock);
		    if(x->light_mode == 1) {
			x->light_mode = 2;
		    }
		    x->time_last_audio_move = time;
		    pthread_mutex_unlock(&x->lock);
		    CallPanTilt(x,pan_send,tilt_send);
		}
		pthread_mutex_lock(&x->lock);
		if(!x->face_mode && !x->recording_mode) {
		    /*Verify The User Wants To Record By Putting Hand in Box*/
		    outlet_anything(x->m_handout,gensym("verify"), 0,NULL);
		}
		x->face_mode = 1;		
		pthread_mutex_unlock(&x->lock); 	
		clear_audio_buffer(x);
	    }
	    /*Else we're close to the person being centered if the face is large*/
	    else if (x->face_centered > 0){ 
		t_atom modeNum[2];
		modeNum[0].a_type = A_LONG;
		modeNum[0].a_w.w_long = (int) (320.0 + 250.0*(1.0+x->current_zoom)*tan((x->current_pan - pan_send)/180*M_PI));
		modeNum[1].a_type = A_LONG;
		modeNum[1].a_w.w_long = (int) (240.0 + 250.0*(1.0+x->current_zoom)*tan((x->current_tilt - tilt_send)/180*M_PI));
		outlet_anything(x->m_facetrackerout,gensym("audio_draw"), 2,(t_atom *) &modeNum);
		int i;
		for(i=0; i < 21; i++) {
		    tdoa_send[i] /= num;
		}
		for(i=0; i < 6; i++) {
		    energy_send[i] /= (float) num;
		}
		/*Write the TDOA to the log*/
		char msg[256];
		sprintf(msg, "*stable audio detection*: %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f",
			x->current_pan, x->current_tilt, x->current_zoom, tdoa_send[0], tdoa_send[1], tdoa_send[2], tdoa_send[3], 
			tdoa_send[4], tdoa_send[5], tdoa_send[6], tdoa_send[7], tdoa_send[8], tdoa_send[9], tdoa_send[10], tdoa_send[11], 
			tdoa_send[12], tdoa_send[13], tdoa_send[14], tdoa_send[15], tdoa_send[16], tdoa_send[17], tdoa_send[18], 
			tdoa_send[19], tdoa_send[20], energy_send[0], energy_send[1], energy_send[2], energy_send[3], energy_send[4], energy_send[5]);
		log_it(x->log_file, msg,1); 
		clear_audio_buffer(x);
	    }
	}
    } 
}

void face_control(t_controller * x) {
    int row_temp[64];
    int col_temp[64];
    int size_temp[64];
    int size = 0;
    int face_center_FLAG = x->face_centered - 1 >= 0 ? x->face_centered - 1 : 0;
    pthread_mutex_lock(&x->lock);
    int index = x->face_buff_pointer == 0 ? x->face_buff_size - 1 : x->face_buff_pointer - 1;
    struct timeval tim;
    gettimeofday(&tim,NULL);
    double time = tim.tv_sec*1000.0 + tim.tv_usec/1000.0;

    while(time - x->face_time_buff[index] < 1000 && x->face_time_buff[index] > x->time_last_move && size < 64) {
	row_temp[size] = x->face_row_buff[index];
	col_temp[size] = x->face_col_buff[index];
	size_temp[size] = x->face_size_buff[index];
	size++;
	index = index == 0 ? x->face_buff_size - 1 : index - 1; 
    }
    pthread_mutex_unlock(&x->lock);
    if(size >= 2) {
	/* Calculate Medians */
	int sort_row[64];
	int sort_col[64];
	int sort_size[64];
	memcpy(sort_row, row_temp, size*sizeof(int));
	memcpy(sort_col, col_temp, size*sizeof(int));
	memcpy(sort_size, size_temp, size*sizeof(int));
	qsort(sort_row, size, sizeof(int), compare_int);
	qsort(sort_col, size, sizeof(int), compare_int);
	qsort(sort_size, size, sizeof(int), compare_int);
	double row_median = size % 2 == 1 ? sort_row[(int) (1.0*(double) size/2.0)] : ((double) (sort_row[(int) (1.0*(double) size/2.0)] + sort_row[((int) (1.0*(double) size/2.0)) - 1]))/2.0;
	double col_median = size % 2 == 1 ? sort_col[(int) (1.0*(double) size/2.0)] : ((double) (sort_col[(int) (1.0*(double) size/2.0)] + sort_col[((int) (1.0*(double) size/2.0)) - 1]))/2.0;
	double size_median = size % 2 == 1 ? sort_size[(int) (1.0*(double) size/2.0)] : ((double) (sort_size[(int) (1.0*(double) size/2.0)] + sort_size[((int) (1.0*(double) size/2.0)) - 1]))/2.0;
	/*How many are close to the median */
	double row_send = 0;
	double col_send = 0;
	double size_send = 0;
	int num = 0;
	int i;
	for(i=0; i < size; i++) {
	    if(abs(row_temp[i] - row_median) < 5.0 &&
	       abs(col_temp[i] - col_median) < 5.0 &&
	       abs(size_temp[i] - size_median) < 5.0)
	    {
		row_send += row_temp[i];
		col_send += col_temp[i];
		size_send += size_temp[i];
		num++;
	    }
	}
	if(num >= 1) {
	    face_center_FLAG = 1000;
	    size_send /= (double) num;
	    if(x->light_mode) {
		struct timeval tim;
		gettimeofday(&tim,NULL);
		double time = tim.tv_sec*1000.0 + tim.tv_usec/1000.0;
		int send_x = (int) ((double) row_send/ (double) num) + size_send/2.0;
		int send_y = (int) ((double) col_send/ (double) num) + size_send/2.0;
		char msg[256];
		sprintf(msg, "%f - Light Found at (%d, %d), current PTZ = (%f, %f, %f)",
			time, send_x, send_y, x->current_pan, x->current_tilt, x->current_zoom);
		log_it(x->log_file, msg,1);
	    }
	    x->current_face_size = (int) size_send;
	    if(size_send > GOOD_FACE_SIZE + FACE_SIZE_BUF_VAL && x->current_zoom > 0) {
/* 		log_it(x->log_file, "Controller(face_control) - Face is too big, trying to zoom out!",0); */
 		IncrementalZoom(x,-1); 
	    }

	    double buff_factor = 1.5;
	    if(x->zoom_mode == 1 && time - x->zoom_time > 5000 && size_send < GOOD_FACE_SIZE - FACE_SIZE_BUF_VAL && x->current_zoom <= .4) { /*Center it and try to zoom in!*/
		double offset = .25*size_send;
		int send_x = (int) ((double) row_send/ (double) num) + size_send/2.0;
		int send_y = (int) ((double) col_send/ (double) num) + size_send/2.0;
		double speed = sqrt((send_x - 320)*(send_x - 320) + (send_y - 240)*(send_y - 240));
/* 		log_it(x->log_file, "Controller(face_control) - Centering a small face",0); */
		if(send_x <= 320 - buff_factor*offset) {
		    IncrementalPT(x, -1, 1,speed, send_x, send_y);
		}
		else if(send_x > 320 - buff_factor*offset && send_x < 320 + buff_factor*offset) {
		    if(send_y > 240 - buff_factor*offset && send_y < 240 + buff_factor*offset) {
/*  			log_it(x->log_file, "Controller(face_control) - User is near couch and face is centered, but small, zooming in",0);  */
			if(!x->light_mode) {
			    IncrementalZoom(x,1);
			}
		    }
		    else {
			IncrementalPT(x, 1, 0,speed, send_x, send_y);
		    }		    
		}
		else {
		    IncrementalPT(x, 1, -1,speed, send_x, send_y);		    
		}
 	    } 
	    else {
		double offset = .25*size_send;
		int send_x = (int) ((double) row_send/ (double) num) + size_send/2.0;
		int send_y = (int) ((double) col_send/ (double) num);
		double speed = sqrt((send_x - 320)*(send_x - 320) + (send_y - 3*offset)*(send_y - 3*offset));
/* 		log_it(x->log_file, "Controller(face_control) - Centering large face",0); */
		if(send_x <= 320 - buff_factor*offset) {
		    IncrementalPT(x, -1, 1,speed, send_x, send_y + size_send);
		}
		else if(send_x > 320 - buff_factor*offset && send_x < 320 + buff_factor*offset) {
		    if(send_y > offset && send_y < 4*offset) { /*Large and in the right place!*/		      
/*  			log_it(x->log_file, "Controller(face_control) - Face is centered and a good size!",0);  */
			SaveRecording(x);
		    }
		    else {
			IncrementalPT(x, 1, 0,speed, send_x, send_y + size_send);
		    }
		}
		else {
		    IncrementalPT(x, -1, -1,speed, send_x, send_y + size_send);
		}
	    }
	}
	else { /*We didn't find a face!*/
	    IncrementalZoom(x,0);
	    x->current_face_size = 0;
	}
	if(x->has_camera_moved) {
	    InquirePanTilt(x);
	    InquireZoom(x);
	}
	CheckZoomMode(x);
    }
    x->face_centered = face_center_FLAG;
}

void set_face_mode(t_controller * x) {
    pthread_mutex_lock(&x->lock);
    int lock_flag = 1;
    if(x->face_mode) {
	struct timeval tim;
	gettimeofday(&tim,NULL);
	double time = tim.tv_sec*1000.0 + tim.tv_usec/1000.0;
	if(time - x->time_last_move > 5000) {
	    int index = x->face_buff_pointer == 0 ? x->face_buff_size - 1 : x->face_buff_pointer - 1;
	    int size = 0;
	    while(time - x->face_time_buff[index] < 5000 && size < x->face_buff_size) {
		size++;
		index = index == 0 ? x->face_buff_size - 1 : index - 1; 
	    }
	    if(size < 2) {
		if(x->light_mode == 2) {
		    x->light_mode = 1;
		}
		x->face_mode = 0;
		x->face_centered = 0;
		if(x->current_pan !=0 || x->current_tilt != -20) {
		    x->current_pan = 0; 
		    x->current_tilt = -20;
		    x->current_zoom = 0;
		    pthread_mutex_unlock(&x->lock);
		    lock_flag = 0;
		    CallPanTilt(x, x->current_pan,x->current_tilt);
		    CallZoom(x,0);
		}		 
		log_it(x->log_file, "Controller(set_face_mode) - No face, going back to wide view",1);
		outlet_anything(x->m_handout,gensym("user_lost"), 0,NULL);		    
		outlet_anything(x->m_facetrackerout,gensym("face_stop"), 0,NULL); 
		StopRecording(x); 
	    }
	}
    }
    if(lock_flag)
	pthread_mutex_unlock(&x->lock);
	
}

void controller_assist(t_controller *x, void *b, long m, long a, char *s)
{
    if (m==ASSIST_INLET) {
	switch (a) {
	case 0: sprintf(s,"Takes as Input Messages e.g. audio <pan> <tilt> <time> or face <x> <y> <size> <time>"); break;	
	}
    }
    else {
	switch (a) {                                                    
	case 0: sprintf(s,"Sends messages to start/stop recording"); break;              
	}
    }
}

void *controller_new(t_symbol *s, short ac, t_atom *av)
{
    srand(time(NULL));
    t_controller *x;	
    x = (t_controller *)newobject(controller_class);
    x->m_handout = outlet_new(x,0L);
    x->m_facetrackerout = outlet_new(x,0L);
    x->m_messageout = outlet_new(x,0L);

    x->save_recording = 0;
    x->force_recording = 0;
    x->audio_buff_size = 64;
    x->audio_buff_pointer = 0;
    x->audio_pan_buff = (double *) malloc(sizeof(double)*x->audio_buff_size);
    x->audio_tilt_buff = (double *) malloc(sizeof(double)*x->audio_buff_size);
    x->audio_time_buff = (double *) malloc(sizeof(double)*x->audio_buff_size);
    x->audio_tdoa_buff = (float **) malloc(sizeof(float *)*x->audio_buff_size);
    x->audio_energy_buff = (float **) malloc(sizeof(float *)*x->audio_buff_size);
    int i;
    for(i=0; i < x->audio_buff_size; i++) {
	x->audio_tdoa_buff[i] = (float *) malloc(sizeof(float)*21);
	x->audio_energy_buff[i] = (float *) malloc(sizeof(float)*6);
    }
	
    x->face_buff_size = 64;
    x->face_buff_pointer = 0;
    x->face_row_buff = (int *) malloc(sizeof(int)*x->face_buff_size);
    x->face_col_buff = (int *) malloc(sizeof(int)*x->face_buff_size);
    x->face_size_buff = (int *) malloc(sizeof(int)*x->face_buff_size);
    x->face_time_buff = (double *) malloc(sizeof(double)*x->face_buff_size);

    x->zoom_mode = 0;
    x->current_pan = 0;
    x->current_tilt = 0;
    x->current_face_size = 0;
    x->current_zoom = 0;
    x->face_mode = 0;
    x->light_mode = 0;
    x->face_centered = 0;
    x->time_last_move = 0;
    x->time_last_audio_move = 0;
    x->time_last_picture_scan = 0;
    x->audio_switch = 0;
    for(i = 0; i < NUM_MICS; i++) {
	x->tdoa_array[i] = 0;
    }
    pthread_mutex_init(&x->lock,NULL);
    pthread_mutex_init(&x->curl_lock,NULL);
    return x;
}

void Start(t_controller * x) {
    char cmd[128];
    sprintf(cmd, "/usr/bin/osascript /Users/cameraman/cameraman/controller/run_quicktime.scpt");
    ExecCmd(cmd);

    sprintf(x->movie_path, "/Users/cameraman/Documents/Quicktime_Recordings/log.txt");
    x->current_pan = 0;
    x->current_tilt = -20;
    x->current_zoom = 0;
    x->log_file = NULL;
    x->recording_mode = 0;
    x->has_camera_moved = 0;
    x->curl = curl_easy_init();
    CallPanTilt(x,x->current_pan,x->current_tilt);
    CallZoom(x, 0);
    pthread_t threads;
    pthread_attr_t attr;
    if(pthread_attr_init(&attr)) {
	log_it(x->log_file, "ERROR: Can't init pthread attr\n",1);
	exit(-1);
    }
    if(pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED)) {
	log_it(x->log_file, "ERROR: Can't set inherit pthread attr\n",1);
	exit(-1);
    }
    if(pthread_attr_setschedpolicy(&attr, SCHED_RR)) {
	log_it(x->log_file,"ERROR: Can't set schedpolicy pthread attr\n",1);
	exit(-1);
    }
    struct sched_param schedParam;
    schedParam.sched_priority = sched_get_priority_max(SCHED_RR);
    if (pthread_attr_setschedparam(&attr, &schedParam)) {
	log_it(x->log_file, "Failed to set scheduler parameters\n",1);
	exit(-1);
    }
    
    int rc;
    /*Fire up controller thread*/
    rc = pthread_create(&threads, &attr, (void *) Run_Controller, x);
    if(rc) {
	log_it(x->log_file, "ERROR: Creating controller thread!",1);
	exit(-1);
    }    
    outlet_anything(x->m_facetrackerout,gensym("face_stop"), 0,NULL); 
}

void controller_free(t_controller *x) {
    curl_easy_cleanup(x->curl);
    free(x->audio_pan_buff);
    free(x->audio_tilt_buff);
    free(x->audio_time_buff);
    int i;
    for(i=0; i < x->audio_buff_size; i++) {
	free(x->audio_tdoa_buff[i]);
	free(x->audio_energy_buff[i]);
    }
    free(x->audio_energy_buff);
    free(x->audio_tdoa_buff);
    free(x->face_row_buff);
    free(x->face_col_buff);
    free(x->face_size_buff);
    free(x->face_time_buff);
}

void clear_face_buffer(t_controller * x) {
    int i;
    pthread_mutex_lock(&x->lock);
    for(i = 0; i < x->face_buff_size; i++) {
	x->face_row_buff[i] = 0;
	x->face_col_buff[i] = 0;
	x->face_size_buff[i] = 0;
	x->face_time_buff[i] = 0;
    }
    pthread_mutex_unlock(&x->lock);
}

void clear_audio_buffer(t_controller * x) {
    int i;
    pthread_mutex_lock(&x->lock);
    for(i = 0; i < x->audio_buff_size; i++) {
	x->audio_time_buff[i] = 0;
    }
    pthread_mutex_unlock(&x->lock);
}

void *Run_Controller(t_controller * x) {
    log_it(x->log_file, "Controller started!",1);
    struct timespec req = {0, 10000000};
    while(!x->kill_flag) {
	set_face_mode(x);
	/* eettinger 6/9/10: UNCOMMENT picture_control to start
	   picture taking every 10 minutes of room 
	   picture_control(x); */
	audio_control(x);
	if(x->face_mode > 0 || x->light_mode) {
	    pthread_mutex_lock(&x->lock);
	    if(x->light_mode == 2) {
		x->face_mode = 1;
	    } else if(x->light_mode == 1) {
		x->face_mode = 0;
	    }
	    double before =  x->time_last_move;
	    pthread_mutex_unlock(&x->lock);
	    face_control(x);
	    pthread_mutex_lock(&x->lock);
	    double after =  x->time_last_move;
	    pthread_mutex_unlock(&x->lock);
	    if(before != after) {
		req.tv_nsec = 100000000;
		nanosleep(&req, NULL);
		clear_face_buffer(x);
		req.tv_nsec = 10000000;
	    }
	}
	nanosleep(&req,NULL);
    }
    log_it(x->log_file, "Controller killed!",1);
    pthread_exit(NULL);
}
