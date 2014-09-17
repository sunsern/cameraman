/*
 * File: PHAT.h
 *
 * Real-Time Workshop code generated for Simulink model PHAT.
 *
 * Model version                        : 1.73
 * Real-Time Workshop file version      : 6.5  (R2006b Beta)  21-Sep-2006
 * Real-Time Workshop file generated on : Mon Jul 28 11:30:25 2008
 * TLC version                          : 6.5 (Sep 20 2006)
 * C source code generated on           : Mon Jul 28 11:30:26 2008
 */
#ifndef _RTW_HEADER_PHAT_h_
#define _RTW_HEADER_PHAT_h_
#ifndef _PHAT_COMMON_INCLUDES_
# define _PHAT_COMMON_INCLUDES_
#include <math.h>
#include <stddef.h>
#include "rtwtypes.h"
#include "rtlibsrc.h"
#endif                                 /* _PHAT_COMMON_INCLUDES_ */

#include "PHAT_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* Block signals (auto storage) */
typedef struct {
  creal32_T FFT[2048];                 /* '<Root>/FFT' */
  creal32_T FFT1[2048];                /* '<Root>/FFT1' */
  creal32_T FFT3[2048];                /* '<Root>/FFT3' */
  creal32_T FFT4[2048];                /* '<Root>/FFT4' */
  creal32_T FFT5[2048];                /* '<Root>/FFT5' */
  creal32_T MathFunction1[2048];       /* '<Root>/Math Function1' */
  creal32_T FFT2[2048];                /* '<Root>/FFT2' */
  creal32_T MathFunction2[2048];       /* '<Root>/Math Function2' */
  creal32_T MathFunction3[2048];       /* '<Root>/Math Function3' */
  creal32_T MathFunction4[2048];       /* '<Root>/Math Function4' */
  creal32_T MathFunction5[2048];       /* '<Root>/Math Function5' */
  creal32_T Divide20[2048];            /* '<Root>/Divide20' */
  creal32_T Assignment22[2048];        /* '<Root>/Assignment22' */
  real32_T MathFunction6[2048];        /* '<Root>/Math Function6' */
  real32_T IFFT20[2048];               /* '<Root>/IFFT20' */
} BlockIO_PHAT;

/* External inputs (root inport signals with auto storage) */
typedef struct {
  real32_T Channel1[1600];             /* '<Root>/Channel 1' */
  real32_T Channel2[1600];             /* '<Root>/Channel 2' */
  real32_T Channel3[1600];             /* '<Root>/Channel 3' */
  real32_T Channel4[1600];             /* '<Root>/Channel 4' */
  real32_T Channel5[1600];             /* '<Root>/Channel 5' */
  real32_T Channel6[1600];             /* '<Root>/Channel 6' */
  real32_T Channel7[1600];             /* '<Root>/Channel 7' */
} ExternalInputs_PHAT;

/* External outputs (root outports fed by signals with auto storage) */
typedef struct {
  real32_T R12[1001];                  /* '<Root>/R12' */
  real32_T R13[1001];                  /* '<Root>/R13' */
  real32_T R14[1001];                  /* '<Root>/R14' */
  real32_T R15[1001];                  /* '<Root>/R15' */
  real32_T R16[1001];                  /* '<Root>/R16' */
  real32_T R17[1001];                  /* '<Root>/R17' */
  real32_T R23[1001];                  /* '<Root>/R23' */
  real32_T R24[1001];                  /* '<Root>/R24' */
  real32_T R25[1001];                  /* '<Root>/R25' */
  real32_T R26[1001];                  /* '<Root>/R26' */
  real32_T R27[1001];                  /* '<Root>/R27' */
  real32_T R34[1001];                  /* '<Root>/R34' */
  real32_T R35[1001];                  /* '<Root>/R35' */
  real32_T R36[1001];                  /* '<Root>/R36' */
  real32_T R37[1001];                  /* '<Root>/R37' */
  real32_T R45[1001];                  /* '<Root>/R45' */
  real32_T R46[1001];                  /* '<Root>/R46' */
  real32_T R47[1001];                  /* '<Root>/R47' */
  real32_T R56[1001];                  /* '<Root>/R56' */
  real32_T R57[1001];                  /* '<Root>/R57' */
  real32_T R67[1001];                  /* '<Root>/R67' */
  real32_T Energy;                     /* '<Root>/Energy' */
} ExternalOutputs_PHAT;

/* Parameters (auto storage) */
struct Parameters_PHAT {
  real32_T ZeroPad_PadValue;           /* Computed Parameter: PadValue
                                        * '<Root>/Zero Pad'
                                        */
  real32_T FFT_TwiddleTable[1536];     /* Computed Parameter: TwiddleTable
                                        * '<Root>/FFT'
                                        */
  real32_T ZeroPad1_PadValue;          /* Computed Parameter: PadValue
                                        * '<Root>/Zero Pad1'
                                        */
  real32_T FFT1_TwiddleTable[1536];    /* Computed Parameter: TwiddleTable
                                        * '<Root>/FFT1'
                                        */
  real32_T Constant7_Value;            /* Computed Parameter: Value
                                        * '<Root>/Constant7'
                                        */
  real32_T IFFT_TwiddleTable[1536];    /* Computed Parameter: TwiddleTable
                                        * '<Root>/IFFT'
                                        */
  real32_T ZeroPad2_PadValue;          /* Computed Parameter: PadValue
                                        * '<Root>/Zero Pad2'
                                        */
  real32_T FFT2_TwiddleTable[1536];    /* Computed Parameter: TwiddleTable
                                        * '<Root>/FFT2'
                                        */
  real32_T Constant1_Value;            /* Computed Parameter: Value
                                        * '<Root>/Constant1'
                                        */
  real32_T IFFT1_TwiddleTable[1536];   /* Computed Parameter: TwiddleTable
                                        * '<Root>/IFFT1'
                                        */
  real32_T ZeroPad3_PadValue;          /* Computed Parameter: PadValue
                                        * '<Root>/Zero Pad3'
                                        */
  real32_T FFT3_TwiddleTable[1536];    /* Computed Parameter: TwiddleTable
                                        * '<Root>/FFT3'
                                        */
  real32_T Constant2_Value;            /* Computed Parameter: Value
                                        * '<Root>/Constant2'
                                        */
  real32_T IFFT2_TwiddleTable[1536];   /* Computed Parameter: TwiddleTable
                                        * '<Root>/IFFT2'
                                        */
  real32_T ZeroPad4_PadValue;          /* Computed Parameter: PadValue
                                        * '<Root>/Zero Pad4'
                                        */
  real32_T FFT4_TwiddleTable[1536];    /* Computed Parameter: TwiddleTable
                                        * '<Root>/FFT4'
                                        */
  real32_T Constant3_Value;            /* Computed Parameter: Value
                                        * '<Root>/Constant3'
                                        */
  real32_T IFFT3_TwiddleTable[1536];   /* Computed Parameter: TwiddleTable
                                        * '<Root>/IFFT3'
                                        */
  real32_T ZeroPad5_PadValue;          /* Computed Parameter: PadValue
                                        * '<Root>/Zero Pad5'
                                        */
  real32_T FFT5_TwiddleTable[1536];    /* Computed Parameter: TwiddleTable
                                        * '<Root>/FFT5'
                                        */
  real32_T Constant4_Value;            /* Computed Parameter: Value
                                        * '<Root>/Constant4'
                                        */
  real32_T IFFT4_TwiddleTable[1536];   /* Computed Parameter: TwiddleTable
                                        * '<Root>/IFFT4'
                                        */
  real32_T ZeroPad6_PadValue;          /* Computed Parameter: PadValue
                                        * '<Root>/Zero Pad6'
                                        */
  real32_T FFT6_TwiddleTable[1536];    /* Computed Parameter: TwiddleTable
                                        * '<Root>/FFT6'
                                        */
  real32_T Constant5_Value;            /* Computed Parameter: Value
                                        * '<Root>/Constant5'
                                        */
  real32_T IFFT5_TwiddleTable[1536];   /* Computed Parameter: TwiddleTable
                                        * '<Root>/IFFT5'
                                        */
  real32_T Constant6_Value;            /* Computed Parameter: Value
                                        * '<Root>/Constant6'
                                        */
  real32_T IFFT6_TwiddleTable[1536];   /* Computed Parameter: TwiddleTable
                                        * '<Root>/IFFT6'
                                        */
  real32_T Constant8_Value;            /* Computed Parameter: Value
                                        * '<Root>/Constant8'
                                        */
  real32_T IFFT7_TwiddleTable[1536];   /* Computed Parameter: TwiddleTable
                                        * '<Root>/IFFT7'
                                        */
  real32_T Constant9_Value;            /* Computed Parameter: Value
                                        * '<Root>/Constant9'
                                        */
  real32_T IFFT8_TwiddleTable[1536];   /* Computed Parameter: TwiddleTable
                                        * '<Root>/IFFT8'
                                        */
  real32_T Constant10_Value;           /* Computed Parameter: Value
                                        * '<Root>/Constant10'
                                        */
  real32_T IFFT9_TwiddleTable[1536];   /* Computed Parameter: TwiddleTable
                                        * '<Root>/IFFT9'
                                        */
  real32_T Constant11_Value;           /* Computed Parameter: Value
                                        * '<Root>/Constant11'
                                        */
  real32_T IFFT10_TwiddleTable[1536];  /* Computed Parameter: TwiddleTable
                                        * '<Root>/IFFT10'
                                        */
  real32_T Constant12_Value;           /* Computed Parameter: Value
                                        * '<Root>/Constant12'
                                        */
  real32_T IFFT11_TwiddleTable[1536];  /* Computed Parameter: TwiddleTable
                                        * '<Root>/IFFT11'
                                        */
  real32_T Constant13_Value;           /* Computed Parameter: Value
                                        * '<Root>/Constant13'
                                        */
  real32_T IFFT12_TwiddleTable[1536];  /* Computed Parameter: TwiddleTable
                                        * '<Root>/IFFT12'
                                        */
  real32_T Constant14_Value;           /* Computed Parameter: Value
                                        * '<Root>/Constant14'
                                        */
  real32_T IFFT13_TwiddleTable[1536];  /* Computed Parameter: TwiddleTable
                                        * '<Root>/IFFT13'
                                        */
  real32_T Constant15_Value;           /* Computed Parameter: Value
                                        * '<Root>/Constant15'
                                        */
  real32_T IFFT14_TwiddleTable[1536];  /* Computed Parameter: TwiddleTable
                                        * '<Root>/IFFT14'
                                        */
  real32_T Constant16_Value;           /* Computed Parameter: Value
                                        * '<Root>/Constant16'
                                        */
  real32_T IFFT15_TwiddleTable[1536];  /* Computed Parameter: TwiddleTable
                                        * '<Root>/IFFT15'
                                        */
  real32_T Constant17_Value;           /* Computed Parameter: Value
                                        * '<Root>/Constant17'
                                        */
  real32_T IFFT16_TwiddleTable[1536];  /* Computed Parameter: TwiddleTable
                                        * '<Root>/IFFT16'
                                        */
  real32_T Constant18_Value;           /* Computed Parameter: Value
                                        * '<Root>/Constant18'
                                        */
  real32_T IFFT17_TwiddleTable[1536];  /* Computed Parameter: TwiddleTable
                                        * '<Root>/IFFT17'
                                        */
  real32_T Constant19_Value;           /* Computed Parameter: Value
                                        * '<Root>/Constant19'
                                        */
  real32_T IFFT18_TwiddleTable[1536];  /* Computed Parameter: TwiddleTable
                                        * '<Root>/IFFT18'
                                        */
  real32_T Constant20_Value;           /* Computed Parameter: Value
                                        * '<Root>/Constant20'
                                        */
  real32_T IFFT19_TwiddleTable[1536];  /* Computed Parameter: TwiddleTable
                                        * '<Root>/IFFT19'
                                        */
  real32_T Constant21_Value;           /* Computed Parameter: Value
                                        * '<Root>/Constant21'
                                        */
  real32_T IFFT20_TwiddleTable[1536];  /* Computed Parameter: TwiddleTable
                                        * '<Root>/IFFT20'
                                        */
  real32_T Constant22_Value;           /* Computed Parameter: Value
                                        * '<Root>/Constant22'
                                        */
};

/* Real-time Model Data Structure */
struct RT_MODEL_PHAT {
  const char_T * volatile errorStatus;
};

/* Block parameters (auto storage) */
extern Parameters_PHAT PHAT_P;

/* Block signals (auto storage) */
extern BlockIO_PHAT PHAT_B;

/* External inputs (root inport signals with auto storage) */
extern ExternalInputs_PHAT PHAT_U;

/* External outputs (root outports fed by signals with auto storage) */
extern ExternalOutputs_PHAT PHAT_Y;

/* Model entry point functions */
extern void PHAT_initialize(void);
extern void PHAT_step(void);
extern void PHAT_terminate(void);

/* Real-time Model object */
extern RT_MODEL_PHAT *PHAT_M;

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
 * '<Root>' : PHAT
 */
#endif                                 /* _RTW_HEADER_PHAT_h_ */

/* File trailer for Real-Time Workshop generated code.
 *
 * [EOF]
 */
