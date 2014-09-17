/*
 * File: Filter.c
 *
 * Real-Time Workshop code generated for Simulink model Filter.
 *
 * Model version                        : 1.6
 * Real-Time Workshop file version      : 6.5  (R2006b Beta)  21-Sep-2006
 * Real-Time Workshop file generated on : Wed Jul  9 18:09:11 2008
 * TLC version                          : 6.5 (Sep 20 2006)
 * C source code generated on           : Wed Jul  9 18:09:12 2008
 */
#include "Filter.h"
#include "Filter_private.h"

/* Block signals (auto storage) */
BlockIO_Filter Filter_B;

/* Block states (auto storage) */
D_Work_Filter Filter_DWork;

/* External inputs (root inport signals with auto storage) */
ExternalInputs_Filter Filter_U;

/* External outputs (root outports fed by signals with auto storage) */
ExternalOutputs_Filter Filter_Y;

/* Real-time model */
RT_MODEL_Filter Filter_M_;
RT_MODEL_Filter *Filter_M = &Filter_M_;

/* Model step function */
void Filter_step(void)
{
  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<Root>/MultiBandFilter' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)&Filter_U.Channel1[0];
    real32_T *outputs = (real32_T *)Filter_B.Stage10;
    real32_T *coeffs = (real32_T *)&Filter_P.MultiBandFilter_RTP1COEFF[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.MultiBandFilter_RTP2COEFF[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.MultiBandFilter_FILT_STATES
        [0];
      real32_T stageIn, stageOut;
      int_T memOffset = 4 * k;         /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 2; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  {
    int32_T i;
    for (i = 0; i < 400; i++) {
      /* Outport: '<Root>/Output 1' */
      Filter_Y.Output1[i] = Filter_B.Stage10[i];
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S1>/Stage1' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)&Filter_U.Channel2[0];
    real32_T *outputs = (real32_T *)Filter_B.Stage10;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage1_RTP1COEFF[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage1_RTP2COEFF[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage1_FILT_STATES[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 26 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 13; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S1>/Stage2' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage10;
    real32_T *outputs = (real32_T *)Filter_B.Stage9;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage2_RTP1COEFF[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage2_RTP2COEFF[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage2_FILT_STATES[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 26 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 13; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S1>/Stage3' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage9;
    real32_T *outputs = (real32_T *)Filter_B.Stage10;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage3_RTP1COEFF[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage3_RTP2COEFF[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage3_FILT_STATES[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 26 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 13; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S1>/Stage4' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage10;
    real32_T *outputs = (real32_T *)Filter_B.Stage9;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage4_RTP1COEFF[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage4_RTP2COEFF[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage4_FILT_STATES[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 26 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 13; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S1>/Stage5' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage9;
    real32_T *outputs = (real32_T *)Filter_B.Stage10;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage5_RTP1COEFF[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage5_RTP2COEFF[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage5_FILT_STATES[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 26 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 13; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S1>/Stage6' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage10;
    real32_T *outputs = (real32_T *)Filter_B.Stage9;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage6_RTP1COEFF[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage6_RTP2COEFF[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage6_FILT_STATES[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 26 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 13; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S1>/Stage7' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage9;
    real32_T *outputs = (real32_T *)Filter_B.Stage10;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage7_RTP1COEFF[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage7_RTP2COEFF[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage7_FILT_STATES[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 26 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 13; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S1>/Stage8' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage10;
    real32_T *outputs = (real32_T *)Filter_B.Stage9;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage8_RTP1COEFF[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage8_RTP2COEFF[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage8_FILT_STATES[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 24 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 12; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S1>/Stage9' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage9;
    real32_T *outputs = (real32_T *)Filter_B.Stage10;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage9_RTP1COEFF[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage9_RTP2COEFF[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage9_FILT_STATES[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 24 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 12; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S1>/Stage10' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage10;
    real32_T *outputs = (real32_T *)Filter_B.Stage9;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage10_RTP1COEFF[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage10_RTP2COEFF[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage10_FILT_STATES[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 24 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 12; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  {
    int32_T i;
    for (i = 0; i < 400; i++) {
      /* Outport: '<Root>/Output 2' */
      Filter_Y.Output2[i] = Filter_B.Stage9[i];
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S2>/Stage1' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)&Filter_U.Channel3[0];
    real32_T *outputs = (real32_T *)Filter_B.Stage9;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage1_RTP1COEFF_m[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage1_RTP2COEFF_h[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage1_FILT_STATES_e[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 26 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 13; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S2>/Stage2' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage9;
    real32_T *outputs = (real32_T *)Filter_B.Stage10;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage2_RTP1COEFF_h[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage2_RTP2COEFF_f[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage2_FILT_STATES_a[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 26 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 13; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S2>/Stage3' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage10;
    real32_T *outputs = (real32_T *)Filter_B.Stage9;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage3_RTP1COEFF_j[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage3_RTP2COEFF_d[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage3_FILT_STATES_c[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 26 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 13; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S2>/Stage4' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage9;
    real32_T *outputs = (real32_T *)Filter_B.Stage10;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage4_RTP1COEFF_l[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage4_RTP2COEFF_b[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage4_FILT_STATES_n[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 26 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 13; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S2>/Stage5' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage10;
    real32_T *outputs = (real32_T *)Filter_B.Stage9;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage5_RTP1COEFF_j[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage5_RTP2COEFF_d[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage5_FILT_STATES_f[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 26 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 13; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S2>/Stage6' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage9;
    real32_T *outputs = (real32_T *)Filter_B.Stage10;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage6_RTP1COEFF_o[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage6_RTP2COEFF_h[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage6_FILT_STATES_b[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 26 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 13; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S2>/Stage7' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage10;
    real32_T *outputs = (real32_T *)Filter_B.Stage9;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage7_RTP1COEFF_n[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage7_RTP2COEFF_o[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage7_FILT_STATES_a[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 26 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 13; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S2>/Stage8' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage9;
    real32_T *outputs = (real32_T *)Filter_B.Stage10;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage8_RTP1COEFF_h[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage8_RTP2COEFF_c[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage8_FILT_STATES_o[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 24 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 12; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S2>/Stage9' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage10;
    real32_T *outputs = (real32_T *)Filter_B.Stage9;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage9_RTP1COEFF_o[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage9_RTP2COEFF_o[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage9_FILT_STATES_p[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 24 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 12; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S2>/Stage10' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage9;
    real32_T *outputs = (real32_T *)Filter_B.Stage10;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage10_RTP1COEFF_o[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage10_RTP2COEFF_g[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage10_FILT_STATES_f[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 24 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 12; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  {
    int32_T i;
    for (i = 0; i < 400; i++) {
      /* Outport: '<Root>/Output 3' */
      Filter_Y.Output3[i] = Filter_B.Stage10[i];
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S3>/Stage1' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)&Filter_U.Channel4[0];
    real32_T *outputs = (real32_T *)Filter_B.Stage9;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage1_RTP1COEFF_o[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage1_RTP2COEFF_n[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage1_FILT_STATES_j[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 26 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 13; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S3>/Stage2' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage9;
    real32_T *outputs = (real32_T *)Filter_B.Stage10;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage2_RTP1COEFF_n[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage2_RTP2COEFF_m[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage2_FILT_STATES_f[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 26 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 13; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S3>/Stage3' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage10;
    real32_T *outputs = (real32_T *)Filter_B.Stage9;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage3_RTP1COEFF_i[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage3_RTP2COEFF_p[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage3_FILT_STATES_d[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 26 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 13; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S3>/Stage4' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage9;
    real32_T *outputs = (real32_T *)Filter_B.Stage10;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage4_RTP1COEFF_d[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage4_RTP2COEFF_n[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage4_FILT_STATES_o[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 26 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 13; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S3>/Stage5' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage10;
    real32_T *outputs = (real32_T *)Filter_B.Stage9;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage5_RTP1COEFF_jd[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage5_RTP2COEFF_i[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage5_FILT_STATES_l[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 26 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 13; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S3>/Stage6' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage9;
    real32_T *outputs = (real32_T *)Filter_B.Stage10;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage6_RTP1COEFF_j[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage6_RTP2COEFF_n[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage6_FILT_STATES_m[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 26 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 13; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S3>/Stage7' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage10;
    real32_T *outputs = (real32_T *)Filter_B.Stage9;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage7_RTP1COEFF_i[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage7_RTP2COEFF_b[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage7_FILT_STATES_d[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 26 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 13; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S3>/Stage8' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage9;
    real32_T *outputs = (real32_T *)Filter_B.Stage10;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage8_RTP1COEFF_l[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage8_RTP2COEFF_n[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage8_FILT_STATES_a[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 24 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 12; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S3>/Stage9' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage10;
    real32_T *outputs = (real32_T *)Filter_B.Stage9;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage9_RTP1COEFF_b[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage9_RTP2COEFF_i[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage9_FILT_STATES_p3[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 24 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 12; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S3>/Stage10' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage9;
    real32_T *outputs = (real32_T *)Filter_B.Stage10;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage10_RTP1COEFF_a[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage10_RTP2COEFF_a[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage10_FILT_STATES_m[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 24 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 12; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  {
    int32_T i;
    for (i = 0; i < 400; i++) {
      /* Outport: '<Root>/Output 4' */
      Filter_Y.Output4[i] = Filter_B.Stage10[i];
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S4>/Stage1' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)&Filter_U.Channel5[0];
    real32_T *outputs = (real32_T *)Filter_B.Stage9;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage1_RTP1COEFF_f[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage1_RTP2COEFF_no[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage1_FILT_STATES_k[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 26 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 13; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S4>/Stage2' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage9;
    real32_T *outputs = (real32_T *)Filter_B.Stage10;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage2_RTP1COEFF_l[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage2_RTP2COEFF_e[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage2_FILT_STATES_j[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 26 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 13; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S4>/Stage3' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage10;
    real32_T *outputs = (real32_T *)Filter_B.Stage9;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage3_RTP1COEFF_o[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage3_RTP2COEFF_p1[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage3_FILT_STATES_ci[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 26 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 13; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S4>/Stage4' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage9;
    real32_T *outputs = (real32_T *)Filter_B.Stage10;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage4_RTP1COEFF_n[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage4_RTP2COEFF_k[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage4_FILT_STATES_h[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 26 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 13; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S4>/Stage5' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage10;
    real32_T *outputs = (real32_T *)Filter_B.Stage9;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage5_RTP1COEFF_n[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage5_RTP2COEFF_b[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage5_FILT_STATES_o[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 26 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 13; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S4>/Stage6' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage9;
    real32_T *outputs = (real32_T *)Filter_B.Stage10;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage6_RTP1COEFF_h[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage6_RTP2COEFF_k[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage6_FILT_STATES_e[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 26 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 13; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S4>/Stage7' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage10;
    real32_T *outputs = (real32_T *)Filter_B.Stage9;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage7_RTP1COEFF_m[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage7_RTP2COEFF_f[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage7_FILT_STATES_j[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 26 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 13; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S4>/Stage8' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage9;
    real32_T *outputs = (real32_T *)Filter_B.Stage10;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage8_RTP1COEFF_e[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage8_RTP2COEFF_m[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage8_FILT_STATES_m[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 24 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 12; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S4>/Stage9' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage10;
    real32_T *outputs = (real32_T *)Filter_B.Stage9;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage9_RTP1COEFF_i[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage9_RTP2COEFF_i3[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage9_FILT_STATES_a[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 24 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 12; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S4>/Stage10' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage9;
    real32_T *outputs = (real32_T *)Filter_B.Stage10;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage10_RTP1COEFF_h[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage10_RTP2COEFF_am[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage10_FILT_STATES_a[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 24 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 12; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  {
    int32_T i;
    for (i = 0; i < 400; i++) {
      /* Outport: '<Root>/Output 5' */
      Filter_Y.Output5[i] = Filter_B.Stage10[i];
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S5>/Stage1' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)&Filter_U.Channel6[0];
    real32_T *outputs = (real32_T *)Filter_B.Stage9;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage1_RTP1COEFF_g[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage1_RTP2COEFF_e[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage1_FILT_STATES_j3[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 26 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 13; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S5>/Stage2' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage9;
    real32_T *outputs = (real32_T *)Filter_B.Stage10;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage2_RTP1COEFF_j[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage2_RTP2COEFF_o[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage2_FILT_STATES_n[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 26 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 13; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S5>/Stage3' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage10;
    real32_T *outputs = (real32_T *)Filter_B.Stage9;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage3_RTP1COEFF_b[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage3_RTP2COEFF_e[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage3_FILT_STATES_j[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 26 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 13; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S5>/Stage4' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage9;
    real32_T *outputs = (real32_T *)Filter_B.Stage10;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage4_RTP1COEFF_f[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage4_RTP2COEFF_n4[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage4_FILT_STATES_p[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 26 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 13; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S5>/Stage5' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage10;
    real32_T *outputs = (real32_T *)Filter_B.Stage9;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage5_RTP1COEFF_o[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage5_RTP2COEFF_e[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage5_FILT_STATES_n[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 26 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 13; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S5>/Stage6' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage9;
    real32_T *outputs = (real32_T *)Filter_B.Stage10;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage6_RTP1COEFF_hm[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage6_RTP2COEFF_hr[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage6_FILT_STATES_bo[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 26 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 13; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S5>/Stage7' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage10;
    real32_T *outputs = (real32_T *)Filter_B.Stage9;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage7_RTP1COEFF_f[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage7_RTP2COEFF_e[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage7_FILT_STATES_h[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 26 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 13; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S5>/Stage8' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage9;
    real32_T *outputs = (real32_T *)Filter_B.Stage10;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage8_RTP1COEFF_c[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage8_RTP2COEFF_cb[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage8_FILT_STATES_n[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 24 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 12; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S5>/Stage9' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage10;
    real32_T *outputs = (real32_T *)Filter_B.Stage9;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage9_RTP1COEFF_g[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage9_RTP2COEFF_b[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage9_FILT_STATES_l[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 24 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 12; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S5>/Stage10' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage9;
    real32_T *outputs = (real32_T *)Filter_B.Stage10;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage10_RTP1COEFF_i[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage10_RTP2COEFF_h[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage10_FILT_STATES_e[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 24 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 12; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  {
    int32_T i;
    for (i = 0; i < 400; i++) {
      /* Outport: '<Root>/Output 6' */
      Filter_Y.Output6[i] = Filter_B.Stage10[i];
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S6>/Stage1' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)&Filter_U.Channel7[0];
    real32_T *outputs = (real32_T *)Filter_B.Stage9;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage1_RTP1COEFF_h[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage1_RTP2COEFF_l[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage1_FILT_STATES_a[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 26 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 13; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S6>/Stage2' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage9;
    real32_T *outputs = (real32_T *)Filter_B.Stage10;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage2_RTP1COEFF_c[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage2_RTP2COEFF_p[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage2_FILT_STATES_o[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 26 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 13; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S6>/Stage3' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage10;
    real32_T *outputs = (real32_T *)Filter_B.Stage9;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage3_RTP1COEFF_ja[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage3_RTP2COEFF_h[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage3_FILT_STATES_h[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 26 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 13; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S6>/Stage4' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage9;
    real32_T *outputs = (real32_T *)Filter_B.Stage10;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage4_RTP1COEFF_a[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage4_RTP2COEFF_c[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage4_FILT_STATES_f[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 26 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 13; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S6>/Stage5' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage10;
    real32_T *outputs = (real32_T *)Filter_B.Stage9;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage5_RTP1COEFF_l[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage5_RTP2COEFF_f[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage5_FILT_STATES_e[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 26 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 13; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S6>/Stage6' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage9;
    real32_T *outputs = (real32_T *)Filter_B.Stage10;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage6_RTP1COEFF_a[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage6_RTP2COEFF_b[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage6_FILT_STATES_i[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 26 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 13; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S6>/Stage7' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage10;
    real32_T *outputs = (real32_T *)Filter_B.Stage9;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage7_RTP1COEFF_d[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage7_RTP2COEFF_h[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage7_FILT_STATES_m[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 26 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 13; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S6>/Stage8' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage9;
    real32_T *outputs = (real32_T *)Filter_B.Stage10;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage8_RTP1COEFF_d[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage8_RTP2COEFF_g[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage8_FILT_STATES_nh[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 24 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 12; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S6>/Stage9' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage10;
    real32_T *outputs = (real32_T *)Filter_B.Stage9;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage9_RTP1COEFF_a[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage9_RTP2COEFF_j[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage9_FILT_STATES_n[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 24 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 12; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S6>/Stage10' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage9;
    real32_T *outputs = (real32_T *)Filter_B.Stage10;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage10_RTP1COEFF_b[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage10_RTP2COEFF_f[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage10_FILT_STATES_k[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 24 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 12; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  {
    int32_T i;
    for (i = 0; i < 400; i++) {
      /* Outport: '<Root>/Output 7' */
      Filter_Y.Output7[i] = Filter_B.Stage10[i];
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S7>/Stage1' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)&Filter_U.Channel8[0];
    real32_T *outputs = (real32_T *)Filter_B.Stage9;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage1_RTP1COEFF_c[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage1_RTP2COEFF_b[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage1_FILT_STATES_c[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 26 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 13; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S7>/Stage2' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage9;
    real32_T *outputs = (real32_T *)Filter_B.Stage10;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage2_RTP1COEFF_g[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage2_RTP2COEFF_f3[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage2_FILT_STATES_e[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 26 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 13; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S7>/Stage3' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage10;
    real32_T *outputs = (real32_T *)Filter_B.Stage9;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage3_RTP1COEFF_a[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage3_RTP2COEFF_m[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage3_FILT_STATES_k[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 26 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 13; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S7>/Stage4' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage9;
    real32_T *outputs = (real32_T *)Filter_B.Stage10;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage4_RTP1COEFF_d3[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage4_RTP2COEFF_o[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage4_FILT_STATES_pv[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 26 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 13; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S7>/Stage5' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage10;
    real32_T *outputs = (real32_T *)Filter_B.Stage9;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage5_RTP1COEFF_f[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage5_RTP2COEFF_a[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage5_FILT_STATES_g[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 26 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 13; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S7>/Stage6' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage9;
    real32_T *outputs = (real32_T *)Filter_B.Stage10;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage6_RTP1COEFF_k[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage6_RTP2COEFF_e[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage6_FILT_STATES_j[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 26 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 13; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S7>/Stage7' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage10;
    real32_T *outputs = (real32_T *)Filter_B.Stage9;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage7_RTP1COEFF_iw[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage7_RTP2COEFF_g[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage7_FILT_STATES_e[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 26 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 13; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S7>/Stage8' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage9;
    real32_T *outputs = (real32_T *)Filter_B.Stage10;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage8_RTP1COEFF_j[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage8_RTP2COEFF_d[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage8_FILT_STATES_i[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 24 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 12; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S7>/Stage9' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage10;
    real32_T *outputs = (real32_T *)Filter_B.Stage9;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage9_RTP1COEFF_c[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage9_RTP2COEFF_l[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage9_FILT_STATES_pa[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 24 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 12; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S7>/Stage10' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    real32_T *inputs = (real32_T *)Filter_B.Stage9;
    real32_T *outputs = (real32_T *)Filter_B.Stage10;
    real32_T *coeffs = (real32_T *)&Filter_P.Stage10_RTP1COEFF_n[0];
    real32_T *scaleVals = (real32_T *)&Filter_P.Stage10_RTP2COEFF_e[0];
    int_T io_Idx = 0;
    int_T k;

    /* Loop over each input channel */
    for (k=0; k < 400; k++) {
      real32_T *states = (real32_T *)&Filter_DWork.Stage10_FILT_STATES_j[0];
      real32_T stageIn, stageOut;
      int_T memOffset = 24 * k;        /* state memory offset for this channel */
      int_T j;
      stageIn = inputs[io_Idx] * scaleVals[0];

      /* Loop over sections */
      for (j = 0; j < 12; j++) {
        int_T memIdx = memOffset + 2 * j;/* memory index for this section */
        int_T coeffIdx = 5 * j;        /* Coefficient offset for this section */
        int_T numIdx = coeffIdx;
        int_T denIdx = numIdx + 3;

        /* Accumulate denominator side products:
         * stageIn -= a(1) * state(0) - a(2) * state(1)
         */
        stageIn -= coeffs[denIdx] * states[memIdx];
        stageIn -= coeffs[denIdx+1] * states[memIdx+1];

        /* Accumulate numerator side products:
         * stageOut = b(0) * stageIn + b(1) * state(0) + b(2) * state(1)
         */
        stageOut = coeffs[numIdx] * stageIn;
        stageOut += coeffs[numIdx+1] * states[memIdx];
        stageOut += coeffs[numIdx+2] * states[memIdx+1];

        /* update states */
        states[memIdx+1] = states[memIdx];
        states[memIdx] = stageIn;

        /* update stage input for next stage */
        stageIn = stageOut * scaleVals[j+1];
      }                                /* section loop */

      outputs[io_Idx] = stageIn;
      io_Idx++;
    }                                  /* channel loop */
  }

  {
    int32_T i;
    for (i = 0; i < 400; i++) {
      /* Outport: '<Root>/Output 8' */
      Filter_Y.Output8[i] = Filter_B.Stage10[i];
    }
  }
}

/* Model initialize function */
void Filter_initialize(void)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatus(Filter_M, (const char_T *)0);

  /* block I/O */
  {
    int_T i;
    void *pVoidBlockIORegion;
    pVoidBlockIORegion = (void *)(&Filter_B.Stage9[0]);
    for (i = 0; i < 800; i++) {
      ((real32_T*)pVoidBlockIORegion)[i] = 0.0F;
    }
  }

  /* states (dwork) */
  {
    int_T i;
    real32_T *dwork_ptr = (real32_T *) &Filter_DWork.MultiBandFilter_FILT_STATES
      [0];
    for (i = 0; i < 712800; i++) {
      dwork_ptr[i] = 0.0F;
    }
  }

  /* external inputs */
  {
    int_T i;
    for (i = 0; i < 400; i++) {
      Filter_U.Channel1[i] = 0.0F;
    }
  }

  {
    int_T i;
    for (i = 0; i < 400; i++) {
      Filter_U.Channel2[i] = 0.0F;
    }
  }

  {
    int_T i;
    for (i = 0; i < 400; i++) {
      Filter_U.Channel3[i] = 0.0F;
    }
  }

  {
    int_T i;
    for (i = 0; i < 400; i++) {
      Filter_U.Channel4[i] = 0.0F;
    }
  }

  {
    int_T i;
    for (i = 0; i < 400; i++) {
      Filter_U.Channel5[i] = 0.0F;
    }
  }

  {
    int_T i;
    for (i = 0; i < 400; i++) {
      Filter_U.Channel6[i] = 0.0F;
    }
  }

  {
    int_T i;
    for (i = 0; i < 400; i++) {
      Filter_U.Channel7[i] = 0.0F;
    }
  }

  {
    int_T i;
    for (i = 0; i < 400; i++) {
      Filter_U.Channel8[i] = 0.0F;
    }
  }

  /* external outputs */
  {
    int_T i;
    for (i = 0; i < 400; i++) {
      Filter_Y.Output1[i] = 0.0F;
    }
  }

  {
    int_T i;
    for (i = 0; i < 400; i++) {
      Filter_Y.Output2[i] = 0.0F;
    }
  }

  {
    int_T i;
    for (i = 0; i < 400; i++) {
      Filter_Y.Output3[i] = 0.0F;
    }
  }

  {
    int_T i;
    for (i = 0; i < 400; i++) {
      Filter_Y.Output4[i] = 0.0F;
    }
  }

  {
    int_T i;
    for (i = 0; i < 400; i++) {
      Filter_Y.Output5[i] = 0.0F;
    }
  }

  {
    int_T i;
    for (i = 0; i < 400; i++) {
      Filter_Y.Output6[i] = 0.0F;
    }
  }

  {
    int_T i;
    for (i = 0; i < 400; i++) {
      Filter_Y.Output7[i] = 0.0F;
    }
  }

  {
    int_T i;
    for (i = 0; i < 400; i++) {
      Filter_Y.Output8[i] = 0.0F;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<Root>/MultiBandFilter' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 1600; i++) {
      Filter_DWork.MultiBandFilter_FILT_STATES[i] =
        Filter_P.MultiBandFilter_ICRTP;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S1>/Stage1' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 10400; i++) {
      Filter_DWork.Stage1_FILT_STATES[i] = Filter_P.Stage1_ICRTP;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S1>/Stage2' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 10400; i++) {
      Filter_DWork.Stage2_FILT_STATES[i] = Filter_P.Stage2_ICRTP;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S1>/Stage3' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 10400; i++) {
      Filter_DWork.Stage3_FILT_STATES[i] = Filter_P.Stage3_ICRTP;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S1>/Stage4' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 10400; i++) {
      Filter_DWork.Stage4_FILT_STATES[i] = Filter_P.Stage4_ICRTP;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S1>/Stage5' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 10400; i++) {
      Filter_DWork.Stage5_FILT_STATES[i] = Filter_P.Stage5_ICRTP;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S1>/Stage6' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 10400; i++) {
      Filter_DWork.Stage6_FILT_STATES[i] = Filter_P.Stage6_ICRTP;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S1>/Stage7' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 10400; i++) {
      Filter_DWork.Stage7_FILT_STATES[i] = Filter_P.Stage7_ICRTP;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S1>/Stage8' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 9600; i++) {
      Filter_DWork.Stage8_FILT_STATES[i] = Filter_P.Stage8_ICRTP;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S1>/Stage9' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 9600; i++) {
      Filter_DWork.Stage9_FILT_STATES[i] = Filter_P.Stage9_ICRTP;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S1>/Stage10' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 9600; i++) {
      Filter_DWork.Stage10_FILT_STATES[i] = Filter_P.Stage10_ICRTP;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S2>/Stage1' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 10400; i++) {
      Filter_DWork.Stage1_FILT_STATES_e[i] = Filter_P.Stage1_ICRTP_j;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S2>/Stage2' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 10400; i++) {
      Filter_DWork.Stage2_FILT_STATES_a[i] = Filter_P.Stage2_ICRTP_e;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S2>/Stage3' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 10400; i++) {
      Filter_DWork.Stage3_FILT_STATES_c[i] = Filter_P.Stage3_ICRTP_o;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S2>/Stage4' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 10400; i++) {
      Filter_DWork.Stage4_FILT_STATES_n[i] = Filter_P.Stage4_ICRTP_m;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S2>/Stage5' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 10400; i++) {
      Filter_DWork.Stage5_FILT_STATES_f[i] = Filter_P.Stage5_ICRTP_m;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S2>/Stage6' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 10400; i++) {
      Filter_DWork.Stage6_FILT_STATES_b[i] = Filter_P.Stage6_ICRTP_p;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S2>/Stage7' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 10400; i++) {
      Filter_DWork.Stage7_FILT_STATES_a[i] = Filter_P.Stage7_ICRTP_o;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S2>/Stage8' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 9600; i++) {
      Filter_DWork.Stage8_FILT_STATES_o[i] = Filter_P.Stage8_ICRTP_c;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S2>/Stage9' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 9600; i++) {
      Filter_DWork.Stage9_FILT_STATES_p[i] = Filter_P.Stage9_ICRTP_p;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S2>/Stage10' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 9600; i++) {
      Filter_DWork.Stage10_FILT_STATES_f[i] = Filter_P.Stage10_ICRTP_d;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S3>/Stage1' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 10400; i++) {
      Filter_DWork.Stage1_FILT_STATES_j[i] = Filter_P.Stage1_ICRTP_g;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S3>/Stage2' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 10400; i++) {
      Filter_DWork.Stage2_FILT_STATES_f[i] = Filter_P.Stage2_ICRTP_m;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S3>/Stage3' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 10400; i++) {
      Filter_DWork.Stage3_FILT_STATES_d[i] = Filter_P.Stage3_ICRTP_h;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S3>/Stage4' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 10400; i++) {
      Filter_DWork.Stage4_FILT_STATES_o[i] = Filter_P.Stage4_ICRTP_n;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S3>/Stage5' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 10400; i++) {
      Filter_DWork.Stage5_FILT_STATES_l[i] = Filter_P.Stage5_ICRTP_a;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S3>/Stage6' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 10400; i++) {
      Filter_DWork.Stage6_FILT_STATES_m[i] = Filter_P.Stage6_ICRTP_b;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S3>/Stage7' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 10400; i++) {
      Filter_DWork.Stage7_FILT_STATES_d[i] = Filter_P.Stage7_ICRTP_g;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S3>/Stage8' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 9600; i++) {
      Filter_DWork.Stage8_FILT_STATES_a[i] = Filter_P.Stage8_ICRTP_k;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S3>/Stage9' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 9600; i++) {
      Filter_DWork.Stage9_FILT_STATES_p3[i] = Filter_P.Stage9_ICRTP_b;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S3>/Stage10' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 9600; i++) {
      Filter_DWork.Stage10_FILT_STATES_m[i] = Filter_P.Stage10_ICRTP_m;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S4>/Stage1' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 10400; i++) {
      Filter_DWork.Stage1_FILT_STATES_k[i] = Filter_P.Stage1_ICRTP_o;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S4>/Stage2' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 10400; i++) {
      Filter_DWork.Stage2_FILT_STATES_j[i] = Filter_P.Stage2_ICRTP_m3;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S4>/Stage3' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 10400; i++) {
      Filter_DWork.Stage3_FILT_STATES_ci[i] = Filter_P.Stage3_ICRTP_j;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S4>/Stage4' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 10400; i++) {
      Filter_DWork.Stage4_FILT_STATES_h[i] = Filter_P.Stage4_ICRTP_e;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S4>/Stage5' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 10400; i++) {
      Filter_DWork.Stage5_FILT_STATES_o[i] = Filter_P.Stage5_ICRTP_a3;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S4>/Stage6' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 10400; i++) {
      Filter_DWork.Stage6_FILT_STATES_e[i] = Filter_P.Stage6_ICRTP_pb;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S4>/Stage7' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 10400; i++) {
      Filter_DWork.Stage7_FILT_STATES_j[i] = Filter_P.Stage7_ICRTP_j;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S4>/Stage8' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 9600; i++) {
      Filter_DWork.Stage8_FILT_STATES_m[i] = Filter_P.Stage8_ICRTP_h;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S4>/Stage9' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 9600; i++) {
      Filter_DWork.Stage9_FILT_STATES_a[i] = Filter_P.Stage9_ICRTP_e;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S4>/Stage10' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 9600; i++) {
      Filter_DWork.Stage10_FILT_STATES_a[i] = Filter_P.Stage10_ICRTP_j;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S5>/Stage1' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 10400; i++) {
      Filter_DWork.Stage1_FILT_STATES_j3[i] = Filter_P.Stage1_ICRTP_i;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S5>/Stage2' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 10400; i++) {
      Filter_DWork.Stage2_FILT_STATES_n[i] = Filter_P.Stage2_ICRTP_i;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S5>/Stage3' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 10400; i++) {
      Filter_DWork.Stage3_FILT_STATES_j[i] = Filter_P.Stage3_ICRTP_c;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S5>/Stage4' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 10400; i++) {
      Filter_DWork.Stage4_FILT_STATES_p[i] = Filter_P.Stage4_ICRTP_j;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S5>/Stage5' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 10400; i++) {
      Filter_DWork.Stage5_FILT_STATES_n[i] = Filter_P.Stage5_ICRTP_b;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S5>/Stage6' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 10400; i++) {
      Filter_DWork.Stage6_FILT_STATES_bo[i] = Filter_P.Stage6_ICRTP_f;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S5>/Stage7' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 10400; i++) {
      Filter_DWork.Stage7_FILT_STATES_h[i] = Filter_P.Stage7_ICRTP_l;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S5>/Stage8' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 9600; i++) {
      Filter_DWork.Stage8_FILT_STATES_n[i] = Filter_P.Stage8_ICRTP_g;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S5>/Stage9' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 9600; i++) {
      Filter_DWork.Stage9_FILT_STATES_l[i] = Filter_P.Stage9_ICRTP_a;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S5>/Stage10' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 9600; i++) {
      Filter_DWork.Stage10_FILT_STATES_e[i] = Filter_P.Stage10_ICRTP_l;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S6>/Stage1' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 10400; i++) {
      Filter_DWork.Stage1_FILT_STATES_a[i] = Filter_P.Stage1_ICRTP_n;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S6>/Stage2' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 10400; i++) {
      Filter_DWork.Stage2_FILT_STATES_o[i] = Filter_P.Stage2_ICRTP_j;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S6>/Stage3' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 10400; i++) {
      Filter_DWork.Stage3_FILT_STATES_h[i] = Filter_P.Stage3_ICRTP_c5;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S6>/Stage4' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 10400; i++) {
      Filter_DWork.Stage4_FILT_STATES_f[i] = Filter_P.Stage4_ICRTP_mz;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S6>/Stage5' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 10400; i++) {
      Filter_DWork.Stage5_FILT_STATES_e[i] = Filter_P.Stage5_ICRTP_h;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S6>/Stage6' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 10400; i++) {
      Filter_DWork.Stage6_FILT_STATES_i[i] = Filter_P.Stage6_ICRTP_e;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S6>/Stage7' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 10400; i++) {
      Filter_DWork.Stage7_FILT_STATES_m[i] = Filter_P.Stage7_ICRTP_d;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S6>/Stage8' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 9600; i++) {
      Filter_DWork.Stage8_FILT_STATES_nh[i] = Filter_P.Stage8_ICRTP_e;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S6>/Stage9' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 9600; i++) {
      Filter_DWork.Stage9_FILT_STATES_n[i] = Filter_P.Stage9_ICRTP_j;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S6>/Stage10' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 9600; i++) {
      Filter_DWork.Stage10_FILT_STATES_k[i] = Filter_P.Stage10_ICRTP_j4;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S7>/Stage1' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 10400; i++) {
      Filter_DWork.Stage1_FILT_STATES_c[i] = Filter_P.Stage1_ICRTP_jt;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S7>/Stage2' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 10400; i++) {
      Filter_DWork.Stage2_FILT_STATES_e[i] = Filter_P.Stage2_ICRTP_d;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S7>/Stage3' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 10400; i++) {
      Filter_DWork.Stage3_FILT_STATES_k[i] = Filter_P.Stage3_ICRTP_m;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S7>/Stage4' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 10400; i++) {
      Filter_DWork.Stage4_FILT_STATES_pv[i] = Filter_P.Stage4_ICRTP_k;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S7>/Stage5' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 10400; i++) {
      Filter_DWork.Stage5_FILT_STATES_g[i] = Filter_P.Stage5_ICRTP_j;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S7>/Stage6' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 10400; i++) {
      Filter_DWork.Stage6_FILT_STATES_j[i] = Filter_P.Stage6_ICRTP_j;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S7>/Stage7' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 10400; i++) {
      Filter_DWork.Stage7_FILT_STATES_e[i] = Filter_P.Stage7_ICRTP_k;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S7>/Stage8' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 9600; i++) {
      Filter_DWork.Stage8_FILT_STATES_i[i] = Filter_P.Stage8_ICRTP_e5;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S7>/Stage9' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 9600; i++) {
      Filter_DWork.Stage9_FILT_STATES_pa[i] = Filter_P.Stage9_ICRTP_h;
    }
  }

  /* Signal Processing Blockset Filter Implementation (sdspfilter2) - '<S7>/Stage10' */
  /* Biquad (second order sections), Direct-form II (a0 == 1) */
  {
    int_T i;
    for (i = 0; i < 9600; i++) {
      Filter_DWork.Stage10_FILT_STATES_j[i] = Filter_P.Stage10_ICRTP_p;
    }
  }
}

/* Model terminate function */
void Filter_terminate(void)
{
  /* (no terminate code required) */
}

/* File trailer for Real-Time Workshop generated code.
 *
 * [EOF]
 */
