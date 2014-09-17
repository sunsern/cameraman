/*
 * File: Filter.h
 *
 * Real-Time Workshop code generated for Simulink model Filter.
 *
 * Model version                        : 1.6
 * Real-Time Workshop file version      : 6.5  (R2006b Beta)  21-Sep-2006
 * Real-Time Workshop file generated on : Wed Jul  9 18:09:11 2008
 * TLC version                          : 6.5 (Sep 20 2006)
 * C source code generated on           : Wed Jul  9 18:09:12 2008
 */
#ifndef _RTW_HEADER_Filter_h_
#define _RTW_HEADER_Filter_h_
#ifndef _Filter_COMMON_INCLUDES_
# define _Filter_COMMON_INCLUDES_
#include <math.h>
#include <stddef.h>
#include "rtwtypes.h"
#endif                                 /* _Filter_COMMON_INCLUDES_ */

#include "Filter_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* Block signals (auto storage) */
typedef struct {
  real32_T Stage9[400];                /* '<S7>/Stage9' */
  real32_T Stage10[400];               /* '<S7>/Stage10' */
} BlockIO_Filter;

/* Block states (auto storage) for system '<Root>' */
typedef struct {
  real32_T MultiBandFilter_FILT_STATES[1600];/* '<Root>/MultiBandFilter' */
  real32_T Stage1_FILT_STATES[10400];  /* '<S1>/Stage1' */
  real32_T Stage2_FILT_STATES[10400];  /* '<S1>/Stage2' */
  real32_T Stage3_FILT_STATES[10400];  /* '<S1>/Stage3' */
  real32_T Stage4_FILT_STATES[10400];  /* '<S1>/Stage4' */
  real32_T Stage5_FILT_STATES[10400];  /* '<S1>/Stage5' */
  real32_T Stage6_FILT_STATES[10400];  /* '<S1>/Stage6' */
  real32_T Stage7_FILT_STATES[10400];  /* '<S1>/Stage7' */
  real32_T Stage8_FILT_STATES[9600];   /* '<S1>/Stage8' */
  real32_T Stage9_FILT_STATES[9600];   /* '<S1>/Stage9' */
  real32_T Stage10_FILT_STATES[9600];  /* '<S1>/Stage10' */
  real32_T Stage1_FILT_STATES_e[10400];/* '<S2>/Stage1' */
  real32_T Stage2_FILT_STATES_a[10400];/* '<S2>/Stage2' */
  real32_T Stage3_FILT_STATES_c[10400];/* '<S2>/Stage3' */
  real32_T Stage4_FILT_STATES_n[10400];/* '<S2>/Stage4' */
  real32_T Stage5_FILT_STATES_f[10400];/* '<S2>/Stage5' */
  real32_T Stage6_FILT_STATES_b[10400];/* '<S2>/Stage6' */
  real32_T Stage7_FILT_STATES_a[10400];/* '<S2>/Stage7' */
  real32_T Stage8_FILT_STATES_o[9600]; /* '<S2>/Stage8' */
  real32_T Stage9_FILT_STATES_p[9600]; /* '<S2>/Stage9' */
  real32_T Stage10_FILT_STATES_f[9600];/* '<S2>/Stage10' */
  real32_T Stage1_FILT_STATES_j[10400];/* '<S3>/Stage1' */
  real32_T Stage2_FILT_STATES_f[10400];/* '<S3>/Stage2' */
  real32_T Stage3_FILT_STATES_d[10400];/* '<S3>/Stage3' */
  real32_T Stage4_FILT_STATES_o[10400];/* '<S3>/Stage4' */
  real32_T Stage5_FILT_STATES_l[10400];/* '<S3>/Stage5' */
  real32_T Stage6_FILT_STATES_m[10400];/* '<S3>/Stage6' */
  real32_T Stage7_FILT_STATES_d[10400];/* '<S3>/Stage7' */
  real32_T Stage8_FILT_STATES_a[9600]; /* '<S3>/Stage8' */
  real32_T Stage9_FILT_STATES_p3[9600];/* '<S3>/Stage9' */
  real32_T Stage10_FILT_STATES_m[9600];/* '<S3>/Stage10' */
  real32_T Stage1_FILT_STATES_k[10400];/* '<S4>/Stage1' */
  real32_T Stage2_FILT_STATES_j[10400];/* '<S4>/Stage2' */
  real32_T Stage3_FILT_STATES_ci[10400];/* '<S4>/Stage3' */
  real32_T Stage4_FILT_STATES_h[10400];/* '<S4>/Stage4' */
  real32_T Stage5_FILT_STATES_o[10400];/* '<S4>/Stage5' */
  real32_T Stage6_FILT_STATES_e[10400];/* '<S4>/Stage6' */
  real32_T Stage7_FILT_STATES_j[10400];/* '<S4>/Stage7' */
  real32_T Stage8_FILT_STATES_m[9600]; /* '<S4>/Stage8' */
  real32_T Stage9_FILT_STATES_a[9600]; /* '<S4>/Stage9' */
  real32_T Stage10_FILT_STATES_a[9600];/* '<S4>/Stage10' */
  real32_T Stage1_FILT_STATES_j3[10400];/* '<S5>/Stage1' */
  real32_T Stage2_FILT_STATES_n[10400];/* '<S5>/Stage2' */
  real32_T Stage3_FILT_STATES_j[10400];/* '<S5>/Stage3' */
  real32_T Stage4_FILT_STATES_p[10400];/* '<S5>/Stage4' */
  real32_T Stage5_FILT_STATES_n[10400];/* '<S5>/Stage5' */
  real32_T Stage6_FILT_STATES_bo[10400];/* '<S5>/Stage6' */
  real32_T Stage7_FILT_STATES_h[10400];/* '<S5>/Stage7' */
  real32_T Stage8_FILT_STATES_n[9600]; /* '<S5>/Stage8' */
  real32_T Stage9_FILT_STATES_l[9600]; /* '<S5>/Stage9' */
  real32_T Stage10_FILT_STATES_e[9600];/* '<S5>/Stage10' */
  real32_T Stage1_FILT_STATES_a[10400];/* '<S6>/Stage1' */
  real32_T Stage2_FILT_STATES_o[10400];/* '<S6>/Stage2' */
  real32_T Stage3_FILT_STATES_h[10400];/* '<S6>/Stage3' */
  real32_T Stage4_FILT_STATES_f[10400];/* '<S6>/Stage4' */
  real32_T Stage5_FILT_STATES_e[10400];/* '<S6>/Stage5' */
  real32_T Stage6_FILT_STATES_i[10400];/* '<S6>/Stage6' */
  real32_T Stage7_FILT_STATES_m[10400];/* '<S6>/Stage7' */
  real32_T Stage8_FILT_STATES_nh[9600];/* '<S6>/Stage8' */
  real32_T Stage9_FILT_STATES_n[9600]; /* '<S6>/Stage9' */
  real32_T Stage10_FILT_STATES_k[9600];/* '<S6>/Stage10' */
  real32_T Stage1_FILT_STATES_c[10400];/* '<S7>/Stage1' */
  real32_T Stage2_FILT_STATES_e[10400];/* '<S7>/Stage2' */
  real32_T Stage3_FILT_STATES_k[10400];/* '<S7>/Stage3' */
  real32_T Stage4_FILT_STATES_pv[10400];/* '<S7>/Stage4' */
  real32_T Stage5_FILT_STATES_g[10400];/* '<S7>/Stage5' */
  real32_T Stage6_FILT_STATES_j[10400];/* '<S7>/Stage6' */
  real32_T Stage7_FILT_STATES_e[10400];/* '<S7>/Stage7' */
  real32_T Stage8_FILT_STATES_i[9600]; /* '<S7>/Stage8' */
  real32_T Stage9_FILT_STATES_pa[9600];/* '<S7>/Stage9' */
  real32_T Stage10_FILT_STATES_j[9600];/* '<S7>/Stage10' */
} D_Work_Filter;

/* External inputs (root inport signals with auto storage) */
typedef struct {
  real32_T Channel1[400];              /* '<Root>/Channel 1' */
  real32_T Channel2[400];              /* '<Root>/Channel 2' */
  real32_T Channel3[400];              /* '<Root>/Channel 3' */
  real32_T Channel4[400];              /* '<Root>/Channel 4' */
  real32_T Channel5[400];              /* '<Root>/Channel 5' */
  real32_T Channel6[400];              /* '<Root>/Channel 6' */
  real32_T Channel7[400];              /* '<Root>/Channel 7' */
  real32_T Channel8[400];              /* '<Root>/Channel 8' */
} ExternalInputs_Filter;

/* External outputs (root outports fed by signals with auto storage) */
typedef struct {
  real32_T Output1[400];               /* '<Root>/Output 1' */
  real32_T Output2[400];               /* '<Root>/Output 2' */
  real32_T Output3[400];               /* '<Root>/Output 3' */
  real32_T Output4[400];               /* '<Root>/Output 4' */
  real32_T Output5[400];               /* '<Root>/Output 5' */
  real32_T Output6[400];               /* '<Root>/Output 6' */
  real32_T Output7[400];               /* '<Root>/Output 7' */
  real32_T Output8[400];               /* '<Root>/Output 8' */
} ExternalOutputs_Filter;

/* Parameters (auto storage) */
struct Parameters_Filter {
  real32_T MultiBandFilter_ICRTP;      /* Expression: a.IC
                                        * '<Root>/MultiBandFilter'
                                        */
  real32_T MultiBandFilter_RTP1COEFF[10];/* Expression: a.SOSCoeffs
                                        * '<Root>/MultiBandFilter'
                                        */
  real32_T MultiBandFilter_RTP2COEFF[3];/* Expression: a.ScaleValues
                                        * '<Root>/MultiBandFilter'
                                        */
  real32_T Stage1_ICRTP;               /* Expression: a.IC
                                        * '<S1>/Stage1'
                                        */
  real32_T Stage1_RTP1COEFF[65];       /* Expression: a.SOSCoeffs
                                        * '<S1>/Stage1'
                                        */
  real32_T Stage1_RTP2COEFF[14];       /* Expression: a.ScaleValues
                                        * '<S1>/Stage1'
                                        */
  real32_T Stage2_ICRTP;               /* Expression: a.IC
                                        * '<S1>/Stage2'
                                        */
  real32_T Stage2_RTP1COEFF[65];       /* Expression: a.SOSCoeffs
                                        * '<S1>/Stage2'
                                        */
  real32_T Stage2_RTP2COEFF[14];       /* Expression: a.ScaleValues
                                        * '<S1>/Stage2'
                                        */
  real32_T Stage3_ICRTP;               /* Expression: a.IC
                                        * '<S1>/Stage3'
                                        */
  real32_T Stage3_RTP1COEFF[65];       /* Expression: a.SOSCoeffs
                                        * '<S1>/Stage3'
                                        */
  real32_T Stage3_RTP2COEFF[14];       /* Expression: a.ScaleValues
                                        * '<S1>/Stage3'
                                        */
  real32_T Stage4_ICRTP;               /* Expression: a.IC
                                        * '<S1>/Stage4'
                                        */
  real32_T Stage4_RTP1COEFF[65];       /* Expression: a.SOSCoeffs
                                        * '<S1>/Stage4'
                                        */
  real32_T Stage4_RTP2COEFF[14];       /* Expression: a.ScaleValues
                                        * '<S1>/Stage4'
                                        */
  real32_T Stage5_ICRTP;               /* Expression: a.IC
                                        * '<S1>/Stage5'
                                        */
  real32_T Stage5_RTP1COEFF[65];       /* Expression: a.SOSCoeffs
                                        * '<S1>/Stage5'
                                        */
  real32_T Stage5_RTP2COEFF[14];       /* Expression: a.ScaleValues
                                        * '<S1>/Stage5'
                                        */
  real32_T Stage6_ICRTP;               /* Expression: a.IC
                                        * '<S1>/Stage6'
                                        */
  real32_T Stage6_RTP1COEFF[65];       /* Expression: a.SOSCoeffs
                                        * '<S1>/Stage6'
                                        */
  real32_T Stage6_RTP2COEFF[14];       /* Expression: a.ScaleValues
                                        * '<S1>/Stage6'
                                        */
  real32_T Stage7_ICRTP;               /* Expression: a.IC
                                        * '<S1>/Stage7'
                                        */
  real32_T Stage7_RTP1COEFF[65];       /* Expression: a.SOSCoeffs
                                        * '<S1>/Stage7'
                                        */
  real32_T Stage7_RTP2COEFF[14];       /* Expression: a.ScaleValues
                                        * '<S1>/Stage7'
                                        */
  real32_T Stage8_ICRTP;               /* Expression: a.IC
                                        * '<S1>/Stage8'
                                        */
  real32_T Stage8_RTP1COEFF[60];       /* Expression: a.SOSCoeffs
                                        * '<S1>/Stage8'
                                        */
  real32_T Stage8_RTP2COEFF[13];       /* Expression: a.ScaleValues
                                        * '<S1>/Stage8'
                                        */
  real32_T Stage9_ICRTP;               /* Expression: a.IC
                                        * '<S1>/Stage9'
                                        */
  real32_T Stage9_RTP1COEFF[60];       /* Expression: a.SOSCoeffs
                                        * '<S1>/Stage9'
                                        */
  real32_T Stage9_RTP2COEFF[13];       /* Expression: a.ScaleValues
                                        * '<S1>/Stage9'
                                        */
  real32_T Stage10_ICRTP;              /* Expression: a.IC
                                        * '<S1>/Stage10'
                                        */
  real32_T Stage10_RTP1COEFF[60];      /* Expression: a.SOSCoeffs
                                        * '<S1>/Stage10'
                                        */
  real32_T Stage10_RTP2COEFF[13];      /* Expression: a.ScaleValues
                                        * '<S1>/Stage10'
                                        */
  real32_T Stage1_ICRTP_j;             /* Expression: a.IC
                                        * '<S2>/Stage1'
                                        */
  real32_T Stage1_RTP1COEFF_m[65];     /* Expression: a.SOSCoeffs
                                        * '<S2>/Stage1'
                                        */
  real32_T Stage1_RTP2COEFF_h[14];     /* Expression: a.ScaleValues
                                        * '<S2>/Stage1'
                                        */
  real32_T Stage2_ICRTP_e;             /* Expression: a.IC
                                        * '<S2>/Stage2'
                                        */
  real32_T Stage2_RTP1COEFF_h[65];     /* Expression: a.SOSCoeffs
                                        * '<S2>/Stage2'
                                        */
  real32_T Stage2_RTP2COEFF_f[14];     /* Expression: a.ScaleValues
                                        * '<S2>/Stage2'
                                        */
  real32_T Stage3_ICRTP_o;             /* Expression: a.IC
                                        * '<S2>/Stage3'
                                        */
  real32_T Stage3_RTP1COEFF_j[65];     /* Expression: a.SOSCoeffs
                                        * '<S2>/Stage3'
                                        */
  real32_T Stage3_RTP2COEFF_d[14];     /* Expression: a.ScaleValues
                                        * '<S2>/Stage3'
                                        */
  real32_T Stage4_ICRTP_m;             /* Expression: a.IC
                                        * '<S2>/Stage4'
                                        */
  real32_T Stage4_RTP1COEFF_l[65];     /* Expression: a.SOSCoeffs
                                        * '<S2>/Stage4'
                                        */
  real32_T Stage4_RTP2COEFF_b[14];     /* Expression: a.ScaleValues
                                        * '<S2>/Stage4'
                                        */
  real32_T Stage5_ICRTP_m;             /* Expression: a.IC
                                        * '<S2>/Stage5'
                                        */
  real32_T Stage5_RTP1COEFF_j[65];     /* Expression: a.SOSCoeffs
                                        * '<S2>/Stage5'
                                        */
  real32_T Stage5_RTP2COEFF_d[14];     /* Expression: a.ScaleValues
                                        * '<S2>/Stage5'
                                        */
  real32_T Stage6_ICRTP_p;             /* Expression: a.IC
                                        * '<S2>/Stage6'
                                        */
  real32_T Stage6_RTP1COEFF_o[65];     /* Expression: a.SOSCoeffs
                                        * '<S2>/Stage6'
                                        */
  real32_T Stage6_RTP2COEFF_h[14];     /* Expression: a.ScaleValues
                                        * '<S2>/Stage6'
                                        */
  real32_T Stage7_ICRTP_o;             /* Expression: a.IC
                                        * '<S2>/Stage7'
                                        */
  real32_T Stage7_RTP1COEFF_n[65];     /* Expression: a.SOSCoeffs
                                        * '<S2>/Stage7'
                                        */
  real32_T Stage7_RTP2COEFF_o[14];     /* Expression: a.ScaleValues
                                        * '<S2>/Stage7'
                                        */
  real32_T Stage8_ICRTP_c;             /* Expression: a.IC
                                        * '<S2>/Stage8'
                                        */
  real32_T Stage8_RTP1COEFF_h[60];     /* Expression: a.SOSCoeffs
                                        * '<S2>/Stage8'
                                        */
  real32_T Stage8_RTP2COEFF_c[13];     /* Expression: a.ScaleValues
                                        * '<S2>/Stage8'
                                        */
  real32_T Stage9_ICRTP_p;             /* Expression: a.IC
                                        * '<S2>/Stage9'
                                        */
  real32_T Stage9_RTP1COEFF_o[60];     /* Expression: a.SOSCoeffs
                                        * '<S2>/Stage9'
                                        */
  real32_T Stage9_RTP2COEFF_o[13];     /* Expression: a.ScaleValues
                                        * '<S2>/Stage9'
                                        */
  real32_T Stage10_ICRTP_d;            /* Expression: a.IC
                                        * '<S2>/Stage10'
                                        */
  real32_T Stage10_RTP1COEFF_o[60];    /* Expression: a.SOSCoeffs
                                        * '<S2>/Stage10'
                                        */
  real32_T Stage10_RTP2COEFF_g[13];    /* Expression: a.ScaleValues
                                        * '<S2>/Stage10'
                                        */
  real32_T Stage1_ICRTP_g;             /* Expression: a.IC
                                        * '<S3>/Stage1'
                                        */
  real32_T Stage1_RTP1COEFF_o[65];     /* Expression: a.SOSCoeffs
                                        * '<S3>/Stage1'
                                        */
  real32_T Stage1_RTP2COEFF_n[14];     /* Expression: a.ScaleValues
                                        * '<S3>/Stage1'
                                        */
  real32_T Stage2_ICRTP_m;             /* Expression: a.IC
                                        * '<S3>/Stage2'
                                        */
  real32_T Stage2_RTP1COEFF_n[65];     /* Expression: a.SOSCoeffs
                                        * '<S3>/Stage2'
                                        */
  real32_T Stage2_RTP2COEFF_m[14];     /* Expression: a.ScaleValues
                                        * '<S3>/Stage2'
                                        */
  real32_T Stage3_ICRTP_h;             /* Expression: a.IC
                                        * '<S3>/Stage3'
                                        */
  real32_T Stage3_RTP1COEFF_i[65];     /* Expression: a.SOSCoeffs
                                        * '<S3>/Stage3'
                                        */
  real32_T Stage3_RTP2COEFF_p[14];     /* Expression: a.ScaleValues
                                        * '<S3>/Stage3'
                                        */
  real32_T Stage4_ICRTP_n;             /* Expression: a.IC
                                        * '<S3>/Stage4'
                                        */
  real32_T Stage4_RTP1COEFF_d[65];     /* Expression: a.SOSCoeffs
                                        * '<S3>/Stage4'
                                        */
  real32_T Stage4_RTP2COEFF_n[14];     /* Expression: a.ScaleValues
                                        * '<S3>/Stage4'
                                        */
  real32_T Stage5_ICRTP_a;             /* Expression: a.IC
                                        * '<S3>/Stage5'
                                        */
  real32_T Stage5_RTP1COEFF_jd[65];    /* Expression: a.SOSCoeffs
                                        * '<S3>/Stage5'
                                        */
  real32_T Stage5_RTP2COEFF_i[14];     /* Expression: a.ScaleValues
                                        * '<S3>/Stage5'
                                        */
  real32_T Stage6_ICRTP_b;             /* Expression: a.IC
                                        * '<S3>/Stage6'
                                        */
  real32_T Stage6_RTP1COEFF_j[65];     /* Expression: a.SOSCoeffs
                                        * '<S3>/Stage6'
                                        */
  real32_T Stage6_RTP2COEFF_n[14];     /* Expression: a.ScaleValues
                                        * '<S3>/Stage6'
                                        */
  real32_T Stage7_ICRTP_g;             /* Expression: a.IC
                                        * '<S3>/Stage7'
                                        */
  real32_T Stage7_RTP1COEFF_i[65];     /* Expression: a.SOSCoeffs
                                        * '<S3>/Stage7'
                                        */
  real32_T Stage7_RTP2COEFF_b[14];     /* Expression: a.ScaleValues
                                        * '<S3>/Stage7'
                                        */
  real32_T Stage8_ICRTP_k;             /* Expression: a.IC
                                        * '<S3>/Stage8'
                                        */
  real32_T Stage8_RTP1COEFF_l[60];     /* Expression: a.SOSCoeffs
                                        * '<S3>/Stage8'
                                        */
  real32_T Stage8_RTP2COEFF_n[13];     /* Expression: a.ScaleValues
                                        * '<S3>/Stage8'
                                        */
  real32_T Stage9_ICRTP_b;             /* Expression: a.IC
                                        * '<S3>/Stage9'
                                        */
  real32_T Stage9_RTP1COEFF_b[60];     /* Expression: a.SOSCoeffs
                                        * '<S3>/Stage9'
                                        */
  real32_T Stage9_RTP2COEFF_i[13];     /* Expression: a.ScaleValues
                                        * '<S3>/Stage9'
                                        */
  real32_T Stage10_ICRTP_m;            /* Expression: a.IC
                                        * '<S3>/Stage10'
                                        */
  real32_T Stage10_RTP1COEFF_a[60];    /* Expression: a.SOSCoeffs
                                        * '<S3>/Stage10'
                                        */
  real32_T Stage10_RTP2COEFF_a[13];    /* Expression: a.ScaleValues
                                        * '<S3>/Stage10'
                                        */
  real32_T Stage1_ICRTP_o;             /* Expression: a.IC
                                        * '<S4>/Stage1'
                                        */
  real32_T Stage1_RTP1COEFF_f[65];     /* Expression: a.SOSCoeffs
                                        * '<S4>/Stage1'
                                        */
  real32_T Stage1_RTP2COEFF_no[14];    /* Expression: a.ScaleValues
                                        * '<S4>/Stage1'
                                        */
  real32_T Stage2_ICRTP_m3;            /* Expression: a.IC
                                        * '<S4>/Stage2'
                                        */
  real32_T Stage2_RTP1COEFF_l[65];     /* Expression: a.SOSCoeffs
                                        * '<S4>/Stage2'
                                        */
  real32_T Stage2_RTP2COEFF_e[14];     /* Expression: a.ScaleValues
                                        * '<S4>/Stage2'
                                        */
  real32_T Stage3_ICRTP_j;             /* Expression: a.IC
                                        * '<S4>/Stage3'
                                        */
  real32_T Stage3_RTP1COEFF_o[65];     /* Expression: a.SOSCoeffs
                                        * '<S4>/Stage3'
                                        */
  real32_T Stage3_RTP2COEFF_p1[14];    /* Expression: a.ScaleValues
                                        * '<S4>/Stage3'
                                        */
  real32_T Stage4_ICRTP_e;             /* Expression: a.IC
                                        * '<S4>/Stage4'
                                        */
  real32_T Stage4_RTP1COEFF_n[65];     /* Expression: a.SOSCoeffs
                                        * '<S4>/Stage4'
                                        */
  real32_T Stage4_RTP2COEFF_k[14];     /* Expression: a.ScaleValues
                                        * '<S4>/Stage4'
                                        */
  real32_T Stage5_ICRTP_a3;            /* Expression: a.IC
                                        * '<S4>/Stage5'
                                        */
  real32_T Stage5_RTP1COEFF_n[65];     /* Expression: a.SOSCoeffs
                                        * '<S4>/Stage5'
                                        */
  real32_T Stage5_RTP2COEFF_b[14];     /* Expression: a.ScaleValues
                                        * '<S4>/Stage5'
                                        */
  real32_T Stage6_ICRTP_pb;            /* Expression: a.IC
                                        * '<S4>/Stage6'
                                        */
  real32_T Stage6_RTP1COEFF_h[65];     /* Expression: a.SOSCoeffs
                                        * '<S4>/Stage6'
                                        */
  real32_T Stage6_RTP2COEFF_k[14];     /* Expression: a.ScaleValues
                                        * '<S4>/Stage6'
                                        */
  real32_T Stage7_ICRTP_j;             /* Expression: a.IC
                                        * '<S4>/Stage7'
                                        */
  real32_T Stage7_RTP1COEFF_m[65];     /* Expression: a.SOSCoeffs
                                        * '<S4>/Stage7'
                                        */
  real32_T Stage7_RTP2COEFF_f[14];     /* Expression: a.ScaleValues
                                        * '<S4>/Stage7'
                                        */
  real32_T Stage8_ICRTP_h;             /* Expression: a.IC
                                        * '<S4>/Stage8'
                                        */
  real32_T Stage8_RTP1COEFF_e[60];     /* Expression: a.SOSCoeffs
                                        * '<S4>/Stage8'
                                        */
  real32_T Stage8_RTP2COEFF_m[13];     /* Expression: a.ScaleValues
                                        * '<S4>/Stage8'
                                        */
  real32_T Stage9_ICRTP_e;             /* Expression: a.IC
                                        * '<S4>/Stage9'
                                        */
  real32_T Stage9_RTP1COEFF_i[60];     /* Expression: a.SOSCoeffs
                                        * '<S4>/Stage9'
                                        */
  real32_T Stage9_RTP2COEFF_i3[13];    /* Expression: a.ScaleValues
                                        * '<S4>/Stage9'
                                        */
  real32_T Stage10_ICRTP_j;            /* Expression: a.IC
                                        * '<S4>/Stage10'
                                        */
  real32_T Stage10_RTP1COEFF_h[60];    /* Expression: a.SOSCoeffs
                                        * '<S4>/Stage10'
                                        */
  real32_T Stage10_RTP2COEFF_am[13];   /* Expression: a.ScaleValues
                                        * '<S4>/Stage10'
                                        */
  real32_T Stage1_ICRTP_i;             /* Expression: a.IC
                                        * '<S5>/Stage1'
                                        */
  real32_T Stage1_RTP1COEFF_g[65];     /* Expression: a.SOSCoeffs
                                        * '<S5>/Stage1'
                                        */
  real32_T Stage1_RTP2COEFF_e[14];     /* Expression: a.ScaleValues
                                        * '<S5>/Stage1'
                                        */
  real32_T Stage2_ICRTP_i;             /* Expression: a.IC
                                        * '<S5>/Stage2'
                                        */
  real32_T Stage2_RTP1COEFF_j[65];     /* Expression: a.SOSCoeffs
                                        * '<S5>/Stage2'
                                        */
  real32_T Stage2_RTP2COEFF_o[14];     /* Expression: a.ScaleValues
                                        * '<S5>/Stage2'
                                        */
  real32_T Stage3_ICRTP_c;             /* Expression: a.IC
                                        * '<S5>/Stage3'
                                        */
  real32_T Stage3_RTP1COEFF_b[65];     /* Expression: a.SOSCoeffs
                                        * '<S5>/Stage3'
                                        */
  real32_T Stage3_RTP2COEFF_e[14];     /* Expression: a.ScaleValues
                                        * '<S5>/Stage3'
                                        */
  real32_T Stage4_ICRTP_j;             /* Expression: a.IC
                                        * '<S5>/Stage4'
                                        */
  real32_T Stage4_RTP1COEFF_f[65];     /* Expression: a.SOSCoeffs
                                        * '<S5>/Stage4'
                                        */
  real32_T Stage4_RTP2COEFF_n4[14];    /* Expression: a.ScaleValues
                                        * '<S5>/Stage4'
                                        */
  real32_T Stage5_ICRTP_b;             /* Expression: a.IC
                                        * '<S5>/Stage5'
                                        */
  real32_T Stage5_RTP1COEFF_o[65];     /* Expression: a.SOSCoeffs
                                        * '<S5>/Stage5'
                                        */
  real32_T Stage5_RTP2COEFF_e[14];     /* Expression: a.ScaleValues
                                        * '<S5>/Stage5'
                                        */
  real32_T Stage6_ICRTP_f;             /* Expression: a.IC
                                        * '<S5>/Stage6'
                                        */
  real32_T Stage6_RTP1COEFF_hm[65];    /* Expression: a.SOSCoeffs
                                        * '<S5>/Stage6'
                                        */
  real32_T Stage6_RTP2COEFF_hr[14];    /* Expression: a.ScaleValues
                                        * '<S5>/Stage6'
                                        */
  real32_T Stage7_ICRTP_l;             /* Expression: a.IC
                                        * '<S5>/Stage7'
                                        */
  real32_T Stage7_RTP1COEFF_f[65];     /* Expression: a.SOSCoeffs
                                        * '<S5>/Stage7'
                                        */
  real32_T Stage7_RTP2COEFF_e[14];     /* Expression: a.ScaleValues
                                        * '<S5>/Stage7'
                                        */
  real32_T Stage8_ICRTP_g;             /* Expression: a.IC
                                        * '<S5>/Stage8'
                                        */
  real32_T Stage8_RTP1COEFF_c[60];     /* Expression: a.SOSCoeffs
                                        * '<S5>/Stage8'
                                        */
  real32_T Stage8_RTP2COEFF_cb[13];    /* Expression: a.ScaleValues
                                        * '<S5>/Stage8'
                                        */
  real32_T Stage9_ICRTP_a;             /* Expression: a.IC
                                        * '<S5>/Stage9'
                                        */
  real32_T Stage9_RTP1COEFF_g[60];     /* Expression: a.SOSCoeffs
                                        * '<S5>/Stage9'
                                        */
  real32_T Stage9_RTP2COEFF_b[13];     /* Expression: a.ScaleValues
                                        * '<S5>/Stage9'
                                        */
  real32_T Stage10_ICRTP_l;            /* Expression: a.IC
                                        * '<S5>/Stage10'
                                        */
  real32_T Stage10_RTP1COEFF_i[60];    /* Expression: a.SOSCoeffs
                                        * '<S5>/Stage10'
                                        */
  real32_T Stage10_RTP2COEFF_h[13];    /* Expression: a.ScaleValues
                                        * '<S5>/Stage10'
                                        */
  real32_T Stage1_ICRTP_n;             /* Expression: a.IC
                                        * '<S6>/Stage1'
                                        */
  real32_T Stage1_RTP1COEFF_h[65];     /* Expression: a.SOSCoeffs
                                        * '<S6>/Stage1'
                                        */
  real32_T Stage1_RTP2COEFF_l[14];     /* Expression: a.ScaleValues
                                        * '<S6>/Stage1'
                                        */
  real32_T Stage2_ICRTP_j;             /* Expression: a.IC
                                        * '<S6>/Stage2'
                                        */
  real32_T Stage2_RTP1COEFF_c[65];     /* Expression: a.SOSCoeffs
                                        * '<S6>/Stage2'
                                        */
  real32_T Stage2_RTP2COEFF_p[14];     /* Expression: a.ScaleValues
                                        * '<S6>/Stage2'
                                        */
  real32_T Stage3_ICRTP_c5;            /* Expression: a.IC
                                        * '<S6>/Stage3'
                                        */
  real32_T Stage3_RTP1COEFF_ja[65];    /* Expression: a.SOSCoeffs
                                        * '<S6>/Stage3'
                                        */
  real32_T Stage3_RTP2COEFF_h[14];     /* Expression: a.ScaleValues
                                        * '<S6>/Stage3'
                                        */
  real32_T Stage4_ICRTP_mz;            /* Expression: a.IC
                                        * '<S6>/Stage4'
                                        */
  real32_T Stage4_RTP1COEFF_a[65];     /* Expression: a.SOSCoeffs
                                        * '<S6>/Stage4'
                                        */
  real32_T Stage4_RTP2COEFF_c[14];     /* Expression: a.ScaleValues
                                        * '<S6>/Stage4'
                                        */
  real32_T Stage5_ICRTP_h;             /* Expression: a.IC
                                        * '<S6>/Stage5'
                                        */
  real32_T Stage5_RTP1COEFF_l[65];     /* Expression: a.SOSCoeffs
                                        * '<S6>/Stage5'
                                        */
  real32_T Stage5_RTP2COEFF_f[14];     /* Expression: a.ScaleValues
                                        * '<S6>/Stage5'
                                        */
  real32_T Stage6_ICRTP_e;             /* Expression: a.IC
                                        * '<S6>/Stage6'
                                        */
  real32_T Stage6_RTP1COEFF_a[65];     /* Expression: a.SOSCoeffs
                                        * '<S6>/Stage6'
                                        */
  real32_T Stage6_RTP2COEFF_b[14];     /* Expression: a.ScaleValues
                                        * '<S6>/Stage6'
                                        */
  real32_T Stage7_ICRTP_d;             /* Expression: a.IC
                                        * '<S6>/Stage7'
                                        */
  real32_T Stage7_RTP1COEFF_d[65];     /* Expression: a.SOSCoeffs
                                        * '<S6>/Stage7'
                                        */
  real32_T Stage7_RTP2COEFF_h[14];     /* Expression: a.ScaleValues
                                        * '<S6>/Stage7'
                                        */
  real32_T Stage8_ICRTP_e;             /* Expression: a.IC
                                        * '<S6>/Stage8'
                                        */
  real32_T Stage8_RTP1COEFF_d[60];     /* Expression: a.SOSCoeffs
                                        * '<S6>/Stage8'
                                        */
  real32_T Stage8_RTP2COEFF_g[13];     /* Expression: a.ScaleValues
                                        * '<S6>/Stage8'
                                        */
  real32_T Stage9_ICRTP_j;             /* Expression: a.IC
                                        * '<S6>/Stage9'
                                        */
  real32_T Stage9_RTP1COEFF_a[60];     /* Expression: a.SOSCoeffs
                                        * '<S6>/Stage9'
                                        */
  real32_T Stage9_RTP2COEFF_j[13];     /* Expression: a.ScaleValues
                                        * '<S6>/Stage9'
                                        */
  real32_T Stage10_ICRTP_j4;           /* Expression: a.IC
                                        * '<S6>/Stage10'
                                        */
  real32_T Stage10_RTP1COEFF_b[60];    /* Expression: a.SOSCoeffs
                                        * '<S6>/Stage10'
                                        */
  real32_T Stage10_RTP2COEFF_f[13];    /* Expression: a.ScaleValues
                                        * '<S6>/Stage10'
                                        */
  real32_T Stage1_ICRTP_jt;            /* Expression: a.IC
                                        * '<S7>/Stage1'
                                        */
  real32_T Stage1_RTP1COEFF_c[65];     /* Expression: a.SOSCoeffs
                                        * '<S7>/Stage1'
                                        */
  real32_T Stage1_RTP2COEFF_b[14];     /* Expression: a.ScaleValues
                                        * '<S7>/Stage1'
                                        */
  real32_T Stage2_ICRTP_d;             /* Expression: a.IC
                                        * '<S7>/Stage2'
                                        */
  real32_T Stage2_RTP1COEFF_g[65];     /* Expression: a.SOSCoeffs
                                        * '<S7>/Stage2'
                                        */
  real32_T Stage2_RTP2COEFF_f3[14];    /* Expression: a.ScaleValues
                                        * '<S7>/Stage2'
                                        */
  real32_T Stage3_ICRTP_m;             /* Expression: a.IC
                                        * '<S7>/Stage3'
                                        */
  real32_T Stage3_RTP1COEFF_a[65];     /* Expression: a.SOSCoeffs
                                        * '<S7>/Stage3'
                                        */
  real32_T Stage3_RTP2COEFF_m[14];     /* Expression: a.ScaleValues
                                        * '<S7>/Stage3'
                                        */
  real32_T Stage4_ICRTP_k;             /* Expression: a.IC
                                        * '<S7>/Stage4'
                                        */
  real32_T Stage4_RTP1COEFF_d3[65];    /* Expression: a.SOSCoeffs
                                        * '<S7>/Stage4'
                                        */
  real32_T Stage4_RTP2COEFF_o[14];     /* Expression: a.ScaleValues
                                        * '<S7>/Stage4'
                                        */
  real32_T Stage5_ICRTP_j;             /* Expression: a.IC
                                        * '<S7>/Stage5'
                                        */
  real32_T Stage5_RTP1COEFF_f[65];     /* Expression: a.SOSCoeffs
                                        * '<S7>/Stage5'
                                        */
  real32_T Stage5_RTP2COEFF_a[14];     /* Expression: a.ScaleValues
                                        * '<S7>/Stage5'
                                        */
  real32_T Stage6_ICRTP_j;             /* Expression: a.IC
                                        * '<S7>/Stage6'
                                        */
  real32_T Stage6_RTP1COEFF_k[65];     /* Expression: a.SOSCoeffs
                                        * '<S7>/Stage6'
                                        */
  real32_T Stage6_RTP2COEFF_e[14];     /* Expression: a.ScaleValues
                                        * '<S7>/Stage6'
                                        */
  real32_T Stage7_ICRTP_k;             /* Expression: a.IC
                                        * '<S7>/Stage7'
                                        */
  real32_T Stage7_RTP1COEFF_iw[65];    /* Expression: a.SOSCoeffs
                                        * '<S7>/Stage7'
                                        */
  real32_T Stage7_RTP2COEFF_g[14];     /* Expression: a.ScaleValues
                                        * '<S7>/Stage7'
                                        */
  real32_T Stage8_ICRTP_e5;            /* Expression: a.IC
                                        * '<S7>/Stage8'
                                        */
  real32_T Stage8_RTP1COEFF_j[60];     /* Expression: a.SOSCoeffs
                                        * '<S7>/Stage8'
                                        */
  real32_T Stage8_RTP2COEFF_d[13];     /* Expression: a.ScaleValues
                                        * '<S7>/Stage8'
                                        */
  real32_T Stage9_ICRTP_h;             /* Expression: a.IC
                                        * '<S7>/Stage9'
                                        */
  real32_T Stage9_RTP1COEFF_c[60];     /* Expression: a.SOSCoeffs
                                        * '<S7>/Stage9'
                                        */
  real32_T Stage9_RTP2COEFF_l[13];     /* Expression: a.ScaleValues
                                        * '<S7>/Stage9'
                                        */
  real32_T Stage10_ICRTP_p;            /* Expression: a.IC
                                        * '<S7>/Stage10'
                                        */
  real32_T Stage10_RTP1COEFF_n[60];    /* Expression: a.SOSCoeffs
                                        * '<S7>/Stage10'
                                        */
  real32_T Stage10_RTP2COEFF_e[13];    /* Expression: a.ScaleValues
                                        * '<S7>/Stage10'
                                        */
};

/* Real-time Model Data Structure */
struct RT_MODEL_Filter {
  const char_T * volatile errorStatus;
};

/* Block parameters (auto storage) */
extern Parameters_Filter Filter_P;

/* Block signals (auto storage) */
extern BlockIO_Filter Filter_B;

/* Block states (auto storage) */
extern D_Work_Filter Filter_DWork;

/* External inputs (root inport signals with auto storage) */
extern ExternalInputs_Filter Filter_U;

/* External outputs (root outports fed by signals with auto storage) */
extern ExternalOutputs_Filter Filter_Y;

/* Model entry point functions */
extern void Filter_initialize(void);
extern void Filter_step(void);
extern void Filter_terminate(void);

/* Real-time Model object */
extern RT_MODEL_Filter *Filter_M;

/*
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : Filter
 * '<S1>'   : Filter/MultiBandFilter1
 * '<S2>'   : Filter/MultiBandFilter2
 * '<S3>'   : Filter/MultiBandFilter3
 * '<S4>'   : Filter/MultiBandFilter4
 * '<S5>'   : Filter/MultiBandFilter5
 * '<S6>'   : Filter/MultiBandFilter6
 * '<S7>'   : Filter/MultiBandFilter7
 */
#endif                                 /* _RTW_HEADER_Filter_h_ */

/* File trailer for Real-Time Workshop generated code.
 *
 * [EOF]
 */
