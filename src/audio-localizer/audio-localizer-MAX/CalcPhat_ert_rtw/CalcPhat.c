/*
 * File: CalcPhat.c
 *
 * Real-Time Workshop code generated for Simulink model CalcPhat.
 *
 * Model version                        : 1.3
 * Real-Time Workshop file version      : 6.5  (R2006b Beta)  21-Sep-2006
 * Real-Time Workshop file generated on : Wed Oct  8 13:31:41 2008
 * TLC version                          : 6.5 (Sep 20 2006)
 * C source code generated on           : Wed Oct  8 13:31:42 2008
 */
#include "CalcPhat.h"
#include "CalcPhat_private.h"

/* Block signals (auto storage) */
BlockIO_CalcPhat CalcPhat_B;

/* External inputs (root inport signals with auto storage) */
ExternalInputs_CalcPhat CalcPhat_U;

/* External outputs (root outports fed by signals with auto storage) */
ExternalOutputs_CalcPhat CalcPhat_Y;

/* Real-time model */
RT_MODEL_CalcPhat CalcPhat_M_;
RT_MODEL_CalcPhat *CalcPhat_M = &CalcPhat_M_;

/* Model step function */
void CalcPhat_step(void)
{
  /* Math: '<Root>/Math Function' incorporates:
   *   Inport: '<Root>/fftIn2'
   */                                  /* Operator : conj */
  {
    int_T i1;
    const creal32_T *u0 = &CalcPhat_U.fftIn2[0];
    creal32_T *y0 = CalcPhat_B.Divide;
    for (i1=0; i1 < 8192; i1++) {
      y0[i1].re = u0[i1].re;
      y0[i1].im = -u0[i1].im;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 8192; i++) {
      /* Product: '<Root>/Prod' incorporates:
       *  Inport: '<Root>/fftIn1'
       */
      CalcPhat_B.Prod[i].re = CalcPhat_U.fftIn1[i].re * CalcPhat_B.Divide[i].re
        - CalcPhat_U.fftIn1[i].im * CalcPhat_B.Divide[i].im;
      CalcPhat_B.Prod[i].im = CalcPhat_U.fftIn1[i].re * CalcPhat_B.Divide[i].im
        + CalcPhat_U.fftIn1[i].im * CalcPhat_B.Divide[i].re;
    }
  }

  /* Abs: '<Root>/Abs' */
  {
    int_T i1;
    const creal32_T *u0 = CalcPhat_B.Prod;
    real32_T *y0 = CalcPhat_B.IFFT;
    for (i1=0; i1 < 8192; i1++) {
      y0[i1] = (real32_T)sqrt( u0[i1].re*u0[i1].re + u0[i1].im*u0[i1].im );
    }
  }

  {
    creal32_T output;

    {
      int_T i1;
      const creal32_T *u0 = CalcPhat_B.Prod;
      const real32_T *u1 = CalcPhat_B.IFFT;
      creal32_T *y0 = CalcPhat_B.Divide;
      for (i1=0; i1 < 8192; i1++) {
        output = u0[i1];
        output.re = output.re / u1[i1];
        output.im = output.im / u1[i1];
        y0[i1] = output;
      }
    }
  }

  /* Signal Processing Blockset FFT (sdspfft2) - '<Root>/IFFT' */
  /* Complex input, real output,1 channels, 8192 rows, linear output order */
  /* Apply half-length algorithm to single real signal: */
  {
    const creal32_T *lastColIn = CalcPhat_B.Divide;/* Point to last column of input */
    real32_T *lastColOut = CalcPhat_B.IFFT;/* Point to last column of output */
    MWDSP_Ifft_DblLen_TBL_C_Cbr_Oop((creal32_T *)lastColOut, lastColIn, 8192,
      &CalcPhat_P.IFFT_TwiddleTable[0], 1);
    MWDSP_R2DIT_TBLS_C((creal32_T *)lastColOut, 1, 4096, 4096,
                       &CalcPhat_P.IFFT_TwiddleTable[0], 2, 1);/* Radix-2 DIT IFFT using TableSpeed twiddle computation */
  }

  MWDSP_ScaleData_RR(CalcPhat_B.IFFT, 8192, 1.0/8192);

  {
    int32_T i;

    /* Outport: '<Root>/phatOut' */
    for (i = 0; i < 201; i++) {
      CalcPhat_Y.phatOut[i] = CalcPhat_B.IFFT[i+7991];
    }

    for (i = 0; i < 200; i++) {
      CalcPhat_Y.phatOut[i + 201] = CalcPhat_B.IFFT[i];
    }
  }
}

/* Model initialize function */
void CalcPhat_initialize(void)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatus(CalcPhat_M, (const char_T *)0);

  /* block I/O */
  {
    int_T i;
    void *pVoidBlockIORegion;
    pVoidBlockIORegion = (void *)(&CalcPhat_B.Prod[0].re);
    for (i = 0; i < 40960; i++) {
      ((real32_T*)pVoidBlockIORegion)[i] = 0.0F;
    }
  }

  /* external inputs */
  {
    int_T i;
    for (i = 0; i < 8192; i++) {
      CalcPhat_U.fftIn1[i].re = 0.0F;
      CalcPhat_U.fftIn1[i].im = 0.0F;
    }
  }

  {
    int_T i;
    for (i = 0; i < 8192; i++) {
      CalcPhat_U.fftIn2[i].re = 0.0F;
      CalcPhat_U.fftIn2[i].im = 0.0F;
    }
  }

  /* external outputs */
  {
    int_T i;
    for (i = 0; i < 401; i++) {
      CalcPhat_Y.phatOut[i] = 0.0F;
    }
  }
}

/* Model terminate function */
void CalcPhat_terminate(void)
{
  /* (no terminate code required) */
}

/* File trailer for Real-Time Workshop generated code.
 *
 * [EOF]
 */
