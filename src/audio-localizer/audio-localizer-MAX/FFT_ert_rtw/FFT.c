/*
 * File: FFT.c
 *
 * Real-Time Workshop code generated for Simulink model FFT.
 *
 * Model version                        : 1.3
 * Real-Time Workshop file version      : 6.5  (R2006b Beta)  21-Sep-2006
 * Real-Time Workshop file generated on : Wed Oct  8 13:32:53 2008
 * TLC version                          : 6.5 (Sep 20 2006)
 * C source code generated on           : Wed Oct  8 13:32:54 2008
 */
#include "FFT.h"
#include "FFT_private.h"

/* Block signals (auto storage) */
BlockIO_FFT FFT_B;

/* External inputs (root inport signals with auto storage) */
ExternalInputs_FFT FFT_U;

/* External outputs (root outports fed by signals with auto storage) */
ExternalOutputs_FFT FFT_Y;

/* Real-time model */
RT_MODEL_FFT FFT_M_;
RT_MODEL_FFT *FFT_M = &FFT_M_;

/* Model step function */
void FFT_step(void)
{
  /* Signal Processing Blockset Pad (sdsppad) - '<Root>/Zero Pad' */
  /* Input dimensions: [8000 x 1], output dimensions: [8192 x 1] */
  {
    const real32_T *u = &FFT_U.audioIn[0];
    real32_T *y = FFT_B.ZeroPad;

    /* Copy current input column to current output column */
    memcpy(y, u, 8000 * sizeof(real32_T));
    y += 8000;

    /* Pad remaining samples for current output column */
    {
      int_T extraRowIdx = 192;
      while (extraRowIdx--) {
        *y++ = 0.0F;
      }
    }
  }

  /* Signal Processing Blockset FFT (sdspfft2) - '<Root>/FFT' */
  /* Real input, 1 channels, 8192 rows, linear output order */
  /* Interleave data to prepare for real-data algorithms: */
  MWDSP_FFTInterleave_BR_R(FFT_B.FFT_l, FFT_B.ZeroPad, 1, 8192);

  /* Apply half-length algorithm to single real signal: */
  {
    creal32_T *lastCol = FFT_B.FFT_l;  /* Point to last column of input */
    MWDSP_R2DIT_TBLS_C(lastCol, 1, 8192, 4096, &FFT_ConstP.FFT_TwiddleTable[0],
                       2, 0);          /* Radix-2 DIT FFT using TableSpeed twiddle computation */
    MWDSP_DblLen_TBL_C(lastCol, 8192, &FFT_ConstP.FFT_TwiddleTable[0], 1);
  }

  {
    int32_T i;
    for (i = 0; i < 8192; i++) {
      /* Outport: '<Root>/fftOut' */
      FFT_Y.fftOut[i].re = FFT_B.FFT_l[i].re;
      FFT_Y.fftOut[i].im = FFT_B.FFT_l[i].im;
    }
  }
}

/* Model initialize function */
void FFT_initialize(void)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatus(FFT_M, (const char_T *)0);
}

/* Model terminate function */
void FFT_terminate(void)
{
  /* (no terminate code required) */
}

/* File trailer for Real-Time Workshop generated code.
 *
 * [EOF]
 */
