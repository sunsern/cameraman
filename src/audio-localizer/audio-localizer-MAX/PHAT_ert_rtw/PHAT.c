/*
 * File: PHAT.c
 *
 * Real-Time Workshop code generated for Simulink model PHAT.
 *
 * Model version                        : 1.73
 * Real-Time Workshop file version      : 6.5  (R2006b Beta)  21-Sep-2006
 * Real-Time Workshop file generated on : Mon Jul 28 11:30:25 2008
 * TLC version                          : 6.5 (Sep 20 2006)
 * C source code generated on           : Mon Jul 28 11:30:26 2008
 */
#include "PHAT.h"
#include "PHAT_private.h"

/* Block signals (auto storage) */
BlockIO_PHAT PHAT_B;

/* External inputs (root inport signals with auto storage) */
ExternalInputs_PHAT PHAT_U;

/* External outputs (root outports fed by signals with auto storage) */
ExternalOutputs_PHAT PHAT_Y;

/* Real-time model */
RT_MODEL_PHAT PHAT_M_;
RT_MODEL_PHAT *PHAT_M = &PHAT_M_;

/* Model step function */
void PHAT_step(void)
{
  /* Signal Processing Blockset Pad (sdsppad) - '<Root>/Zero Pad' */
  /* Input dimensions: [1600 x 1], output dimensions: [2048 x 1] */
  {
    const real32_T *u = &PHAT_U.Channel1[0];
    real32_T *y = PHAT_B.IFFT20;

    /* Copy current input column to current output column */
    memcpy(y, u, 1600 * sizeof(real32_T));
    y += 1600;

    /* Pad remaining samples for current output column */
    {
      int_T extraRowIdx = 448;
      while (extraRowIdx--) {
        *y++ = PHAT_P.ZeroPad_PadValue;
      }
    }
  }

  /* Signal Processing Blockset FFT (sdspfft2) - '<Root>/FFT' */
  /* Real input, 1 channels, 2048 rows, linear output order */
  /* Interleave data to prepare for real-data algorithms: */
  MWDSP_FFTInterleave_BR_R(PHAT_B.FFT, PHAT_B.IFFT20, 1, 2048);

  /* Apply half-length algorithm to single real signal: */
  {
    creal32_T *lastCol = PHAT_B.FFT;   /* Point to last column of input */
    MWDSP_R2DIT_TBLS_C(lastCol, 1, 2048, 1024, &PHAT_P.FFT_TwiddleTable[0], 2, 0
                       );              /* Radix-2 DIT FFT using TableSpeed twiddle computation */
    MWDSP_DblLen_TBL_C(lastCol, 2048, &PHAT_P.FFT_TwiddleTable[0], 1);
  }

  /* Signal Processing Blockset Pad (sdsppad) - '<Root>/Zero Pad1' */
  /* Input dimensions: [1600 x 1], output dimensions: [2048 x 1] */
  {
    const real32_T *u = &PHAT_U.Channel2[0];
    real32_T *y = PHAT_B.IFFT20;

    /* Copy current input column to current output column */
    memcpy(y, u, 1600 * sizeof(real32_T));
    y += 1600;

    /* Pad remaining samples for current output column */
    {
      int_T extraRowIdx = 448;
      while (extraRowIdx--) {
        *y++ = PHAT_P.ZeroPad1_PadValue;
      }
    }
  }

  /* Signal Processing Blockset FFT (sdspfft2) - '<Root>/FFT1' */
  /* Real input, 1 channels, 2048 rows, linear output order */
  /* Interleave data to prepare for real-data algorithms: */
  MWDSP_FFTInterleave_BR_R(PHAT_B.FFT1, PHAT_B.IFFT20, 1, 2048);

  /* Apply half-length algorithm to single real signal: */
  {
    creal32_T *lastCol = PHAT_B.FFT1;  /* Point to last column of input */
    MWDSP_R2DIT_TBLS_C(lastCol, 1, 2048, 1024, &PHAT_P.FFT1_TwiddleTable[0], 2,
                       0);             /* Radix-2 DIT FFT using TableSpeed twiddle computation */
    MWDSP_DblLen_TBL_C(lastCol, 2048, &PHAT_P.FFT1_TwiddleTable[0], 1);
  }

  /* Math: '<Root>/Math Function' */   /* Operator : conj */
  {
    int_T i1;
    const creal32_T *u0 = PHAT_B.FFT1;
    creal32_T *y0 = PHAT_B.MathFunction1;
    for (i1=0; i1 < 2048; i1++) {
      y0[i1].re = u0[i1].re;
      y0[i1].im = -u0[i1].im;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 2048; i++) {
      /* Product: '<Root>/P12' */
      PHAT_B.FFT2[i].re = PHAT_B.FFT[i].re * PHAT_B.MathFunction1[i].re -
        PHAT_B.FFT[i].im * PHAT_B.MathFunction1[i].im;
      PHAT_B.FFT2[i].im = PHAT_B.FFT[i].re * PHAT_B.MathFunction1[i].im +
        PHAT_B.FFT[i].im * PHAT_B.MathFunction1[i].re;
    }
  }

  /* Abs: '<Root>/Abs' */
  {
    int_T i1;
    const creal32_T *u0 = PHAT_B.FFT2;
    real32_T *y0 = PHAT_B.IFFT20;
    for (i1=0; i1 < 2048; i1++) {
      y0[i1] = (real32_T)sqrt( u0[i1].re*u0[i1].re + u0[i1].im*u0[i1].im );
    }
  }

  {
    creal32_T output;

    {
      int_T i1;
      const creal32_T *u0 = PHAT_B.FFT2;
      const real32_T *u1 = PHAT_B.IFFT20;
      creal32_T *y0 = PHAT_B.MathFunction1;
      for (i1=0; i1 < 2048; i1++) {
        output = u0[i1];
        output.re = output.re / u1[i1];
        output.im = output.im / u1[i1];
        y0[i1] = output;
      }
    }
  }

  {
    int32_T dimIdxEls[126] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
      , 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
      34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52
      , 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 1985, 1986, 1987, 1988, 1989
      , 1990, 1991, 1992, 1993, 1994, 1995, 1996, 1997, 1998, 1999, 2000, 2001,
      2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013,
      2014, 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024, 2025,
      2026, 2027, 2028, 2029, 2030, 2031, 2032, 2033, 2034, 2035, 2036, 2037,
      2038, 2039, 2040, 2041, 2042, 2043, 2044, 2045, 2046, 2047 };

    int32_T i;

    /* Assignment: '<Root>/Assignment7' incorporates:
     *  Constant: '<Root>/Constant7'
     */
    for (i = 0; i < 2048; i++) {
      PHAT_B.FFT2[i].re = PHAT_B.MathFunction1[i].re;
      PHAT_B.FFT2[i].im = PHAT_B.MathFunction1[i].im;
    }

    for (i = 0; i < 126; i++) {
      PHAT_B.FFT2[dimIdxEls[i]].re = PHAT_P.Constant7_Value;
      PHAT_B.FFT2[dimIdxEls[i]].im = 0.0F;
    }
  }

  /* Signal Processing Blockset FFT (sdspfft2) - '<Root>/IFFT' */
  /* Complex input, real output,1 channels, 2048 rows, linear output order */
  /* Apply half-length algorithm to single real signal: */
  {
    const creal32_T *lastColIn = PHAT_B.FFT2;/* Point to last column of input */
    real32_T *lastColOut = PHAT_B.IFFT20;/* Point to last column of output */
    MWDSP_Ifft_DblLen_TBL_C_Cbr_Oop((creal32_T *)lastColOut, lastColIn, 2048,
      &PHAT_P.IFFT_TwiddleTable[0], 1);
    MWDSP_R2DIT_TBLS_C((creal32_T *)lastColOut, 1, 1024, 1024,
                       &PHAT_P.IFFT_TwiddleTable[0], 2, 1);/* Radix-2 DIT IFFT using TableSpeed twiddle computation */
  }

  MWDSP_ScaleData_RR(PHAT_B.IFFT20, 2048, 1.0/2048);

  {
    int32_T i;

    /* Outport: '<Root>/R12' */
    for (i = 0; i < 501; i++) {
      PHAT_Y.R12[i] = PHAT_B.IFFT20[i+1547];
    }

    for (i = 0; i < 500; i++) {
      PHAT_Y.R12[i + 501] = PHAT_B.IFFT20[i];
    }
  }

  /* Signal Processing Blockset Pad (sdsppad) - '<Root>/Zero Pad2' */
  /* Input dimensions: [1600 x 1], output dimensions: [2048 x 1] */
  {
    const real32_T *u = &PHAT_U.Channel3[0];
    real32_T *y = PHAT_B.IFFT20;

    /* Copy current input column to current output column */
    memcpy(y, u, 1600 * sizeof(real32_T));
    y += 1600;

    /* Pad remaining samples for current output column */
    {
      int_T extraRowIdx = 448;
      while (extraRowIdx--) {
        *y++ = PHAT_P.ZeroPad2_PadValue;
      }
    }
  }

  /* Signal Processing Blockset FFT (sdspfft2) - '<Root>/FFT2' */
  /* Real input, 1 channels, 2048 rows, linear output order */
  /* Interleave data to prepare for real-data algorithms: */
  MWDSP_FFTInterleave_BR_R(PHAT_B.FFT2, PHAT_B.IFFT20, 1, 2048);

  /* Apply half-length algorithm to single real signal: */
  {
    creal32_T *lastCol = PHAT_B.FFT2;  /* Point to last column of input */
    MWDSP_R2DIT_TBLS_C(lastCol, 1, 2048, 1024, &PHAT_P.FFT2_TwiddleTable[0], 2,
                       0);             /* Radix-2 DIT FFT using TableSpeed twiddle computation */
    MWDSP_DblLen_TBL_C(lastCol, 2048, &PHAT_P.FFT2_TwiddleTable[0], 1);
  }

  /* Math: '<Root>/Math Function1' */  /* Operator : conj */
  {
    int_T i1;
    const creal32_T *u0 = PHAT_B.FFT2;
    creal32_T *y0 = PHAT_B.MathFunction1;
    for (i1=0; i1 < 2048; i1++) {
      y0[i1].re = u0[i1].re;
      y0[i1].im = -u0[i1].im;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 2048; i++) {
      /* Product: '<Root>/P13' */
      PHAT_B.MathFunction2[i].re = PHAT_B.FFT[i].re * PHAT_B.MathFunction1[i].re
        - PHAT_B.FFT[i].im * PHAT_B.MathFunction1[i].im;
      PHAT_B.MathFunction2[i].im = PHAT_B.FFT[i].re * PHAT_B.MathFunction1[i].im
        + PHAT_B.FFT[i].im * PHAT_B.MathFunction1[i].re;
    }
  }

  /* Abs: '<Root>/Abs1' */
  {
    int_T i1;
    const creal32_T *u0 = PHAT_B.MathFunction2;
    real32_T *y0 = PHAT_B.IFFT20;
    for (i1=0; i1 < 2048; i1++) {
      y0[i1] = (real32_T)sqrt( u0[i1].re*u0[i1].re + u0[i1].im*u0[i1].im );
    }
  }

  {
    creal32_T output;

    {
      int_T i1;
      const creal32_T *u0 = PHAT_B.MathFunction2;
      const real32_T *u1 = PHAT_B.IFFT20;
      creal32_T *y0 = PHAT_B.FFT3;
      for (i1=0; i1 < 2048; i1++) {
        output = u0[i1];
        output.re = output.re / u1[i1];
        output.im = output.im / u1[i1];
        y0[i1] = output;
      }
    }
  }

  {
    int32_T dimIdxEls[126] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
      , 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
      34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52
      , 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 1985, 1986, 1987, 1988, 1989
      , 1990, 1991, 1992, 1993, 1994, 1995, 1996, 1997, 1998, 1999, 2000, 2001,
      2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013,
      2014, 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024, 2025,
      2026, 2027, 2028, 2029, 2030, 2031, 2032, 2033, 2034, 2035, 2036, 2037,
      2038, 2039, 2040, 2041, 2042, 2043, 2044, 2045, 2046, 2047 };

    int32_T i;

    /* Assignment: '<Root>/Assignment1' incorporates:
     *  Constant: '<Root>/Constant1'
     */
    for (i = 0; i < 2048; i++) {
      PHAT_B.MathFunction2[i].re = PHAT_B.FFT3[i].re;
      PHAT_B.MathFunction2[i].im = PHAT_B.FFT3[i].im;
    }

    for (i = 0; i < 126; i++) {
      PHAT_B.MathFunction2[dimIdxEls[i]].re = PHAT_P.Constant1_Value;
      PHAT_B.MathFunction2[dimIdxEls[i]].im = 0.0F;
    }
  }

  /* Signal Processing Blockset FFT (sdspfft2) - '<Root>/IFFT1' */
  /* Complex input, real output,1 channels, 2048 rows, linear output order */
  /* Apply half-length algorithm to single real signal: */
  {
    const creal32_T *lastColIn = PHAT_B.MathFunction2;/* Point to last column of input */
    real32_T *lastColOut = PHAT_B.IFFT20;/* Point to last column of output */
    MWDSP_Ifft_DblLen_TBL_C_Cbr_Oop((creal32_T *)lastColOut, lastColIn, 2048,
      &PHAT_P.IFFT1_TwiddleTable[0], 1);
    MWDSP_R2DIT_TBLS_C((creal32_T *)lastColOut, 1, 1024, 1024,
                       &PHAT_P.IFFT1_TwiddleTable[0], 2, 1);/* Radix-2 DIT IFFT using TableSpeed twiddle computation */
  }

  MWDSP_ScaleData_RR(PHAT_B.IFFT20, 2048, 1.0/2048);

  {
    int32_T i;

    /* Outport: '<Root>/R13' */
    for (i = 0; i < 501; i++) {
      PHAT_Y.R13[i] = PHAT_B.IFFT20[i+1547];
    }

    for (i = 0; i < 500; i++) {
      PHAT_Y.R13[i + 501] = PHAT_B.IFFT20[i];
    }
  }

  /* Signal Processing Blockset Pad (sdsppad) - '<Root>/Zero Pad3' */
  /* Input dimensions: [1600 x 1], output dimensions: [2048 x 1] */
  {
    const real32_T *u = &PHAT_U.Channel4[0];
    real32_T *y = PHAT_B.IFFT20;

    /* Copy current input column to current output column */
    memcpy(y, u, 1600 * sizeof(real32_T));
    y += 1600;

    /* Pad remaining samples for current output column */
    {
      int_T extraRowIdx = 448;
      while (extraRowIdx--) {
        *y++ = PHAT_P.ZeroPad3_PadValue;
      }
    }
  }

  /* Signal Processing Blockset FFT (sdspfft2) - '<Root>/FFT3' */
  /* Real input, 1 channels, 2048 rows, linear output order */
  /* Interleave data to prepare for real-data algorithms: */
  MWDSP_FFTInterleave_BR_R(PHAT_B.FFT3, PHAT_B.IFFT20, 1, 2048);

  /* Apply half-length algorithm to single real signal: */
  {
    creal32_T *lastCol = PHAT_B.FFT3;  /* Point to last column of input */
    MWDSP_R2DIT_TBLS_C(lastCol, 1, 2048, 1024, &PHAT_P.FFT3_TwiddleTable[0], 2,
                       0);             /* Radix-2 DIT FFT using TableSpeed twiddle computation */
    MWDSP_DblLen_TBL_C(lastCol, 2048, &PHAT_P.FFT3_TwiddleTable[0], 1);
  }

  /* Math: '<Root>/Math Function2' */  /* Operator : conj */
  {
    int_T i1;
    const creal32_T *u0 = PHAT_B.FFT3;
    creal32_T *y0 = PHAT_B.MathFunction2;
    for (i1=0; i1 < 2048; i1++) {
      y0[i1].re = u0[i1].re;
      y0[i1].im = -u0[i1].im;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 2048; i++) {
      /* Product: '<Root>/P14' */
      PHAT_B.MathFunction3[i].re = PHAT_B.FFT[i].re * PHAT_B.MathFunction2[i].re
        - PHAT_B.FFT[i].im * PHAT_B.MathFunction2[i].im;
      PHAT_B.MathFunction3[i].im = PHAT_B.FFT[i].re * PHAT_B.MathFunction2[i].im
        + PHAT_B.FFT[i].im * PHAT_B.MathFunction2[i].re;
    }
  }

  /* Abs: '<Root>/Abs2' */
  {
    int_T i1;
    const creal32_T *u0 = PHAT_B.MathFunction3;
    real32_T *y0 = PHAT_B.IFFT20;
    for (i1=0; i1 < 2048; i1++) {
      y0[i1] = (real32_T)sqrt( u0[i1].re*u0[i1].re + u0[i1].im*u0[i1].im );
    }
  }

  {
    creal32_T output;

    {
      int_T i1;
      const creal32_T *u0 = PHAT_B.MathFunction3;
      const real32_T *u1 = PHAT_B.IFFT20;
      creal32_T *y0 = PHAT_B.FFT4;
      for (i1=0; i1 < 2048; i1++) {
        output = u0[i1];
        output.re = output.re / u1[i1];
        output.im = output.im / u1[i1];
        y0[i1] = output;
      }
    }
  }

  {
    int32_T dimIdxEls[126] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
      , 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
      34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52
      , 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 1985, 1986, 1987, 1988, 1989
      , 1990, 1991, 1992, 1993, 1994, 1995, 1996, 1997, 1998, 1999, 2000, 2001,
      2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013,
      2014, 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024, 2025,
      2026, 2027, 2028, 2029, 2030, 2031, 2032, 2033, 2034, 2035, 2036, 2037,
      2038, 2039, 2040, 2041, 2042, 2043, 2044, 2045, 2046, 2047 };

    int32_T i;

    /* Assignment: '<Root>/Assignment2' incorporates:
     *  Constant: '<Root>/Constant2'
     */
    for (i = 0; i < 2048; i++) {
      PHAT_B.MathFunction3[i].re = PHAT_B.FFT4[i].re;
      PHAT_B.MathFunction3[i].im = PHAT_B.FFT4[i].im;
    }

    for (i = 0; i < 126; i++) {
      PHAT_B.MathFunction3[dimIdxEls[i]].re = PHAT_P.Constant2_Value;
      PHAT_B.MathFunction3[dimIdxEls[i]].im = 0.0F;
    }
  }

  /* Signal Processing Blockset FFT (sdspfft2) - '<Root>/IFFT2' */
  /* Complex input, real output,1 channels, 2048 rows, linear output order */
  /* Apply half-length algorithm to single real signal: */
  {
    const creal32_T *lastColIn = PHAT_B.MathFunction3;/* Point to last column of input */
    real32_T *lastColOut = PHAT_B.IFFT20;/* Point to last column of output */
    MWDSP_Ifft_DblLen_TBL_C_Cbr_Oop((creal32_T *)lastColOut, lastColIn, 2048,
      &PHAT_P.IFFT2_TwiddleTable[0], 1);
    MWDSP_R2DIT_TBLS_C((creal32_T *)lastColOut, 1, 1024, 1024,
                       &PHAT_P.IFFT2_TwiddleTable[0], 2, 1);/* Radix-2 DIT IFFT using TableSpeed twiddle computation */
  }

  MWDSP_ScaleData_RR(PHAT_B.IFFT20, 2048, 1.0/2048);

  {
    int32_T i;

    /* Outport: '<Root>/R14' */
    for (i = 0; i < 501; i++) {
      PHAT_Y.R14[i] = PHAT_B.IFFT20[i+1547];
    }

    for (i = 0; i < 500; i++) {
      PHAT_Y.R14[i + 501] = PHAT_B.IFFT20[i];
    }
  }

  /* Signal Processing Blockset Pad (sdsppad) - '<Root>/Zero Pad4' */
  /* Input dimensions: [1600 x 1], output dimensions: [2048 x 1] */
  {
    const real32_T *u = &PHAT_U.Channel5[0];
    real32_T *y = PHAT_B.IFFT20;

    /* Copy current input column to current output column */
    memcpy(y, u, 1600 * sizeof(real32_T));
    y += 1600;

    /* Pad remaining samples for current output column */
    {
      int_T extraRowIdx = 448;
      while (extraRowIdx--) {
        *y++ = PHAT_P.ZeroPad4_PadValue;
      }
    }
  }

  /* Signal Processing Blockset FFT (sdspfft2) - '<Root>/FFT4' */
  /* Real input, 1 channels, 2048 rows, linear output order */
  /* Interleave data to prepare for real-data algorithms: */
  MWDSP_FFTInterleave_BR_R(PHAT_B.FFT4, PHAT_B.IFFT20, 1, 2048);

  /* Apply half-length algorithm to single real signal: */
  {
    creal32_T *lastCol = PHAT_B.FFT4;  /* Point to last column of input */
    MWDSP_R2DIT_TBLS_C(lastCol, 1, 2048, 1024, &PHAT_P.FFT4_TwiddleTable[0], 2,
                       0);             /* Radix-2 DIT FFT using TableSpeed twiddle computation */
    MWDSP_DblLen_TBL_C(lastCol, 2048, &PHAT_P.FFT4_TwiddleTable[0], 1);
  }

  /* Math: '<Root>/Math Function3' */  /* Operator : conj */
  {
    int_T i1;
    const creal32_T *u0 = PHAT_B.FFT4;
    creal32_T *y0 = PHAT_B.MathFunction3;
    for (i1=0; i1 < 2048; i1++) {
      y0[i1].re = u0[i1].re;
      y0[i1].im = -u0[i1].im;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 2048; i++) {
      /* Product: '<Root>/P15' */
      PHAT_B.MathFunction4[i].re = PHAT_B.FFT[i].re * PHAT_B.MathFunction3[i].re
        - PHAT_B.FFT[i].im * PHAT_B.MathFunction3[i].im;
      PHAT_B.MathFunction4[i].im = PHAT_B.FFT[i].re * PHAT_B.MathFunction3[i].im
        + PHAT_B.FFT[i].im * PHAT_B.MathFunction3[i].re;
    }
  }

  /* Abs: '<Root>/Abs3' */
  {
    int_T i1;
    const creal32_T *u0 = PHAT_B.MathFunction4;
    real32_T *y0 = PHAT_B.IFFT20;
    for (i1=0; i1 < 2048; i1++) {
      y0[i1] = (real32_T)sqrt( u0[i1].re*u0[i1].re + u0[i1].im*u0[i1].im );
    }
  }

  {
    creal32_T output;

    {
      int_T i1;
      const creal32_T *u0 = PHAT_B.MathFunction4;
      const real32_T *u1 = PHAT_B.IFFT20;
      creal32_T *y0 = PHAT_B.FFT5;
      for (i1=0; i1 < 2048; i1++) {
        output = u0[i1];
        output.re = output.re / u1[i1];
        output.im = output.im / u1[i1];
        y0[i1] = output;
      }
    }
  }

  {
    int32_T dimIdxEls[126] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
      , 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
      34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52
      , 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 1985, 1986, 1987, 1988, 1989
      , 1990, 1991, 1992, 1993, 1994, 1995, 1996, 1997, 1998, 1999, 2000, 2001,
      2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013,
      2014, 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024, 2025,
      2026, 2027, 2028, 2029, 2030, 2031, 2032, 2033, 2034, 2035, 2036, 2037,
      2038, 2039, 2040, 2041, 2042, 2043, 2044, 2045, 2046, 2047 };

    int32_T i;

    /* Assignment: '<Root>/Assignment3' incorporates:
     *  Constant: '<Root>/Constant3'
     */
    for (i = 0; i < 2048; i++) {
      PHAT_B.MathFunction4[i].re = PHAT_B.FFT5[i].re;
      PHAT_B.MathFunction4[i].im = PHAT_B.FFT5[i].im;
    }

    for (i = 0; i < 126; i++) {
      PHAT_B.MathFunction4[dimIdxEls[i]].re = PHAT_P.Constant3_Value;
      PHAT_B.MathFunction4[dimIdxEls[i]].im = 0.0F;
    }
  }

  /* Signal Processing Blockset FFT (sdspfft2) - '<Root>/IFFT3' */
  /* Complex input, real output,1 channels, 2048 rows, linear output order */
  /* Apply half-length algorithm to single real signal: */
  {
    const creal32_T *lastColIn = PHAT_B.MathFunction4;/* Point to last column of input */
    real32_T *lastColOut = PHAT_B.IFFT20;/* Point to last column of output */
    MWDSP_Ifft_DblLen_TBL_C_Cbr_Oop((creal32_T *)lastColOut, lastColIn, 2048,
      &PHAT_P.IFFT3_TwiddleTable[0], 1);
    MWDSP_R2DIT_TBLS_C((creal32_T *)lastColOut, 1, 1024, 1024,
                       &PHAT_P.IFFT3_TwiddleTable[0], 2, 1);/* Radix-2 DIT IFFT using TableSpeed twiddle computation */
  }

  MWDSP_ScaleData_RR(PHAT_B.IFFT20, 2048, 1.0/2048);

  {
    int32_T i;

    /* Outport: '<Root>/R15' */
    for (i = 0; i < 501; i++) {
      PHAT_Y.R15[i] = PHAT_B.IFFT20[i+1547];
    }

    for (i = 0; i < 500; i++) {
      PHAT_Y.R15[i + 501] = PHAT_B.IFFT20[i];
    }
  }

  /* Signal Processing Blockset Pad (sdsppad) - '<Root>/Zero Pad5' */
  /* Input dimensions: [1600 x 1], output dimensions: [2048 x 1] */
  {
    const real32_T *u = &PHAT_U.Channel6[0];
    real32_T *y = PHAT_B.IFFT20;

    /* Copy current input column to current output column */
    memcpy(y, u, 1600 * sizeof(real32_T));
    y += 1600;

    /* Pad remaining samples for current output column */
    {
      int_T extraRowIdx = 448;
      while (extraRowIdx--) {
        *y++ = PHAT_P.ZeroPad5_PadValue;
      }
    }
  }

  /* Signal Processing Blockset FFT (sdspfft2) - '<Root>/FFT5' */
  /* Real input, 1 channels, 2048 rows, linear output order */
  /* Interleave data to prepare for real-data algorithms: */
  MWDSP_FFTInterleave_BR_R(PHAT_B.FFT5, PHAT_B.IFFT20, 1, 2048);

  /* Apply half-length algorithm to single real signal: */
  {
    creal32_T *lastCol = PHAT_B.FFT5;  /* Point to last column of input */
    MWDSP_R2DIT_TBLS_C(lastCol, 1, 2048, 1024, &PHAT_P.FFT5_TwiddleTable[0], 2,
                       0);             /* Radix-2 DIT FFT using TableSpeed twiddle computation */
    MWDSP_DblLen_TBL_C(lastCol, 2048, &PHAT_P.FFT5_TwiddleTable[0], 1);
  }

  /* Math: '<Root>/Math Function4' */  /* Operator : conj */
  {
    int_T i1;
    const creal32_T *u0 = PHAT_B.FFT5;
    creal32_T *y0 = PHAT_B.MathFunction4;
    for (i1=0; i1 < 2048; i1++) {
      y0[i1].re = u0[i1].re;
      y0[i1].im = -u0[i1].im;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 2048; i++) {
      /* Product: '<Root>/P16' */
      PHAT_B.MathFunction5[i].re = PHAT_B.FFT[i].re * PHAT_B.MathFunction4[i].re
        - PHAT_B.FFT[i].im * PHAT_B.MathFunction4[i].im;
      PHAT_B.MathFunction5[i].im = PHAT_B.FFT[i].re * PHAT_B.MathFunction4[i].im
        + PHAT_B.FFT[i].im * PHAT_B.MathFunction4[i].re;
    }
  }

  /* Abs: '<Root>/Abs4' */
  {
    int_T i1;
    const creal32_T *u0 = PHAT_B.MathFunction5;
    real32_T *y0 = PHAT_B.IFFT20;
    for (i1=0; i1 < 2048; i1++) {
      y0[i1] = (real32_T)sqrt( u0[i1].re*u0[i1].re + u0[i1].im*u0[i1].im );
    }
  }

  {
    creal32_T output;

    {
      int_T i1;
      const creal32_T *u0 = PHAT_B.MathFunction5;
      const real32_T *u1 = PHAT_B.IFFT20;
      creal32_T *y0 = PHAT_B.Divide20;
      for (i1=0; i1 < 2048; i1++) {
        output = u0[i1];
        output.re = output.re / u1[i1];
        output.im = output.im / u1[i1];
        y0[i1] = output;
      }
    }
  }

  {
    int32_T dimIdxEls[126] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
      , 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
      34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52
      , 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 1985, 1986, 1987, 1988, 1989
      , 1990, 1991, 1992, 1993, 1994, 1995, 1996, 1997, 1998, 1999, 2000, 2001,
      2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013,
      2014, 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024, 2025,
      2026, 2027, 2028, 2029, 2030, 2031, 2032, 2033, 2034, 2035, 2036, 2037,
      2038, 2039, 2040, 2041, 2042, 2043, 2044, 2045, 2046, 2047 };

    int32_T i;

    /* Assignment: '<Root>/Assignment4' incorporates:
     *  Constant: '<Root>/Constant4'
     */
    for (i = 0; i < 2048; i++) {
      PHAT_B.MathFunction5[i].re = PHAT_B.Divide20[i].re;
      PHAT_B.MathFunction5[i].im = PHAT_B.Divide20[i].im;
    }

    for (i = 0; i < 126; i++) {
      PHAT_B.MathFunction5[dimIdxEls[i]].re = PHAT_P.Constant4_Value;
      PHAT_B.MathFunction5[dimIdxEls[i]].im = 0.0F;
    }
  }

  /* Signal Processing Blockset FFT (sdspfft2) - '<Root>/IFFT4' */
  /* Complex input, real output,1 channels, 2048 rows, linear output order */
  /* Apply half-length algorithm to single real signal: */
  {
    const creal32_T *lastColIn = PHAT_B.MathFunction5;/* Point to last column of input */
    real32_T *lastColOut = PHAT_B.IFFT20;/* Point to last column of output */
    MWDSP_Ifft_DblLen_TBL_C_Cbr_Oop((creal32_T *)lastColOut, lastColIn, 2048,
      &PHAT_P.IFFT4_TwiddleTable[0], 1);
    MWDSP_R2DIT_TBLS_C((creal32_T *)lastColOut, 1, 1024, 1024,
                       &PHAT_P.IFFT4_TwiddleTable[0], 2, 1);/* Radix-2 DIT IFFT using TableSpeed twiddle computation */
  }

  MWDSP_ScaleData_RR(PHAT_B.IFFT20, 2048, 1.0/2048);

  {
    int32_T i;

    /* Outport: '<Root>/R16' */
    for (i = 0; i < 501; i++) {
      PHAT_Y.R16[i] = PHAT_B.IFFT20[i+1547];
    }

    for (i = 0; i < 500; i++) {
      PHAT_Y.R16[i + 501] = PHAT_B.IFFT20[i];
    }
  }

  /* Signal Processing Blockset Pad (sdsppad) - '<Root>/Zero Pad6' */
  /* Input dimensions: [1600 x 1], output dimensions: [2048 x 1] */
  {
    const real32_T *u = &PHAT_U.Channel7[0];
    real32_T *y = PHAT_B.IFFT20;

    /* Copy current input column to current output column */
    memcpy(y, u, 1600 * sizeof(real32_T));
    y += 1600;

    /* Pad remaining samples for current output column */
    {
      int_T extraRowIdx = 448;
      while (extraRowIdx--) {
        *y++ = PHAT_P.ZeroPad6_PadValue;
      }
    }
  }

  /* Signal Processing Blockset FFT (sdspfft2) - '<Root>/FFT6' */
  /* Real input, 1 channels, 2048 rows, linear output order */
  /* Interleave data to prepare for real-data algorithms: */
  MWDSP_FFTInterleave_BR_R(PHAT_B.Divide20, PHAT_B.IFFT20, 1, 2048);

  /* Apply half-length algorithm to single real signal: */
  {
    creal32_T *lastCol = PHAT_B.Divide20;/* Point to last column of input */
    MWDSP_R2DIT_TBLS_C(lastCol, 1, 2048, 1024, &PHAT_P.FFT6_TwiddleTable[0], 2,
                       0);             /* Radix-2 DIT FFT using TableSpeed twiddle computation */
    MWDSP_DblLen_TBL_C(lastCol, 2048, &PHAT_P.FFT6_TwiddleTable[0], 1);
  }

  /* Math: '<Root>/Math Function5' */  /* Operator : conj */
  {
    int_T i1;
    const creal32_T *u0 = PHAT_B.Divide20;
    creal32_T *y0 = PHAT_B.MathFunction5;
    for (i1=0; i1 < 2048; i1++) {
      y0[i1].re = u0[i1].re;
      y0[i1].im = -u0[i1].im;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 2048; i++) {
      /* Product: '<Root>/P17' */
      PHAT_B.Divide20[i].re = PHAT_B.FFT[i].re * PHAT_B.MathFunction5[i].re -
        PHAT_B.FFT[i].im * PHAT_B.MathFunction5[i].im;
      PHAT_B.Divide20[i].im = PHAT_B.FFT[i].re * PHAT_B.MathFunction5[i].im +
        PHAT_B.FFT[i].im * PHAT_B.MathFunction5[i].re;
    }
  }

  /* Abs: '<Root>/Abs5' */
  {
    int_T i1;
    const creal32_T *u0 = PHAT_B.Divide20;
    real32_T *y0 = PHAT_B.IFFT20;
    for (i1=0; i1 < 2048; i1++) {
      y0[i1] = (real32_T)sqrt( u0[i1].re*u0[i1].re + u0[i1].im*u0[i1].im );
    }
  }

  {
    creal32_T output;

    {
      int_T i1;
      const creal32_T *u0 = PHAT_B.Divide20;
      const real32_T *u1 = PHAT_B.IFFT20;
      creal32_T *y0 = PHAT_B.Assignment22;
      for (i1=0; i1 < 2048; i1++) {
        output = u0[i1];
        output.re = output.re / u1[i1];
        output.im = output.im / u1[i1];
        y0[i1] = output;
      }
    }
  }

  {
    int32_T dimIdxEls[126] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
      , 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
      34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52
      , 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 1985, 1986, 1987, 1988, 1989
      , 1990, 1991, 1992, 1993, 1994, 1995, 1996, 1997, 1998, 1999, 2000, 2001,
      2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013,
      2014, 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024, 2025,
      2026, 2027, 2028, 2029, 2030, 2031, 2032, 2033, 2034, 2035, 2036, 2037,
      2038, 2039, 2040, 2041, 2042, 2043, 2044, 2045, 2046, 2047 };

    int32_T i;

    /* Assignment: '<Root>/Assignment5' incorporates:
     *  Constant: '<Root>/Constant5'
     */
    for (i = 0; i < 2048; i++) {
      PHAT_B.Divide20[i].re = PHAT_B.Assignment22[i].re;
      PHAT_B.Divide20[i].im = PHAT_B.Assignment22[i].im;
    }

    for (i = 0; i < 126; i++) {
      PHAT_B.Divide20[dimIdxEls[i]].re = PHAT_P.Constant5_Value;
      PHAT_B.Divide20[dimIdxEls[i]].im = 0.0F;
    }
  }

  /* Signal Processing Blockset FFT (sdspfft2) - '<Root>/IFFT5' */
  /* Complex input, real output,1 channels, 2048 rows, linear output order */
  /* Apply half-length algorithm to single real signal: */
  {
    const creal32_T *lastColIn = PHAT_B.Divide20;/* Point to last column of input */
    real32_T *lastColOut = PHAT_B.IFFT20;/* Point to last column of output */
    MWDSP_Ifft_DblLen_TBL_C_Cbr_Oop((creal32_T *)lastColOut, lastColIn, 2048,
      &PHAT_P.IFFT5_TwiddleTable[0], 1);
    MWDSP_R2DIT_TBLS_C((creal32_T *)lastColOut, 1, 1024, 1024,
                       &PHAT_P.IFFT5_TwiddleTable[0], 2, 1);/* Radix-2 DIT IFFT using TableSpeed twiddle computation */
  }

  MWDSP_ScaleData_RR(PHAT_B.IFFT20, 2048, 1.0/2048);

  {
    int32_T i;

    /* Outport: '<Root>/R17' */
    for (i = 0; i < 501; i++) {
      PHAT_Y.R17[i] = PHAT_B.IFFT20[i+1547];
    }

    for (i = 0; i < 500; i++) {
      PHAT_Y.R17[i + 501] = PHAT_B.IFFT20[i];
    }

    for (i = 0; i < 2048; i++) {
      /* Product: '<Root>/P23' */
      PHAT_B.Assignment22[i].re = PHAT_B.FFT1[i].re * PHAT_B.MathFunction1[i].re
        - PHAT_B.FFT1[i].im * PHAT_B.MathFunction1[i].im;
      PHAT_B.Assignment22[i].im = PHAT_B.FFT1[i].re * PHAT_B.MathFunction1[i].im
        + PHAT_B.FFT1[i].im * PHAT_B.MathFunction1[i].re;
    }
  }

  /* Abs: '<Root>/Abs6' */
  {
    int_T i1;
    const creal32_T *u0 = PHAT_B.Assignment22;
    real32_T *y0 = PHAT_B.IFFT20;
    for (i1=0; i1 < 2048; i1++) {
      y0[i1] = (real32_T)sqrt( u0[i1].re*u0[i1].re + u0[i1].im*u0[i1].im );
    }
  }

  {
    creal32_T output;

    {
      int_T i1;
      const creal32_T *u0 = PHAT_B.Assignment22;
      const real32_T *u1 = PHAT_B.IFFT20;
      creal32_T *y0 = PHAT_B.Divide20;
      for (i1=0; i1 < 2048; i1++) {
        output = u0[i1];
        output.re = output.re / u1[i1];
        output.im = output.im / u1[i1];
        y0[i1] = output;
      }
    }
  }

  {
    int32_T dimIdxEls[126] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
      , 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
      34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52
      , 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 1985, 1986, 1987, 1988, 1989
      , 1990, 1991, 1992, 1993, 1994, 1995, 1996, 1997, 1998, 1999, 2000, 2001,
      2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013,
      2014, 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024, 2025,
      2026, 2027, 2028, 2029, 2030, 2031, 2032, 2033, 2034, 2035, 2036, 2037,
      2038, 2039, 2040, 2041, 2042, 2043, 2044, 2045, 2046, 2047 };

    int32_T i;

    /* Assignment: '<Root>/Assignment6' incorporates:
     *  Constant: '<Root>/Constant6'
     */
    for (i = 0; i < 2048; i++) {
      PHAT_B.Assignment22[i].re = PHAT_B.Divide20[i].re;
      PHAT_B.Assignment22[i].im = PHAT_B.Divide20[i].im;
    }

    for (i = 0; i < 126; i++) {
      PHAT_B.Assignment22[dimIdxEls[i]].re = PHAT_P.Constant6_Value;
      PHAT_B.Assignment22[dimIdxEls[i]].im = 0.0F;
    }
  }

  /* Signal Processing Blockset FFT (sdspfft2) - '<Root>/IFFT6' */
  /* Complex input, real output,1 channels, 2048 rows, linear output order */
  /* Apply half-length algorithm to single real signal: */
  {
    const creal32_T *lastColIn = PHAT_B.Assignment22;/* Point to last column of input */
    real32_T *lastColOut = PHAT_B.IFFT20;/* Point to last column of output */
    MWDSP_Ifft_DblLen_TBL_C_Cbr_Oop((creal32_T *)lastColOut, lastColIn, 2048,
      &PHAT_P.IFFT6_TwiddleTable[0], 1);
    MWDSP_R2DIT_TBLS_C((creal32_T *)lastColOut, 1, 1024, 1024,
                       &PHAT_P.IFFT6_TwiddleTable[0], 2, 1);/* Radix-2 DIT IFFT using TableSpeed twiddle computation */
  }

  MWDSP_ScaleData_RR(PHAT_B.IFFT20, 2048, 1.0/2048);

  {
    int32_T i;

    /* Outport: '<Root>/R23' */
    for (i = 0; i < 501; i++) {
      PHAT_Y.R23[i] = PHAT_B.IFFT20[i+1547];
    }

    for (i = 0; i < 500; i++) {
      PHAT_Y.R23[i + 501] = PHAT_B.IFFT20[i];
    }

    for (i = 0; i < 2048; i++) {
      /* Product: '<Root>/P24' */
      PHAT_B.Assignment22[i].re = PHAT_B.FFT1[i].re * PHAT_B.MathFunction2[i].re
        - PHAT_B.FFT1[i].im * PHAT_B.MathFunction2[i].im;
      PHAT_B.Assignment22[i].im = PHAT_B.FFT1[i].re * PHAT_B.MathFunction2[i].im
        + PHAT_B.FFT1[i].im * PHAT_B.MathFunction2[i].re;
    }
  }

  /* Abs: '<Root>/Abs7' */
  {
    int_T i1;
    const creal32_T *u0 = PHAT_B.Assignment22;
    real32_T *y0 = PHAT_B.IFFT20;
    for (i1=0; i1 < 2048; i1++) {
      y0[i1] = (real32_T)sqrt( u0[i1].re*u0[i1].re + u0[i1].im*u0[i1].im );
    }
  }

  {
    creal32_T output;

    {
      int_T i1;
      const creal32_T *u0 = PHAT_B.Assignment22;
      const real32_T *u1 = PHAT_B.IFFT20;
      creal32_T *y0 = PHAT_B.Divide20;
      for (i1=0; i1 < 2048; i1++) {
        output = u0[i1];
        output.re = output.re / u1[i1];
        output.im = output.im / u1[i1];
        y0[i1] = output;
      }
    }
  }

  {
    int32_T dimIdxEls0_0_73[126] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
      14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32
      , 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
      51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 1985, 1986, 1987, 1988
      , 1989, 1990, 1991, 1992, 1993, 1994, 1995, 1996, 1997, 1998, 1999, 2000,
      2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012,
      2013, 2014, 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024,
      2025, 2026, 2027, 2028, 2029, 2030, 2031, 2032, 2033, 2034, 2035, 2036,
      2037, 2038, 2039, 2040, 2041, 2042, 2043, 2044, 2045, 2046, 2047 };

    int32_T i;

    /* Assignment: '<Root>/Assignment8' incorporates:
     *  Constant: '<Root>/Constant8'
     */
    for (i = 0; i < 2048; i++) {
      PHAT_B.Assignment22[i].re = PHAT_B.Divide20[i].re;
      PHAT_B.Assignment22[i].im = PHAT_B.Divide20[i].im;
    }

    for (i = 0; i < 126; i++) {
      PHAT_B.Assignment22[dimIdxEls0_0_73[i]].re = PHAT_P.Constant8_Value;
      PHAT_B.Assignment22[dimIdxEls0_0_73[i]].im = 0.0F;
    }
  }

  /* Signal Processing Blockset FFT (sdspfft2) - '<Root>/IFFT7' */
  /* Complex input, real output,1 channels, 2048 rows, linear output order */
  /* Apply half-length algorithm to single real signal: */
  {
    const creal32_T *lastColIn = PHAT_B.Assignment22;/* Point to last column of input */
    real32_T *lastColOut = PHAT_B.IFFT20;/* Point to last column of output */
    MWDSP_Ifft_DblLen_TBL_C_Cbr_Oop((creal32_T *)lastColOut, lastColIn, 2048,
      &PHAT_P.IFFT7_TwiddleTable[0], 1);
    MWDSP_R2DIT_TBLS_C((creal32_T *)lastColOut, 1, 1024, 1024,
                       &PHAT_P.IFFT7_TwiddleTable[0], 2, 1);/* Radix-2 DIT IFFT using TableSpeed twiddle computation */
  }

  MWDSP_ScaleData_RR(PHAT_B.IFFT20, 2048, 1.0/2048);

  {
    int32_T i;

    /* Outport: '<Root>/R24' */
    for (i = 0; i < 501; i++) {
      PHAT_Y.R24[i] = PHAT_B.IFFT20[i+1547];
    }

    for (i = 0; i < 500; i++) {
      PHAT_Y.R24[i + 501] = PHAT_B.IFFT20[i];
    }

    for (i = 0; i < 2048; i++) {
      /* Product: '<Root>/P25' */
      PHAT_B.Assignment22[i].re = PHAT_B.FFT1[i].re * PHAT_B.MathFunction3[i].re
        - PHAT_B.FFT1[i].im * PHAT_B.MathFunction3[i].im;
      PHAT_B.Assignment22[i].im = PHAT_B.FFT1[i].re * PHAT_B.MathFunction3[i].im
        + PHAT_B.FFT1[i].im * PHAT_B.MathFunction3[i].re;
    }
  }

  /* Abs: '<Root>/Abs8' */
  {
    int_T i1;
    const creal32_T *u0 = PHAT_B.Assignment22;
    real32_T *y0 = PHAT_B.IFFT20;
    for (i1=0; i1 < 2048; i1++) {
      y0[i1] = (real32_T)sqrt( u0[i1].re*u0[i1].re + u0[i1].im*u0[i1].im );
    }
  }

  {
    creal32_T output;

    {
      int_T i1;
      const creal32_T *u0 = PHAT_B.Assignment22;
      const real32_T *u1 = PHAT_B.IFFT20;
      creal32_T *y0 = PHAT_B.Divide20;
      for (i1=0; i1 < 2048; i1++) {
        output = u0[i1];
        output.re = output.re / u1[i1];
        output.im = output.im / u1[i1];
        y0[i1] = output;
      }
    }
  }

  {
    int32_T dimIdxEls0_0_80[126] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
      14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32
      , 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
      51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 1985, 1986, 1987, 1988
      , 1989, 1990, 1991, 1992, 1993, 1994, 1995, 1996, 1997, 1998, 1999, 2000,
      2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012,
      2013, 2014, 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024,
      2025, 2026, 2027, 2028, 2029, 2030, 2031, 2032, 2033, 2034, 2035, 2036,
      2037, 2038, 2039, 2040, 2041, 2042, 2043, 2044, 2045, 2046, 2047 };

    int32_T i;

    /* Assignment: '<Root>/Assignment9' incorporates:
     *  Constant: '<Root>/Constant9'
     */
    for (i = 0; i < 2048; i++) {
      PHAT_B.Assignment22[i].re = PHAT_B.Divide20[i].re;
      PHAT_B.Assignment22[i].im = PHAT_B.Divide20[i].im;
    }

    for (i = 0; i < 126; i++) {
      PHAT_B.Assignment22[dimIdxEls0_0_80[i]].re = PHAT_P.Constant9_Value;
      PHAT_B.Assignment22[dimIdxEls0_0_80[i]].im = 0.0F;
    }
  }

  /* Signal Processing Blockset FFT (sdspfft2) - '<Root>/IFFT8' */
  /* Complex input, real output,1 channels, 2048 rows, linear output order */
  /* Apply half-length algorithm to single real signal: */
  {
    const creal32_T *lastColIn = PHAT_B.Assignment22;/* Point to last column of input */
    real32_T *lastColOut = PHAT_B.IFFT20;/* Point to last column of output */
    MWDSP_Ifft_DblLen_TBL_C_Cbr_Oop((creal32_T *)lastColOut, lastColIn, 2048,
      &PHAT_P.IFFT8_TwiddleTable[0], 1);
    MWDSP_R2DIT_TBLS_C((creal32_T *)lastColOut, 1, 1024, 1024,
                       &PHAT_P.IFFT8_TwiddleTable[0], 2, 1);/* Radix-2 DIT IFFT using TableSpeed twiddle computation */
  }

  MWDSP_ScaleData_RR(PHAT_B.IFFT20, 2048, 1.0/2048);

  {
    int32_T i;

    /* Outport: '<Root>/R25' */
    for (i = 0; i < 501; i++) {
      PHAT_Y.R25[i] = PHAT_B.IFFT20[i+1547];
    }

    for (i = 0; i < 500; i++) {
      PHAT_Y.R25[i + 501] = PHAT_B.IFFT20[i];
    }

    for (i = 0; i < 2048; i++) {
      /* Product: '<Root>/P26' */
      PHAT_B.Assignment22[i].re = PHAT_B.FFT1[i].re * PHAT_B.MathFunction4[i].re
        - PHAT_B.FFT1[i].im * PHAT_B.MathFunction4[i].im;
      PHAT_B.Assignment22[i].im = PHAT_B.FFT1[i].re * PHAT_B.MathFunction4[i].im
        + PHAT_B.FFT1[i].im * PHAT_B.MathFunction4[i].re;
    }
  }

  /* Abs: '<Root>/Abs9' */
  {
    int_T i1;
    const creal32_T *u0 = PHAT_B.Assignment22;
    real32_T *y0 = PHAT_B.IFFT20;
    for (i1=0; i1 < 2048; i1++) {
      y0[i1] = (real32_T)sqrt( u0[i1].re*u0[i1].re + u0[i1].im*u0[i1].im );
    }
  }

  {
    creal32_T output;

    {
      int_T i1;
      const creal32_T *u0 = PHAT_B.Assignment22;
      const real32_T *u1 = PHAT_B.IFFT20;
      creal32_T *y0 = PHAT_B.Divide20;
      for (i1=0; i1 < 2048; i1++) {
        output = u0[i1];
        output.re = output.re / u1[i1];
        output.im = output.im / u1[i1];
        y0[i1] = output;
      }
    }
  }

  {
    int32_T dimIdxEls[126] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
      , 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
      34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52
      , 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 1985, 1986, 1987, 1988, 1989
      , 1990, 1991, 1992, 1993, 1994, 1995, 1996, 1997, 1998, 1999, 2000, 2001,
      2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013,
      2014, 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024, 2025,
      2026, 2027, 2028, 2029, 2030, 2031, 2032, 2033, 2034, 2035, 2036, 2037,
      2038, 2039, 2040, 2041, 2042, 2043, 2044, 2045, 2046, 2047 };

    int32_T i;

    /* Assignment: '<Root>/Assignment10' incorporates:
     *  Constant: '<Root>/Constant10'
     */
    for (i = 0; i < 2048; i++) {
      PHAT_B.Assignment22[i].re = PHAT_B.Divide20[i].re;
      PHAT_B.Assignment22[i].im = PHAT_B.Divide20[i].im;
    }

    for (i = 0; i < 126; i++) {
      PHAT_B.Assignment22[dimIdxEls[i]].re = PHAT_P.Constant10_Value;
      PHAT_B.Assignment22[dimIdxEls[i]].im = 0.0F;
    }
  }

  /* Signal Processing Blockset FFT (sdspfft2) - '<Root>/IFFT9' */
  /* Complex input, real output,1 channels, 2048 rows, linear output order */
  /* Apply half-length algorithm to single real signal: */
  {
    const creal32_T *lastColIn = PHAT_B.Assignment22;/* Point to last column of input */
    real32_T *lastColOut = PHAT_B.IFFT20;/* Point to last column of output */
    MWDSP_Ifft_DblLen_TBL_C_Cbr_Oop((creal32_T *)lastColOut, lastColIn, 2048,
      &PHAT_P.IFFT9_TwiddleTable[0], 1);
    MWDSP_R2DIT_TBLS_C((creal32_T *)lastColOut, 1, 1024, 1024,
                       &PHAT_P.IFFT9_TwiddleTable[0], 2, 1);/* Radix-2 DIT IFFT using TableSpeed twiddle computation */
  }

  MWDSP_ScaleData_RR(PHAT_B.IFFT20, 2048, 1.0/2048);

  {
    int32_T i;

    /* Outport: '<Root>/R26' */
    for (i = 0; i < 501; i++) {
      PHAT_Y.R26[i] = PHAT_B.IFFT20[i+1547];
    }

    for (i = 0; i < 500; i++) {
      PHAT_Y.R26[i + 501] = PHAT_B.IFFT20[i];
    }

    for (i = 0; i < 2048; i++) {
      /* Product: '<Root>/P27' */
      PHAT_B.Assignment22[i].re = PHAT_B.FFT1[i].re * PHAT_B.MathFunction5[i].re
        - PHAT_B.FFT1[i].im * PHAT_B.MathFunction5[i].im;
      PHAT_B.Assignment22[i].im = PHAT_B.FFT1[i].re * PHAT_B.MathFunction5[i].im
        + PHAT_B.FFT1[i].im * PHAT_B.MathFunction5[i].re;
    }
  }

  /* Abs: '<Root>/Abs10' */
  {
    int_T i1;
    const creal32_T *u0 = PHAT_B.Assignment22;
    real32_T *y0 = PHAT_B.IFFT20;
    for (i1=0; i1 < 2048; i1++) {
      y0[i1] = (real32_T)sqrt( u0[i1].re*u0[i1].re + u0[i1].im*u0[i1].im );
    }
  }

  {
    creal32_T output;

    {
      int_T i1;
      const creal32_T *u0 = PHAT_B.Assignment22;
      const real32_T *u1 = PHAT_B.IFFT20;
      creal32_T *y0 = PHAT_B.Divide20;
      for (i1=0; i1 < 2048; i1++) {
        output = u0[i1];
        output.re = output.re / u1[i1];
        output.im = output.im / u1[i1];
        y0[i1] = output;
      }
    }
  }

  {
    int32_T dimIdxEls0_0_94[126] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
      14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32
      , 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
      51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 1985, 1986, 1987, 1988
      , 1989, 1990, 1991, 1992, 1993, 1994, 1995, 1996, 1997, 1998, 1999, 2000,
      2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012,
      2013, 2014, 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024,
      2025, 2026, 2027, 2028, 2029, 2030, 2031, 2032, 2033, 2034, 2035, 2036,
      2037, 2038, 2039, 2040, 2041, 2042, 2043, 2044, 2045, 2046, 2047 };

    int32_T i;

    /* Assignment: '<Root>/Assignment11' incorporates:
     *  Constant: '<Root>/Constant11'
     */
    for (i = 0; i < 2048; i++) {
      PHAT_B.Assignment22[i].re = PHAT_B.Divide20[i].re;
      PHAT_B.Assignment22[i].im = PHAT_B.Divide20[i].im;
    }

    for (i = 0; i < 126; i++) {
      PHAT_B.Assignment22[dimIdxEls0_0_94[i]].re = PHAT_P.Constant11_Value;
      PHAT_B.Assignment22[dimIdxEls0_0_94[i]].im = 0.0F;
    }
  }

  /* Signal Processing Blockset FFT (sdspfft2) - '<Root>/IFFT10' */
  /* Complex input, real output,1 channels, 2048 rows, linear output order */
  /* Apply half-length algorithm to single real signal: */
  {
    const creal32_T *lastColIn = PHAT_B.Assignment22;/* Point to last column of input */
    real32_T *lastColOut = PHAT_B.IFFT20;/* Point to last column of output */
    MWDSP_Ifft_DblLen_TBL_C_Cbr_Oop((creal32_T *)lastColOut, lastColIn, 2048,
      &PHAT_P.IFFT10_TwiddleTable[0], 1);
    MWDSP_R2DIT_TBLS_C((creal32_T *)lastColOut, 1, 1024, 1024,
                       &PHAT_P.IFFT10_TwiddleTable[0], 2, 1);/* Radix-2 DIT IFFT using TableSpeed twiddle computation */
  }

  MWDSP_ScaleData_RR(PHAT_B.IFFT20, 2048, 1.0/2048);

  {
    int32_T i;

    /* Outport: '<Root>/R27' */
    for (i = 0; i < 501; i++) {
      PHAT_Y.R27[i] = PHAT_B.IFFT20[i+1547];
    }

    for (i = 0; i < 500; i++) {
      PHAT_Y.R27[i + 501] = PHAT_B.IFFT20[i];
    }

    for (i = 0; i < 2048; i++) {
      /* Product: '<Root>/P34' */
      PHAT_B.Assignment22[i].re = PHAT_B.FFT2[i].re * PHAT_B.MathFunction2[i].re
        - PHAT_B.FFT2[i].im * PHAT_B.MathFunction2[i].im;
      PHAT_B.Assignment22[i].im = PHAT_B.FFT2[i].re * PHAT_B.MathFunction2[i].im
        + PHAT_B.FFT2[i].im * PHAT_B.MathFunction2[i].re;
    }
  }

  /* Abs: '<Root>/Abs11' */
  {
    int_T i1;
    const creal32_T *u0 = PHAT_B.Assignment22;
    real32_T *y0 = PHAT_B.IFFT20;
    for (i1=0; i1 < 2048; i1++) {
      y0[i1] = (real32_T)sqrt( u0[i1].re*u0[i1].re + u0[i1].im*u0[i1].im );
    }
  }

  {
    creal32_T output;

    {
      int_T i1;
      const creal32_T *u0 = PHAT_B.Assignment22;
      const real32_T *u1 = PHAT_B.IFFT20;
      creal32_T *y0 = PHAT_B.Divide20;
      for (i1=0; i1 < 2048; i1++) {
        output = u0[i1];
        output.re = output.re / u1[i1];
        output.im = output.im / u1[i1];
        y0[i1] = output;
      }
    }
  }

  {
    int32_T dimIdxEls0_0_101[126] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
      14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32
      , 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
      51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 1985, 1986, 1987, 1988
      , 1989, 1990, 1991, 1992, 1993, 1994, 1995, 1996, 1997, 1998, 1999, 2000,
      2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012,
      2013, 2014, 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024,
      2025, 2026, 2027, 2028, 2029, 2030, 2031, 2032, 2033, 2034, 2035, 2036,
      2037, 2038, 2039, 2040, 2041, 2042, 2043, 2044, 2045, 2046, 2047 };

    int32_T i;

    /* Assignment: '<Root>/Assignment12' incorporates:
     *  Constant: '<Root>/Constant12'
     */
    for (i = 0; i < 2048; i++) {
      PHAT_B.Assignment22[i].re = PHAT_B.Divide20[i].re;
      PHAT_B.Assignment22[i].im = PHAT_B.Divide20[i].im;
    }

    for (i = 0; i < 126; i++) {
      PHAT_B.Assignment22[dimIdxEls0_0_101[i]].re = PHAT_P.Constant12_Value;
      PHAT_B.Assignment22[dimIdxEls0_0_101[i]].im = 0.0F;
    }
  }

  /* Signal Processing Blockset FFT (sdspfft2) - '<Root>/IFFT11' */
  /* Complex input, real output,1 channels, 2048 rows, linear output order */
  /* Apply half-length algorithm to single real signal: */
  {
    const creal32_T *lastColIn = PHAT_B.Assignment22;/* Point to last column of input */
    real32_T *lastColOut = PHAT_B.IFFT20;/* Point to last column of output */
    MWDSP_Ifft_DblLen_TBL_C_Cbr_Oop((creal32_T *)lastColOut, lastColIn, 2048,
      &PHAT_P.IFFT11_TwiddleTable[0], 1);
    MWDSP_R2DIT_TBLS_C((creal32_T *)lastColOut, 1, 1024, 1024,
                       &PHAT_P.IFFT11_TwiddleTable[0], 2, 1);/* Radix-2 DIT IFFT using TableSpeed twiddle computation */
  }

  MWDSP_ScaleData_RR(PHAT_B.IFFT20, 2048, 1.0/2048);

  {
    int32_T i;

    /* Outport: '<Root>/R34' */
    for (i = 0; i < 501; i++) {
      PHAT_Y.R34[i] = PHAT_B.IFFT20[i+1547];
    }

    for (i = 0; i < 500; i++) {
      PHAT_Y.R34[i + 501] = PHAT_B.IFFT20[i];
    }

    for (i = 0; i < 2048; i++) {
      /* Product: '<Root>/P35' */
      PHAT_B.Assignment22[i].re = PHAT_B.FFT2[i].re * PHAT_B.MathFunction3[i].re
        - PHAT_B.FFT2[i].im * PHAT_B.MathFunction3[i].im;
      PHAT_B.Assignment22[i].im = PHAT_B.FFT2[i].re * PHAT_B.MathFunction3[i].im
        + PHAT_B.FFT2[i].im * PHAT_B.MathFunction3[i].re;
    }
  }

  /* Abs: '<Root>/Abs12' */
  {
    int_T i1;
    const creal32_T *u0 = PHAT_B.Assignment22;
    real32_T *y0 = PHAT_B.IFFT20;
    for (i1=0; i1 < 2048; i1++) {
      y0[i1] = (real32_T)sqrt( u0[i1].re*u0[i1].re + u0[i1].im*u0[i1].im );
    }
  }

  {
    creal32_T output;

    {
      int_T i1;
      const creal32_T *u0 = PHAT_B.Assignment22;
      const real32_T *u1 = PHAT_B.IFFT20;
      creal32_T *y0 = PHAT_B.Divide20;
      for (i1=0; i1 < 2048; i1++) {
        output = u0[i1];
        output.re = output.re / u1[i1];
        output.im = output.im / u1[i1];
        y0[i1] = output;
      }
    }
  }

  {
    int32_T dimIdxEls0_0_108[126] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
      14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32
      , 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
      51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 1985, 1986, 1987, 1988
      , 1989, 1990, 1991, 1992, 1993, 1994, 1995, 1996, 1997, 1998, 1999, 2000,
      2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012,
      2013, 2014, 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024,
      2025, 2026, 2027, 2028, 2029, 2030, 2031, 2032, 2033, 2034, 2035, 2036,
      2037, 2038, 2039, 2040, 2041, 2042, 2043, 2044, 2045, 2046, 2047 };

    int32_T i;

    /* Assignment: '<Root>/Assignment13' incorporates:
     *  Constant: '<Root>/Constant13'
     */
    for (i = 0; i < 2048; i++) {
      PHAT_B.Assignment22[i].re = PHAT_B.Divide20[i].re;
      PHAT_B.Assignment22[i].im = PHAT_B.Divide20[i].im;
    }

    for (i = 0; i < 126; i++) {
      PHAT_B.Assignment22[dimIdxEls0_0_108[i]].re = PHAT_P.Constant13_Value;
      PHAT_B.Assignment22[dimIdxEls0_0_108[i]].im = 0.0F;
    }
  }

  /* Signal Processing Blockset FFT (sdspfft2) - '<Root>/IFFT12' */
  /* Complex input, real output,1 channels, 2048 rows, linear output order */
  /* Apply half-length algorithm to single real signal: */
  {
    const creal32_T *lastColIn = PHAT_B.Assignment22;/* Point to last column of input */
    real32_T *lastColOut = PHAT_B.IFFT20;/* Point to last column of output */
    MWDSP_Ifft_DblLen_TBL_C_Cbr_Oop((creal32_T *)lastColOut, lastColIn, 2048,
      &PHAT_P.IFFT12_TwiddleTable[0], 1);
    MWDSP_R2DIT_TBLS_C((creal32_T *)lastColOut, 1, 1024, 1024,
                       &PHAT_P.IFFT12_TwiddleTable[0], 2, 1);/* Radix-2 DIT IFFT using TableSpeed twiddle computation */
  }

  MWDSP_ScaleData_RR(PHAT_B.IFFT20, 2048, 1.0/2048);

  {
    int32_T i;

    /* Outport: '<Root>/R35' */
    for (i = 0; i < 501; i++) {
      PHAT_Y.R35[i] = PHAT_B.IFFT20[i+1547];
    }

    for (i = 0; i < 500; i++) {
      PHAT_Y.R35[i + 501] = PHAT_B.IFFT20[i];
    }

    for (i = 0; i < 2048; i++) {
      /* Product: '<Root>/P36' */
      PHAT_B.Assignment22[i].re = PHAT_B.FFT2[i].re * PHAT_B.MathFunction4[i].re
        - PHAT_B.FFT2[i].im * PHAT_B.MathFunction4[i].im;
      PHAT_B.Assignment22[i].im = PHAT_B.FFT2[i].re * PHAT_B.MathFunction4[i].im
        + PHAT_B.FFT2[i].im * PHAT_B.MathFunction4[i].re;
    }
  }

  /* Abs: '<Root>/Abs13' */
  {
    int_T i1;
    const creal32_T *u0 = PHAT_B.Assignment22;
    real32_T *y0 = PHAT_B.IFFT20;
    for (i1=0; i1 < 2048; i1++) {
      y0[i1] = (real32_T)sqrt( u0[i1].re*u0[i1].re + u0[i1].im*u0[i1].im );
    }
  }

  {
    creal32_T output;

    {
      int_T i1;
      const creal32_T *u0 = PHAT_B.Assignment22;
      const real32_T *u1 = PHAT_B.IFFT20;
      creal32_T *y0 = PHAT_B.Divide20;
      for (i1=0; i1 < 2048; i1++) {
        output = u0[i1];
        output.re = output.re / u1[i1];
        output.im = output.im / u1[i1];
        y0[i1] = output;
      }
    }
  }

  {
    int32_T dimIdxEls0_0_115[126] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
      14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32
      , 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
      51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 1985, 1986, 1987, 1988
      , 1989, 1990, 1991, 1992, 1993, 1994, 1995, 1996, 1997, 1998, 1999, 2000,
      2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012,
      2013, 2014, 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024,
      2025, 2026, 2027, 2028, 2029, 2030, 2031, 2032, 2033, 2034, 2035, 2036,
      2037, 2038, 2039, 2040, 2041, 2042, 2043, 2044, 2045, 2046, 2047 };

    int32_T i;

    /* Assignment: '<Root>/Assignment14' incorporates:
     *  Constant: '<Root>/Constant14'
     */
    for (i = 0; i < 2048; i++) {
      PHAT_B.Assignment22[i].re = PHAT_B.Divide20[i].re;
      PHAT_B.Assignment22[i].im = PHAT_B.Divide20[i].im;
    }

    for (i = 0; i < 126; i++) {
      PHAT_B.Assignment22[dimIdxEls0_0_115[i]].re = PHAT_P.Constant14_Value;
      PHAT_B.Assignment22[dimIdxEls0_0_115[i]].im = 0.0F;
    }
  }

  /* Signal Processing Blockset FFT (sdspfft2) - '<Root>/IFFT13' */
  /* Complex input, real output,1 channels, 2048 rows, linear output order */
  /* Apply half-length algorithm to single real signal: */
  {
    const creal32_T *lastColIn = PHAT_B.Assignment22;/* Point to last column of input */
    real32_T *lastColOut = PHAT_B.IFFT20;/* Point to last column of output */
    MWDSP_Ifft_DblLen_TBL_C_Cbr_Oop((creal32_T *)lastColOut, lastColIn, 2048,
      &PHAT_P.IFFT13_TwiddleTable[0], 1);
    MWDSP_R2DIT_TBLS_C((creal32_T *)lastColOut, 1, 1024, 1024,
                       &PHAT_P.IFFT13_TwiddleTable[0], 2, 1);/* Radix-2 DIT IFFT using TableSpeed twiddle computation */
  }

  MWDSP_ScaleData_RR(PHAT_B.IFFT20, 2048, 1.0/2048);

  {
    int32_T i;

    /* Outport: '<Root>/R36' */
    for (i = 0; i < 501; i++) {
      PHAT_Y.R36[i] = PHAT_B.IFFT20[i+1547];
    }

    for (i = 0; i < 500; i++) {
      PHAT_Y.R36[i + 501] = PHAT_B.IFFT20[i];
    }

    for (i = 0; i < 2048; i++) {
      /* Product: '<Root>/P37' */
      PHAT_B.Assignment22[i].re = PHAT_B.FFT2[i].re * PHAT_B.MathFunction5[i].re
        - PHAT_B.FFT2[i].im * PHAT_B.MathFunction5[i].im;
      PHAT_B.Assignment22[i].im = PHAT_B.FFT2[i].re * PHAT_B.MathFunction5[i].im
        + PHAT_B.FFT2[i].im * PHAT_B.MathFunction5[i].re;
    }
  }

  /* Abs: '<Root>/Abs14' */
  {
    int_T i1;
    const creal32_T *u0 = PHAT_B.Assignment22;
    real32_T *y0 = PHAT_B.IFFT20;
    for (i1=0; i1 < 2048; i1++) {
      y0[i1] = (real32_T)sqrt( u0[i1].re*u0[i1].re + u0[i1].im*u0[i1].im );
    }
  }

  {
    creal32_T output;

    {
      int_T i1;
      const creal32_T *u0 = PHAT_B.Assignment22;
      const real32_T *u1 = PHAT_B.IFFT20;
      creal32_T *y0 = PHAT_B.Divide20;
      for (i1=0; i1 < 2048; i1++) {
        output = u0[i1];
        output.re = output.re / u1[i1];
        output.im = output.im / u1[i1];
        y0[i1] = output;
      }
    }
  }

  {
    int32_T dimIdxEls0_0_122[126] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
      14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32
      , 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
      51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 1985, 1986, 1987, 1988
      , 1989, 1990, 1991, 1992, 1993, 1994, 1995, 1996, 1997, 1998, 1999, 2000,
      2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012,
      2013, 2014, 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024,
      2025, 2026, 2027, 2028, 2029, 2030, 2031, 2032, 2033, 2034, 2035, 2036,
      2037, 2038, 2039, 2040, 2041, 2042, 2043, 2044, 2045, 2046, 2047 };

    int32_T i;

    /* Assignment: '<Root>/Assignment15' incorporates:
     *  Constant: '<Root>/Constant15'
     */
    for (i = 0; i < 2048; i++) {
      PHAT_B.Assignment22[i].re = PHAT_B.Divide20[i].re;
      PHAT_B.Assignment22[i].im = PHAT_B.Divide20[i].im;
    }

    for (i = 0; i < 126; i++) {
      PHAT_B.Assignment22[dimIdxEls0_0_122[i]].re = PHAT_P.Constant15_Value;
      PHAT_B.Assignment22[dimIdxEls0_0_122[i]].im = 0.0F;
    }
  }

  /* Signal Processing Blockset FFT (sdspfft2) - '<Root>/IFFT14' */
  /* Complex input, real output,1 channels, 2048 rows, linear output order */
  /* Apply half-length algorithm to single real signal: */
  {
    const creal32_T *lastColIn = PHAT_B.Assignment22;/* Point to last column of input */
    real32_T *lastColOut = PHAT_B.IFFT20;/* Point to last column of output */
    MWDSP_Ifft_DblLen_TBL_C_Cbr_Oop((creal32_T *)lastColOut, lastColIn, 2048,
      &PHAT_P.IFFT14_TwiddleTable[0], 1);
    MWDSP_R2DIT_TBLS_C((creal32_T *)lastColOut, 1, 1024, 1024,
                       &PHAT_P.IFFT14_TwiddleTable[0], 2, 1);/* Radix-2 DIT IFFT using TableSpeed twiddle computation */
  }

  MWDSP_ScaleData_RR(PHAT_B.IFFT20, 2048, 1.0/2048);

  {
    int32_T i;

    /* Outport: '<Root>/R37' */
    for (i = 0; i < 501; i++) {
      PHAT_Y.R37[i] = PHAT_B.IFFT20[i+1547];
    }

    for (i = 0; i < 500; i++) {
      PHAT_Y.R37[i + 501] = PHAT_B.IFFT20[i];
    }

    for (i = 0; i < 2048; i++) {
      /* Product: '<Root>/P45' */
      PHAT_B.Assignment22[i].re = PHAT_B.FFT3[i].re * PHAT_B.MathFunction3[i].re
        - PHAT_B.FFT3[i].im * PHAT_B.MathFunction3[i].im;
      PHAT_B.Assignment22[i].im = PHAT_B.FFT3[i].re * PHAT_B.MathFunction3[i].im
        + PHAT_B.FFT3[i].im * PHAT_B.MathFunction3[i].re;
    }
  }

  /* Abs: '<Root>/Abs15' */
  {
    int_T i1;
    const creal32_T *u0 = PHAT_B.Assignment22;
    real32_T *y0 = PHAT_B.IFFT20;
    for (i1=0; i1 < 2048; i1++) {
      y0[i1] = (real32_T)sqrt( u0[i1].re*u0[i1].re + u0[i1].im*u0[i1].im );
    }
  }

  {
    creal32_T output;

    {
      int_T i1;
      const creal32_T *u0 = PHAT_B.Assignment22;
      const real32_T *u1 = PHAT_B.IFFT20;
      creal32_T *y0 = PHAT_B.Divide20;
      for (i1=0; i1 < 2048; i1++) {
        output = u0[i1];
        output.re = output.re / u1[i1];
        output.im = output.im / u1[i1];
        y0[i1] = output;
      }
    }
  }

  {
    int32_T dimIdxEls0_0_129[126] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
      14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32
      , 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
      51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 1985, 1986, 1987, 1988
      , 1989, 1990, 1991, 1992, 1993, 1994, 1995, 1996, 1997, 1998, 1999, 2000,
      2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012,
      2013, 2014, 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024,
      2025, 2026, 2027, 2028, 2029, 2030, 2031, 2032, 2033, 2034, 2035, 2036,
      2037, 2038, 2039, 2040, 2041, 2042, 2043, 2044, 2045, 2046, 2047 };

    int32_T i;

    /* Assignment: '<Root>/Assignment16' incorporates:
     *  Constant: '<Root>/Constant16'
     */
    for (i = 0; i < 2048; i++) {
      PHAT_B.Assignment22[i].re = PHAT_B.Divide20[i].re;
      PHAT_B.Assignment22[i].im = PHAT_B.Divide20[i].im;
    }

    for (i = 0; i < 126; i++) {
      PHAT_B.Assignment22[dimIdxEls0_0_129[i]].re = PHAT_P.Constant16_Value;
      PHAT_B.Assignment22[dimIdxEls0_0_129[i]].im = 0.0F;
    }
  }

  /* Signal Processing Blockset FFT (sdspfft2) - '<Root>/IFFT15' */
  /* Complex input, real output,1 channels, 2048 rows, linear output order */
  /* Apply half-length algorithm to single real signal: */
  {
    const creal32_T *lastColIn = PHAT_B.Assignment22;/* Point to last column of input */
    real32_T *lastColOut = PHAT_B.IFFT20;/* Point to last column of output */
    MWDSP_Ifft_DblLen_TBL_C_Cbr_Oop((creal32_T *)lastColOut, lastColIn, 2048,
      &PHAT_P.IFFT15_TwiddleTable[0], 1);
    MWDSP_R2DIT_TBLS_C((creal32_T *)lastColOut, 1, 1024, 1024,
                       &PHAT_P.IFFT15_TwiddleTable[0], 2, 1);/* Radix-2 DIT IFFT using TableSpeed twiddle computation */
  }

  MWDSP_ScaleData_RR(PHAT_B.IFFT20, 2048, 1.0/2048);

  {
    int32_T i;

    /* Outport: '<Root>/R45' */
    for (i = 0; i < 501; i++) {
      PHAT_Y.R45[i] = PHAT_B.IFFT20[i+1547];
    }

    for (i = 0; i < 500; i++) {
      PHAT_Y.R45[i + 501] = PHAT_B.IFFT20[i];
    }

    for (i = 0; i < 2048; i++) {
      /* Product: '<Root>/P46' */
      PHAT_B.Assignment22[i].re = PHAT_B.FFT3[i].re * PHAT_B.MathFunction4[i].re
        - PHAT_B.FFT3[i].im * PHAT_B.MathFunction4[i].im;
      PHAT_B.Assignment22[i].im = PHAT_B.FFT3[i].re * PHAT_B.MathFunction4[i].im
        + PHAT_B.FFT3[i].im * PHAT_B.MathFunction4[i].re;
    }
  }

  /* Abs: '<Root>/Abs16' */
  {
    int_T i1;
    const creal32_T *u0 = PHAT_B.Assignment22;
    real32_T *y0 = PHAT_B.IFFT20;
    for (i1=0; i1 < 2048; i1++) {
      y0[i1] = (real32_T)sqrt( u0[i1].re*u0[i1].re + u0[i1].im*u0[i1].im );
    }
  }

  {
    creal32_T output;

    {
      int_T i1;
      const creal32_T *u0 = PHAT_B.Assignment22;
      const real32_T *u1 = PHAT_B.IFFT20;
      creal32_T *y0 = PHAT_B.Divide20;
      for (i1=0; i1 < 2048; i1++) {
        output = u0[i1];
        output.re = output.re / u1[i1];
        output.im = output.im / u1[i1];
        y0[i1] = output;
      }
    }
  }

  {
    int32_T dimIdxEls0_0_136[126] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
      14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32
      , 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
      51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 1985, 1986, 1987, 1988
      , 1989, 1990, 1991, 1992, 1993, 1994, 1995, 1996, 1997, 1998, 1999, 2000,
      2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012,
      2013, 2014, 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024,
      2025, 2026, 2027, 2028, 2029, 2030, 2031, 2032, 2033, 2034, 2035, 2036,
      2037, 2038, 2039, 2040, 2041, 2042, 2043, 2044, 2045, 2046, 2047 };

    int32_T i;

    /* Assignment: '<Root>/Assignment17' incorporates:
     *  Constant: '<Root>/Constant17'
     */
    for (i = 0; i < 2048; i++) {
      PHAT_B.Assignment22[i].re = PHAT_B.Divide20[i].re;
      PHAT_B.Assignment22[i].im = PHAT_B.Divide20[i].im;
    }

    for (i = 0; i < 126; i++) {
      PHAT_B.Assignment22[dimIdxEls0_0_136[i]].re = PHAT_P.Constant17_Value;
      PHAT_B.Assignment22[dimIdxEls0_0_136[i]].im = 0.0F;
    }
  }

  /* Signal Processing Blockset FFT (sdspfft2) - '<Root>/IFFT16' */
  /* Complex input, real output,1 channels, 2048 rows, linear output order */
  /* Apply half-length algorithm to single real signal: */
  {
    const creal32_T *lastColIn = PHAT_B.Assignment22;/* Point to last column of input */
    real32_T *lastColOut = PHAT_B.IFFT20;/* Point to last column of output */
    MWDSP_Ifft_DblLen_TBL_C_Cbr_Oop((creal32_T *)lastColOut, lastColIn, 2048,
      &PHAT_P.IFFT16_TwiddleTable[0], 1);
    MWDSP_R2DIT_TBLS_C((creal32_T *)lastColOut, 1, 1024, 1024,
                       &PHAT_P.IFFT16_TwiddleTable[0], 2, 1);/* Radix-2 DIT IFFT using TableSpeed twiddle computation */
  }

  MWDSP_ScaleData_RR(PHAT_B.IFFT20, 2048, 1.0/2048);

  {
    int32_T i;

    /* Outport: '<Root>/R46' */
    for (i = 0; i < 501; i++) {
      PHAT_Y.R46[i] = PHAT_B.IFFT20[i+1547];
    }

    for (i = 0; i < 500; i++) {
      PHAT_Y.R46[i + 501] = PHAT_B.IFFT20[i];
    }

    for (i = 0; i < 2048; i++) {
      /* Product: '<Root>/P47' */
      PHAT_B.Assignment22[i].re = PHAT_B.FFT3[i].re * PHAT_B.MathFunction5[i].re
        - PHAT_B.FFT3[i].im * PHAT_B.MathFunction5[i].im;
      PHAT_B.Assignment22[i].im = PHAT_B.FFT3[i].re * PHAT_B.MathFunction5[i].im
        + PHAT_B.FFT3[i].im * PHAT_B.MathFunction5[i].re;
    }
  }

  /* Abs: '<Root>/Abs17' */
  {
    int_T i1;
    const creal32_T *u0 = PHAT_B.Assignment22;
    real32_T *y0 = PHAT_B.IFFT20;
    for (i1=0; i1 < 2048; i1++) {
      y0[i1] = (real32_T)sqrt( u0[i1].re*u0[i1].re + u0[i1].im*u0[i1].im );
    }
  }

  {
    creal32_T output;

    {
      int_T i1;
      const creal32_T *u0 = PHAT_B.Assignment22;
      const real32_T *u1 = PHAT_B.IFFT20;
      creal32_T *y0 = PHAT_B.Divide20;
      for (i1=0; i1 < 2048; i1++) {
        output = u0[i1];
        output.re = output.re / u1[i1];
        output.im = output.im / u1[i1];
        y0[i1] = output;
      }
    }
  }

  {
    int32_T dimIdxEls0_0_143[126] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
      14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32
      , 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
      51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 1985, 1986, 1987, 1988
      , 1989, 1990, 1991, 1992, 1993, 1994, 1995, 1996, 1997, 1998, 1999, 2000,
      2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012,
      2013, 2014, 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024,
      2025, 2026, 2027, 2028, 2029, 2030, 2031, 2032, 2033, 2034, 2035, 2036,
      2037, 2038, 2039, 2040, 2041, 2042, 2043, 2044, 2045, 2046, 2047 };

    int32_T i;

    /* Assignment: '<Root>/Assignment18' incorporates:
     *  Constant: '<Root>/Constant18'
     */
    for (i = 0; i < 2048; i++) {
      PHAT_B.Assignment22[i].re = PHAT_B.Divide20[i].re;
      PHAT_B.Assignment22[i].im = PHAT_B.Divide20[i].im;
    }

    for (i = 0; i < 126; i++) {
      PHAT_B.Assignment22[dimIdxEls0_0_143[i]].re = PHAT_P.Constant18_Value;
      PHAT_B.Assignment22[dimIdxEls0_0_143[i]].im = 0.0F;
    }
  }

  /* Signal Processing Blockset FFT (sdspfft2) - '<Root>/IFFT17' */
  /* Complex input, real output,1 channels, 2048 rows, linear output order */
  /* Apply half-length algorithm to single real signal: */
  {
    const creal32_T *lastColIn = PHAT_B.Assignment22;/* Point to last column of input */
    real32_T *lastColOut = PHAT_B.IFFT20;/* Point to last column of output */
    MWDSP_Ifft_DblLen_TBL_C_Cbr_Oop((creal32_T *)lastColOut, lastColIn, 2048,
      &PHAT_P.IFFT17_TwiddleTable[0], 1);
    MWDSP_R2DIT_TBLS_C((creal32_T *)lastColOut, 1, 1024, 1024,
                       &PHAT_P.IFFT17_TwiddleTable[0], 2, 1);/* Radix-2 DIT IFFT using TableSpeed twiddle computation */
  }

  MWDSP_ScaleData_RR(PHAT_B.IFFT20, 2048, 1.0/2048);

  {
    int32_T i;

    /* Outport: '<Root>/R47' */
    for (i = 0; i < 501; i++) {
      PHAT_Y.R47[i] = PHAT_B.IFFT20[i+1547];
    }

    for (i = 0; i < 500; i++) {
      PHAT_Y.R47[i + 501] = PHAT_B.IFFT20[i];
    }

    for (i = 0; i < 2048; i++) {
      /* Product: '<Root>/P56' */
      PHAT_B.Assignment22[i].re = PHAT_B.FFT4[i].re * PHAT_B.MathFunction4[i].re
        - PHAT_B.FFT4[i].im * PHAT_B.MathFunction4[i].im;
      PHAT_B.Assignment22[i].im = PHAT_B.FFT4[i].re * PHAT_B.MathFunction4[i].im
        + PHAT_B.FFT4[i].im * PHAT_B.MathFunction4[i].re;
    }
  }

  /* Abs: '<Root>/Abs18' */
  {
    int_T i1;
    const creal32_T *u0 = PHAT_B.Assignment22;
    real32_T *y0 = PHAT_B.IFFT20;
    for (i1=0; i1 < 2048; i1++) {
      y0[i1] = (real32_T)sqrt( u0[i1].re*u0[i1].re + u0[i1].im*u0[i1].im );
    }
  }

  {
    creal32_T output;

    {
      int_T i1;
      const creal32_T *u0 = PHAT_B.Assignment22;
      const real32_T *u1 = PHAT_B.IFFT20;
      creal32_T *y0 = PHAT_B.Divide20;
      for (i1=0; i1 < 2048; i1++) {
        output = u0[i1];
        output.re = output.re / u1[i1];
        output.im = output.im / u1[i1];
        y0[i1] = output;
      }
    }
  }

  {
    int32_T dimIdxEls0_0_150[126] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
      14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32
      , 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
      51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 1985, 1986, 1987, 1988
      , 1989, 1990, 1991, 1992, 1993, 1994, 1995, 1996, 1997, 1998, 1999, 2000,
      2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012,
      2013, 2014, 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024,
      2025, 2026, 2027, 2028, 2029, 2030, 2031, 2032, 2033, 2034, 2035, 2036,
      2037, 2038, 2039, 2040, 2041, 2042, 2043, 2044, 2045, 2046, 2047 };

    int32_T i;

    /* Assignment: '<Root>/Assignment19' incorporates:
     *  Constant: '<Root>/Constant19'
     */
    for (i = 0; i < 2048; i++) {
      PHAT_B.Assignment22[i].re = PHAT_B.Divide20[i].re;
      PHAT_B.Assignment22[i].im = PHAT_B.Divide20[i].im;
    }

    for (i = 0; i < 126; i++) {
      PHAT_B.Assignment22[dimIdxEls0_0_150[i]].re = PHAT_P.Constant19_Value;
      PHAT_B.Assignment22[dimIdxEls0_0_150[i]].im = 0.0F;
    }
  }

  /* Signal Processing Blockset FFT (sdspfft2) - '<Root>/IFFT18' */
  /* Complex input, real output,1 channels, 2048 rows, linear output order */
  /* Apply half-length algorithm to single real signal: */
  {
    const creal32_T *lastColIn = PHAT_B.Assignment22;/* Point to last column of input */
    real32_T *lastColOut = PHAT_B.IFFT20;/* Point to last column of output */
    MWDSP_Ifft_DblLen_TBL_C_Cbr_Oop((creal32_T *)lastColOut, lastColIn, 2048,
      &PHAT_P.IFFT18_TwiddleTable[0], 1);
    MWDSP_R2DIT_TBLS_C((creal32_T *)lastColOut, 1, 1024, 1024,
                       &PHAT_P.IFFT18_TwiddleTable[0], 2, 1);/* Radix-2 DIT IFFT using TableSpeed twiddle computation */
  }

  MWDSP_ScaleData_RR(PHAT_B.IFFT20, 2048, 1.0/2048);

  {
    int32_T i;

    /* Outport: '<Root>/R56' */
    for (i = 0; i < 501; i++) {
      PHAT_Y.R56[i] = PHAT_B.IFFT20[i+1547];
    }

    for (i = 0; i < 500; i++) {
      PHAT_Y.R56[i + 501] = PHAT_B.IFFT20[i];
    }

    for (i = 0; i < 2048; i++) {
      /* Product: '<Root>/P57' */
      PHAT_B.Assignment22[i].re = PHAT_B.FFT4[i].re * PHAT_B.MathFunction5[i].re
        - PHAT_B.FFT4[i].im * PHAT_B.MathFunction5[i].im;
      PHAT_B.Assignment22[i].im = PHAT_B.FFT4[i].re * PHAT_B.MathFunction5[i].im
        + PHAT_B.FFT4[i].im * PHAT_B.MathFunction5[i].re;
    }
  }

  /* Abs: '<Root>/Abs19' */
  {
    int_T i1;
    const creal32_T *u0 = PHAT_B.Assignment22;
    real32_T *y0 = PHAT_B.IFFT20;
    for (i1=0; i1 < 2048; i1++) {
      y0[i1] = (real32_T)sqrt( u0[i1].re*u0[i1].re + u0[i1].im*u0[i1].im );
    }
  }

  {
    creal32_T output;

    {
      int_T i1;
      const creal32_T *u0 = PHAT_B.Assignment22;
      const real32_T *u1 = PHAT_B.IFFT20;
      creal32_T *y0 = PHAT_B.Divide20;
      for (i1=0; i1 < 2048; i1++) {
        output = u0[i1];
        output.re = output.re / u1[i1];
        output.im = output.im / u1[i1];
        y0[i1] = output;
      }
    }
  }

  {
    int32_T dimIdxEls0_0_157[126] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
      14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32
      , 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
      51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 1985, 1986, 1987, 1988
      , 1989, 1990, 1991, 1992, 1993, 1994, 1995, 1996, 1997, 1998, 1999, 2000,
      2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012,
      2013, 2014, 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024,
      2025, 2026, 2027, 2028, 2029, 2030, 2031, 2032, 2033, 2034, 2035, 2036,
      2037, 2038, 2039, 2040, 2041, 2042, 2043, 2044, 2045, 2046, 2047 };

    int32_T i;

    /* Assignment: '<Root>/Assignment20' incorporates:
     *  Constant: '<Root>/Constant20'
     */
    for (i = 0; i < 2048; i++) {
      PHAT_B.Assignment22[i].re = PHAT_B.Divide20[i].re;
      PHAT_B.Assignment22[i].im = PHAT_B.Divide20[i].im;
    }

    for (i = 0; i < 126; i++) {
      PHAT_B.Assignment22[dimIdxEls0_0_157[i]].re = PHAT_P.Constant20_Value;
      PHAT_B.Assignment22[dimIdxEls0_0_157[i]].im = 0.0F;
    }
  }

  /* Signal Processing Blockset FFT (sdspfft2) - '<Root>/IFFT19' */
  /* Complex input, real output,1 channels, 2048 rows, linear output order */
  /* Apply half-length algorithm to single real signal: */
  {
    const creal32_T *lastColIn = PHAT_B.Assignment22;/* Point to last column of input */
    real32_T *lastColOut = PHAT_B.IFFT20;/* Point to last column of output */
    MWDSP_Ifft_DblLen_TBL_C_Cbr_Oop((creal32_T *)lastColOut, lastColIn, 2048,
      &PHAT_P.IFFT19_TwiddleTable[0], 1);
    MWDSP_R2DIT_TBLS_C((creal32_T *)lastColOut, 1, 1024, 1024,
                       &PHAT_P.IFFT19_TwiddleTable[0], 2, 1);/* Radix-2 DIT IFFT using TableSpeed twiddle computation */
  }

  MWDSP_ScaleData_RR(PHAT_B.IFFT20, 2048, 1.0/2048);

  {
    int32_T i;

    /* Outport: '<Root>/R57' */
    for (i = 0; i < 501; i++) {
      PHAT_Y.R57[i] = PHAT_B.IFFT20[i+1547];
    }

    for (i = 0; i < 500; i++) {
      PHAT_Y.R57[i + 501] = PHAT_B.IFFT20[i];
    }

    for (i = 0; i < 2048; i++) {
      /* Product: '<Root>/P67' */
      PHAT_B.Assignment22[i].re = PHAT_B.FFT5[i].re * PHAT_B.MathFunction5[i].re
        - PHAT_B.FFT5[i].im * PHAT_B.MathFunction5[i].im;
      PHAT_B.Assignment22[i].im = PHAT_B.FFT5[i].re * PHAT_B.MathFunction5[i].im
        + PHAT_B.FFT5[i].im * PHAT_B.MathFunction5[i].re;
    }
  }

  /* Abs: '<Root>/Abs20' */
  {
    int_T i1;
    const creal32_T *u0 = PHAT_B.Assignment22;
    real32_T *y0 = PHAT_B.IFFT20;
    for (i1=0; i1 < 2048; i1++) {
      y0[i1] = (real32_T)sqrt( u0[i1].re*u0[i1].re + u0[i1].im*u0[i1].im );
    }
  }

  {
    creal32_T output;

    {
      int_T i1;
      const creal32_T *u0 = PHAT_B.Assignment22;
      const real32_T *u1 = PHAT_B.IFFT20;
      creal32_T *y0 = PHAT_B.Divide20;
      for (i1=0; i1 < 2048; i1++) {
        output = u0[i1];
        output.re = output.re / u1[i1];
        output.im = output.im / u1[i1];
        y0[i1] = output;
      }
    }
  }

  {
    int32_T dimIdxEls0_0_164[126] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
      14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32
      , 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
      51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 1985, 1986, 1987, 1988
      , 1989, 1990, 1991, 1992, 1993, 1994, 1995, 1996, 1997, 1998, 1999, 2000,
      2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012,
      2013, 2014, 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024,
      2025, 2026, 2027, 2028, 2029, 2030, 2031, 2032, 2033, 2034, 2035, 2036,
      2037, 2038, 2039, 2040, 2041, 2042, 2043, 2044, 2045, 2046, 2047 };

    int32_T i;

    /* Assignment: '<Root>/Assignment21' incorporates:
     *  Constant: '<Root>/Constant21'
     */
    for (i = 0; i < 2048; i++) {
      PHAT_B.Assignment22[i].re = PHAT_B.Divide20[i].re;
      PHAT_B.Assignment22[i].im = PHAT_B.Divide20[i].im;
    }

    for (i = 0; i < 126; i++) {
      PHAT_B.Assignment22[dimIdxEls0_0_164[i]].re = PHAT_P.Constant21_Value;
      PHAT_B.Assignment22[dimIdxEls0_0_164[i]].im = 0.0F;
    }
  }

  /* Signal Processing Blockset FFT (sdspfft2) - '<Root>/IFFT20' */
  /* Complex input, real output,1 channels, 2048 rows, linear output order */
  /* Apply half-length algorithm to single real signal: */
  {
    const creal32_T *lastColIn = PHAT_B.Assignment22;/* Point to last column of input */
    real32_T *lastColOut = PHAT_B.IFFT20;/* Point to last column of output */
    MWDSP_Ifft_DblLen_TBL_C_Cbr_Oop((creal32_T *)lastColOut, lastColIn, 2048,
      &PHAT_P.IFFT20_TwiddleTable[0], 1);
    MWDSP_R2DIT_TBLS_C((creal32_T *)lastColOut, 1, 1024, 1024,
                       &PHAT_P.IFFT20_TwiddleTable[0], 2, 1);/* Radix-2 DIT IFFT using TableSpeed twiddle computation */
  }

  MWDSP_ScaleData_RR(PHAT_B.IFFT20, 2048, 1.0/2048);

  {
    int32_T dimIdxEls[126] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
      , 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
      34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52
      , 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 1985, 1986, 1987, 1988, 1989
      , 1990, 1991, 1992, 1993, 1994, 1995, 1996, 1997, 1998, 1999, 2000, 2001,
      2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013,
      2014, 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024, 2025,
      2026, 2027, 2028, 2029, 2030, 2031, 2032, 2033, 2034, 2035, 2036, 2037,
      2038, 2039, 2040, 2041, 2042, 2043, 2044, 2045, 2046, 2047 };

    real32_T tmp;
    int32_T i;

    /* Outport: '<Root>/R67' */
    for (i = 0; i < 501; i++) {
      PHAT_Y.R67[i] = PHAT_B.IFFT20[i+1547];
    }

    for (i = 0; i < 500; i++) {
      PHAT_Y.R67[i + 501] = PHAT_B.IFFT20[i];
    }

    /* Assignment: '<Root>/Assignment22' incorporates:
     *  Constant: '<Root>/Constant22'
     */
    for (i = 0; i < 2048; i++) {
      PHAT_B.Assignment22[i].re = PHAT_B.FFT[i].re;
      PHAT_B.Assignment22[i].im = PHAT_B.FFT[i].im;
    }

    for (i = 0; i < 126; i++) {
      PHAT_B.Assignment22[dimIdxEls[i]].re = PHAT_P.Constant22_Value;
      PHAT_B.Assignment22[dimIdxEls[i]].im = 0.0F;
    }

    for (i = 0; i < 2048; i++) {
      /* Math: '<Root>/Math Function6' */
      PHAT_B.MathFunction6[i] = PHAT_B.Assignment22[i].re * PHAT_B.Assignment22
        [i].re + PHAT_B.Assignment22[i].im * PHAT_B.Assignment22[i].im;
    }

    /* Sum: '<Root>/Sum of Elements' */
    tmp = PHAT_B.MathFunction6[0];
    for (i = 1; i < 2048; i++) {
      tmp += PHAT_B.MathFunction6[i];
    }

    /* Outport: '<Root>/Energy' */
    PHAT_Y.Energy = tmp;
  }
}

/* Model initialize function */
void PHAT_initialize(void)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatus(PHAT_M, (const char_T *)0);

  /* block I/O */
  {
    int_T i;
    void *pVoidBlockIORegion;
    pVoidBlockIORegion = (void *)(&PHAT_B.FFT[0].re);
    for (i = 0; i < 57344; i++) {
      ((real32_T*)pVoidBlockIORegion)[i] = 0.0F;
    }
  }

  /* external inputs */
  {
    int_T i;
    for (i = 0; i < 1600; i++) {
      PHAT_U.Channel1[i] = 0.0F;
    }
  }

  {
    int_T i;
    for (i = 0; i < 1600; i++) {
      PHAT_U.Channel2[i] = 0.0F;
    }
  }

  {
    int_T i;
    for (i = 0; i < 1600; i++) {
      PHAT_U.Channel3[i] = 0.0F;
    }
  }

  {
    int_T i;
    for (i = 0; i < 1600; i++) {
      PHAT_U.Channel4[i] = 0.0F;
    }
  }

  {
    int_T i;
    for (i = 0; i < 1600; i++) {
      PHAT_U.Channel5[i] = 0.0F;
    }
  }

  {
    int_T i;
    for (i = 0; i < 1600; i++) {
      PHAT_U.Channel6[i] = 0.0F;
    }
  }

  {
    int_T i;
    for (i = 0; i < 1600; i++) {
      PHAT_U.Channel7[i] = 0.0F;
    }
  }

  /* external outputs */
  {
    int_T i;
    for (i = 0; i < 1001; i++) {
      PHAT_Y.R12[i] = 0.0F;
    }
  }

  {
    int_T i;
    for (i = 0; i < 1001; i++) {
      PHAT_Y.R13[i] = 0.0F;
    }
  }

  {
    int_T i;
    for (i = 0; i < 1001; i++) {
      PHAT_Y.R14[i] = 0.0F;
    }
  }

  {
    int_T i;
    for (i = 0; i < 1001; i++) {
      PHAT_Y.R15[i] = 0.0F;
    }
  }

  {
    int_T i;
    for (i = 0; i < 1001; i++) {
      PHAT_Y.R16[i] = 0.0F;
    }
  }

  {
    int_T i;
    for (i = 0; i < 1001; i++) {
      PHAT_Y.R17[i] = 0.0F;
    }
  }

  {
    int_T i;
    for (i = 0; i < 1001; i++) {
      PHAT_Y.R23[i] = 0.0F;
    }
  }

  {
    int_T i;
    for (i = 0; i < 1001; i++) {
      PHAT_Y.R24[i] = 0.0F;
    }
  }

  {
    int_T i;
    for (i = 0; i < 1001; i++) {
      PHAT_Y.R25[i] = 0.0F;
    }
  }

  {
    int_T i;
    for (i = 0; i < 1001; i++) {
      PHAT_Y.R26[i] = 0.0F;
    }
  }

  {
    int_T i;
    for (i = 0; i < 1001; i++) {
      PHAT_Y.R27[i] = 0.0F;
    }
  }

  {
    int_T i;
    for (i = 0; i < 1001; i++) {
      PHAT_Y.R34[i] = 0.0F;
    }
  }

  {
    int_T i;
    for (i = 0; i < 1001; i++) {
      PHAT_Y.R35[i] = 0.0F;
    }
  }

  {
    int_T i;
    for (i = 0; i < 1001; i++) {
      PHAT_Y.R36[i] = 0.0F;
    }
  }

  {
    int_T i;
    for (i = 0; i < 1001; i++) {
      PHAT_Y.R37[i] = 0.0F;
    }
  }

  {
    int_T i;
    for (i = 0; i < 1001; i++) {
      PHAT_Y.R45[i] = 0.0F;
    }
  }

  {
    int_T i;
    for (i = 0; i < 1001; i++) {
      PHAT_Y.R46[i] = 0.0F;
    }
  }

  {
    int_T i;
    for (i = 0; i < 1001; i++) {
      PHAT_Y.R47[i] = 0.0F;
    }
  }

  {
    int_T i;
    for (i = 0; i < 1001; i++) {
      PHAT_Y.R56[i] = 0.0F;
    }
  }

  {
    int_T i;
    for (i = 0; i < 1001; i++) {
      PHAT_Y.R57[i] = 0.0F;
    }
  }

  {
    int_T i;
    for (i = 0; i < 1001; i++) {
      PHAT_Y.R67[i] = 0.0F;
    }
  }

  PHAT_Y.Energy = 0.0F;
}

/* Model terminate function */
void PHAT_terminate(void)
{
  /* (no terminate code required) */
}

/* File trailer for Real-Time Workshop generated code.
 *
 * [EOF]
 */
