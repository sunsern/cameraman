/*
 * File: FFT.h
 *
 * Real-Time Workshop code generated for Simulink model FFT.
 *
 * Model version                        : 1.3
 * Real-Time Workshop file version      : 6.5  (R2006b Beta)  21-Sep-2006
 * Real-Time Workshop file generated on : Wed Oct  8 13:32:53 2008
 * TLC version                          : 6.5 (Sep 20 2006)
 * C source code generated on           : Wed Oct  8 13:32:54 2008
 */
#ifndef _RTW_HEADER_FFT_h_
#define _RTW_HEADER_FFT_h_
#ifndef _FFT_COMMON_INCLUDES_
# define _FFT_COMMON_INCLUDES_
#include <math.h>
#include <stddef.h>
#include "rtwtypes.h"
#endif                                 /* _FFT_COMMON_INCLUDES_ */

#include "FFT_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* Block signals (auto storage) */
typedef struct {
  creal32_T FFT_l[8192];               /* '<Root>/FFT' */
  real32_T ZeroPad[8192];              /* '<Root>/Zero Pad' */
} BlockIO_FFT;

/* Constant parameters (auto storage) */
typedef struct {
  /* Computed Parameter: TwiddleTable
   * '<Root>/FFT'
   */
  real32_T FFT_TwiddleTable[6144];
} ConstParam_FFT;

/* External inputs (root inport signals with auto storage) */
typedef struct {
  real32_T audioIn[8000];              /* '<Root>/audioIn' */
} ExternalInputs_FFT;

/* External outputs (root outports fed by signals with auto storage) */
typedef struct {
  creal32_T fftOut[8192];              /* '<Root>/fftOut' */
} ExternalOutputs_FFT;

/* Real-time Model Data Structure */
struct RT_MODEL_FFT {
  const char_T * volatile errorStatus;
};

/* Block signals (auto storage) */
extern BlockIO_FFT FFT_B;

/* External inputs (root inport signals with auto storage) */
extern ExternalInputs_FFT FFT_U;

/* External outputs (root outports fed by signals with auto storage) */
extern ExternalOutputs_FFT FFT_Y;

/* Constant parameters (auto storage) */
extern const ConstParam_FFT FFT_ConstP;

/* Model entry point functions */
extern void FFT_initialize(void);
extern void FFT_step(void);
extern void FFT_terminate(void);

/* Real-time Model object */
extern RT_MODEL_FFT *FFT_M;

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
 * '<Root>' : FFT
 */
#endif                                 /* _RTW_HEADER_FFT_h_ */

/* File trailer for Real-Time Workshop generated code.
 *
 * [EOF]
 */
