#ifndef NH_PF_H
#define NH_PF_H

#include "ext.h"
#include <stdlib.h>
#include <math.h>
#include "TdoaMap.h"

#define NUM_PARTICLES 50
#define NUM_PAIRS 21
#define TOL 1e-10
#define ALPHA 0.04
#ifndef max
#define max( a, b ) ( ((a) > (b)) ? (a) : (b) )
#endif


typedef struct _Zscore {
    int pair; /* What pair is this Zscore for */
    int tdoa; /* What is the TDOA */
    double zscore; /* What is the zscore value */
} Zscore;

double nh_weights[NUM_PARTICLES];
double nh_particles[NUM_PARTICLES][NUM_PAIRS];
double nh_regrets[NUM_PARTICLES];
long numparticles_lvl[3] = {0,0,0};
long nh_lvl[NUM_PARTICLES];

int double_comp(double * a, double * b) {
    if(*a == *b) return 0;
    if(*a > *b) return 1;
    return -1;
}

double GetNormalDraw(double mean, double variance) {
    double U = ((double)rand())/((double) RAND_MAX + 1.0);
    double V = ((double)rand())/((double) RAND_MAX + 1.0);
    return mean + sqrt(variance)*sqrt(-2*log(U))*cos(2*M_PI*V);
}

void ComputeProjection(float * projection, double * particle) {
    int i,j;
    int index = 0;  //Use root node for coordinates.
    projection[0] = projection[1] = projection[2] = 0.0;
    for(i = 0; i < NUM_PAIRS; i++) {
	for(j=0; j < 3; j++) {
	    projection[j] += ((float) ((double) particle[i] - pcaTree.Node[index].mean[i])*pcaTree.Node[index].pcs[j][i]);
	}
    }
}

void ProjectParticle(double * particle, int lvl) {
    int index = 1;
    int i,j;
    /* Find The Appropriate Node */
    double p = 0.0;
    for(i = 0; i < lvl; i++) {
	for(j = 0; j < NUM_PAIRS; j++) {
	    p += ((double) particle[j] - pcaTree.Node[index].mean[j])*pcaTree.Node[index].pcs[0][j];
	}
	index = (p - pcaTree.Node[index].thresh >= 0.0) ? index*2 + 1 : index*2;
    }
    index--; /* We're 0-based, not 1-based for this tree in an array */

    /* denoise particle based on PCA of root node */
    double c[3] = {0.0,0.0,0.0};
    for(i = 0; i < NUM_PAIRS; i++) {
	for(j=0; j < 3; j++) {
	    c[j] += ((double) particle[i] - pcaTree.Node[index].mean[i])*pcaTree.Node[index].pcs[j][i];
	}
	particle[i] = pcaTree.Node[index].mean[i];
    }
    for(i = 0; i < NUM_PAIRS; i++) {
	for(j = 0; j < 3; j++) {
	    particle[i] += c[j]*pcaTree.Node[index].pcs[j][i];
	}
    }
}

int SampleParticle(double * particle, double * init_state, double variance) {
    int i;
    for(i = 0; i < NUM_PAIRS; i++) {
	particle[i] = GetNormalDraw(init_state[i], variance);
    }
    double r = ((double)rand())/((double) RAND_MAX + 1.0);
    int lvl = 0;
    if(r < 1.0/3.0) {
	lvl = 1;
    }
    else if(r < 2.0/3.0) {
	lvl = 2;
    }	
    ProjectParticle(particle, lvl);
    return lvl;
}

void InitPf() {
    int i;
    double init_state[NUM_PAIRS] = { 0.0,0.0,0.0,0.0,0.0,0.0,0.0,
				     0.0,0.0,0.0,0.0,0.0,0.0,0.0,
				     0.0,0.0,0.0,0.0,0.0,0.0,0.0 };
    srand((int) time(NULL));
    for(i = 0; i < NUM_PARTICLES; i++) {
	nh_weights[i] = 0.0;
	nh_regrets[i] = 0.0;
	int lvl = SampleParticle(&nh_particles[i][0], init_state, 10);
	nh_lvl[i] = lvl;
	numparticles_lvl[lvl]++;
    }
}

#define NUM_REINIT 3
void ReInitPf(double * state) {
    int i,j;
    double min_weights[NUM_REINIT] = {1.0,1.0,1.0};
    int min_inds[NUM_REINIT] = {-1,-1,-1};
    for(i = 0; i < NUM_PARTICLES; i++) {
	for(j = 0; j < NUM_REINIT; j++) {
	    if(nh_weights[i] < min_weights[j]) {
		min_weights[j] = nh_weights[i];
		min_inds[j] = i;
		break;
	    }
	}
    }
    for(i = 0; i < NUM_REINIT; i++) {
	int lvl = SampleParticle(&nh_particles[min_inds[i]][0], state, 4);
	numparticles_lvl[nh_lvl[min_inds[i]]]--;
	nh_lvl[min_inds[i]] = lvl;
	numparticles_lvl[lvl]++;
    }
}

double C_Sum(double * regrets, double c) {
    double ret_val = 0.0;
    int i;
    for(i = 0; i < NUM_PARTICLES; i++) {
	ret_val += exp(pow(max(regrets[i],0),2)/(2*c));
    }
    return ret_val;
}

double GetNormPdf(double x, double mu, double sigma) {
    return (1/(sigma*sqrt(2*M_PI)))*exp((-1*pow(x - mu,2))/(2*pow(sigma,2)));
}

double GetScore(double * particle, Zscore * z, int n_z) {
    int i;
    double score = 0.0;   
    for(i = 0; i < n_z; i++) {
	score += GetNormPdf(z[i].tdoa, particle[z[i].pair], 10)*z[i].zscore;
    }
    return score;
}

double UpdateRegrets(double * regrets, double particles[][NUM_PAIRS], 
		   double * weights, Zscore * z, int n_z) {
    int i;
    double scores[NUM_PARTICLES];
    double master_score = 0.0;
    for(i = 0; i < NUM_PARTICLES; i++) {
	scores[i] = GetScore(&(particles[i][0]), z, n_z);
	master_score += weights[i]*scores[i];
    }
    for(i = 0; i < NUM_PARTICLES; i++) {
	regrets[i] = (1 - ALPHA)*regrets[i] + (scores[i] - master_score);
    }
    return master_score;
}

void ResampleStep(double * regrets, double particles[][NUM_PAIRS], double * weights,
		  Zscore * z, int n_z, double master_score) {
    int num_dead = 0;
    double sum_weights = 0.0;
    int i;
    for(i = 0; i < NUM_PARTICLES; i++) {
	if(regrets[i] > 0) {
	    sum_weights += weights[i];
	}
	else {
	    weights[i] = 0.0;
	    num_dead++;
	}
    }
/*    post("(nh_pf): num dead particles = %d", num_dead);*/
    if(num_dead == 0) return;

    for(i = 0; i < NUM_PARTICLES; i++) {
	weights[i] /= sum_weights;
    }
    
    int ii = 0;
    int jj = 0;
    int ii_ind = 0;
    double total = 0.0;
    double score;
    double * u = malloc(sizeof(double)*num_dead);
    for(i = 0; i < num_dead; i++) {
	u[i] = ((double)rand())/((double) RAND_MAX + 1.0);
    }
    qsort(u, num_dead, sizeof(double), (void *) double_comp);
    while(weights[ii_ind] != 0.0 && ii_ind < NUM_PARTICLES - 1) ii_ind++;

    while(ii <= num_dead) {
	if(u[ii] < total || jj == NUM_PARTICLES - 1) {
	    int lvl = SampleParticle(&particles[ii_ind][0], &particles[jj][0], 4);
	    numparticles_lvl[nh_lvl[ii_ind]]--;
	    nh_lvl[ii_ind] = lvl;
	    numparticles_lvl[lvl]++;
	    score = GetScore(&particles[ii_ind][0], z, n_z);
	    regrets[ii_ind] = (1 - ALPHA)*regrets[jj] + (score - master_score);
	    if(ii_ind < NUM_PARTICLES - 1) ii_ind++;
	    while(weights[ii_ind] != 0.0 && ii_ind < NUM_PARTICLES - 1) ii_ind++;
	    ii++;
	}
	else {
	    jj++;
	    total += weights[jj];
	}
    }
    free(u);
}

void UpdateWeights(double * regrets, double * weights) {
    int N = NUM_PARTICLES;
    double potential = M_E*N;
    double c_lb = TOL;
    double c_ub = TOL;

    /* Binary search for solution to NH equation for C */
    while(C_Sum(regrets,c_ub) > potential) {
	c_ub *= 2.0;
    }
    
    double c = (c_lb + c_ub)/2.0;
    double err = C_Sum(regrets,c) - potential;
    while(abs(err) > TOL && (c_lb < c) && (c < c_ub)) {
	if(err > 0) c_lb = c;
	else c_ub = c;
	c = (c_lb + c_ub)/2.0;
	err = C_Sum(regrets, c) - potential;
    }
    /* End binary search for C */
    int i;
    double tot_w = 0.0;
    for(i = 0; i < NUM_PARTICLES; i++) {
	weights[i] = max(regrets[i],0.0)*exp(pow(max(regrets[i],0.0),2)/(2*c));
	tot_w += weights[i];
    }
    if(tot_w < TOL) {
	for(i = 0; i < NUM_PARTICLES; i++) {
	    weights[i] = 1.0/((double) N);
	}
    }
    else {
	for(i = 0; i < NUM_PARTICLES; i++) {
	    weights[i] /= tot_w;
	}
    }
}

void ComputeMasterPrediction(double * prediction, double * weights, 
			     double particles[][NUM_PAIRS]) {
    int i,j;
    for(i = 0; i < NUM_PAIRS; i++) {
	prediction[i] = 0.0;
	for(j = 0; j < NUM_PARTICLES; j++) {
	    prediction[i] += weights[j]*particles[j][i];
	}
    }
}

void StepPf(void * message, void * processing, double * prediction, Zscore * z, int n_z, int * num_depth, float * projected_center) {
    /* Get Scores, Master Score, and Update regrets */
    double master_score = UpdateRegrets(nh_regrets, nh_particles, nh_weights, z, n_z);

    /* Resample Dead Particles */
    ResampleStep(nh_regrets, nh_particles, nh_weights, z, n_z, master_score);
    
    /* Compute Particle Weights */
    UpdateWeights(nh_regrets, nh_weights);

    /* Compute Master Prediction */
    ComputeMasterPrediction(prediction, nh_weights, nh_particles);
    
    /*Output num particles at each level*/
    int i;
    t_atom myList[3];
    for(i=0; i < 3; i++) {
	num_depth[i] = numparticles_lvl[i];
	SETLONG(myList+i,numparticles_lvl[i]);
    }
    outlet_list(message, 0L, 3, (void *) &myList);
    
    /*Output information for processing graphic*/
    if(z != NULL) {
	ComputeProjection(projected_center, prediction);
	t_atom pList[4];
	for(i=0; i < 3; i++) {
	    SETFLOAT(pList+i, projected_center[i]);
	}
	if(numparticles_lvl[0] > numparticles_lvl[1] && 
	   numparticles_lvl[0] > numparticles_lvl[2]) {
	    SETFLOAT(pList+3,0.0);
	}
	else if(numparticles_lvl[1] > numparticles_lvl[2]) {
	    SETFLOAT(pList+3,1.0);
	}
	else {
	    SETFLOAT(pList+3,2.0);
	}
	outlet_list(processing, 0L, 4, (void *) &pList);
    }
}

#endif
