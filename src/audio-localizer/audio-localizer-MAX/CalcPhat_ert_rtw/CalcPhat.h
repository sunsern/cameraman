/*
 * File: CalcPhat.h
 *
 * Real-Time Workshop code generated for Simulink model CalcPhat.
 *
 * Model version                        : 1.3
 * Real-Time Workshop file version      : 6.5  (R2006b Beta)  21-Sep-2006
 * Real-Time Workshop file generated on : Wed Oct  8 13:31:41 2008
 * TLC version                          : 6.5 (Sep 20 2006)
 * C source code generated on           : Wed Oct  8 13:31:42 2008
 */
#ifndef _RTW_HEADER_CalcPhat_h_
#define _RTW_HEADER_CalcPhat_h_
#ifndef _CalcPhat_COMMON_INCLUDES_
# define _CalcPhat_COMMON_INCLUDES_
#include <math.h>
#include <stddef.h>
#include "rtwtypes.h"
#include "rtlibsrc.h"
#endif                                 /* _CalcPhat_COMMON_INCLUDES_ */

#include "CalcPhat_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* Block signals (auto storage) */
typedef struct {
  creal32_T Prod[8192];                /* '<Root>/Prod' */
  creal32_T Divide[8192];              /* '<Root>/Divide' */
  real32_T IFFT[8192];                 /* '<Root>/IFFT' */
} BlockIO_CalcPhat;

/* External inputs (root inport signals with auto storage) */
typedef struct {
  creal32_T fftIn1[8192];              /* '<Root>/fftIn1' */
  creal32_T fftIn2[8192];              /* '<Root>/fftIn2' */
} ExternalInputs_CalcPhat;

/* External outputs (root outports fed by signals with auto storage) */
typedef struct {
  real32_T phatOut[401];               /* '<Root>/phatOut' */
} ExternalOutputs_CalcPhat;

/* Parameters (auto storage) */
struct Parameters_CalcPhat {
  real32_T IFFT_TwiddleTable[6144];    /* Computed Parameter: TwiddleTable
                                        * '<Root>/IFFT'
                                        */
};

/* Real-time Model Data Structure */
struct RT_MODEL_CalcPhat {
  const char_T * volatile errorStatus;
};

/* Block parameters (auto storage) */
extern Parameters_CalcPhat CalcPhat_P;

/* Block signals (auto storage) */
extern BlockIO_CalcPhat CalcPhat_B;

/* External inputs (root inport signals with auto storage) */
extern ExternalInputs_CalcPhat CalcPhat_U;

/* External outputs (root outports fed by signals with auto storage) */
extern ExternalOutputs_CalcPhat CalcPhat_Y;

/* Model entry point functions */
extern void CalcPhat_initialize(void);
extern void CalcPhat_step(void);
extern void CalcPhat_terminate(void);

/* Real-time Model object */
extern RT_MODEL_CalcPhat *CalcPhat_M;

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
 * '<Root>' : CalcPhat
 */
#endif                                 /* _RTW_HEADER_CalcPhat_h_ */

/* File trailer for Real-Time Workshop generated code.
 *
 * [EOF]
 */
