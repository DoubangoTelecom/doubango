/*
   ITU-T G.729A Speech Coder    ANSI-C Source Code
   Version 1.1    Last modified: September 1996

   Copyright (c) 1996,
   AT&T, France Telecom, NTT, Universite de Sherbrooke
   All rights reserved.
*/

/*---------------------------------------------------------------*
 * LD8A.H                                                        *
 * ~~~~~~                                                        *
 * Function prototypes and constants use for G.729A 8kb/s coder. *
 *                                                               *
 *---------------------------------------------------------------*/

/*--------------------------------------------------------------------------*
 *       Codec constant parameters (coder, decoder, and postfilter)         *
 *--------------------------------------------------------------------------*/

#define  L_TOTAL      240     /* Total size of speech buffer.               */
#define  L_WINDOW     240     /* Window size in LP analysis.                */
#define  L_NEXT       40      /* Lookahead in LP analysis.                  */
#define  L_FRAME      80      /* Frame size.                                */
#define  L_SUBFR      40      /* Subframe size.                             */
#define  M            10      /* Order of LP filter.                        */
#define  MP1          (M+1)   /* Order of LP filter + 1                     */
#define  PIT_MIN      20      /* Minimum pitch lag.                         */
#define  PIT_MAX      143     /* Maximum pitch lag.                         */
#define  L_INTERPOL   (10+1)  /* Length of filter for interpolation.        */
#define  GAMMA1       24576   /* Bandwitdh factor = 0.75   in Q15           */

#define  PRM_SIZE     11      /* Size of vector of analysis parameters.     */
#define  SERIAL_SIZE  (80+2)  /* bfi+ number of speech bits                 */

#define SHARPMAX  13017   /* Maximum value of pitch sharpening     0.8  Q14 */
#define SHARPMIN  3277    /* Minimum value of pitch sharpening     0.2  Q14 */


/*-------------------------------*
 * Mathematic functions.         *
 *-------------------------------*/

Word32 Inv_sqrt(   /* (o) Q30 : output value   (range: 0<=val<1)           */
  Word32 L_x       /* (i) Q0  : input value    (range: 0<=val<=7fffffff)   */
);

void Log2(
  Word32 L_x,       /* (i) Q0 : input value                                 */
  Word16 *exponent, /* (o) Q0 : Integer part of Log2.   (range: 0<=val<=30) */
  Word16 *fraction  /* (o) Q15: Fractionnal part of Log2. (range: 0<=val<1) */
);

Word32 Pow2(        /* (o) Q0  : result       (range: 0<=val<=0x7fffffff) */
  Word16 exponent,  /* (i) Q0  : Integer part.      (range: 0<=val<=30)   */
  Word16 fraction   /* (i) Q15 : Fractionnal part.  (range: 0.0<=val<1.0) */
);

/*-------------------------------*
 * Pre and post-process.         *
 *-------------------------------*/

void Init_Pre_Process(void);
void Init_Post_Process(void);

void Pre_Process(
  Word16 signal[],   /* Input/output signal */
  Word16 lg          /* Length of signal    */
);

void Post_Process(
 Word16 signal[],   /* Input/output signal */
 Word16 lg          /* Length of signal    */
);

/*----------------------------------*
 * Main coder and decoder functions *
 *----------------------------------*/

void Init_Coder_ld8a(void);

void Coder_ld8a(
 Word16 ana[]       /* output  : Analysis parameters */
);

void Init_Decod_ld8a(void);

void Decod_ld8a(
  Word16  parm[],      /* (i)   : vector of synthesis parameters
                                  parm[0] = bad frame indicator (bfi)  */
  Word16  synth[],     /* (o)   : synthesis speech                     */
  Word16  A_t[],       /* (o)   : decoded LP filter in 2 subframes     */
  Word16  *T2          /* (o)   : decoded pitch lag in 2 subframes     */
);

/*-------------------------------*
 * LPC analysis and filtering.   *
 *-------------------------------*/

void Autocorr(
  Word16 x[],      /* (i)    : Input signal                      */
  Word16 m,        /* (i)    : LPC order                         */
  Word16 r_h[],    /* (o)    : Autocorrelations  (msb)           */
  Word16 r_l[]     /* (o)    : Autocorrelations  (lsb)           */
);

void Lag_window(
  Word16 m,         /* (i)     : LPC order                        */
  Word16 r_h[],     /* (i/o)   : Autocorrelations  (msb)          */
  Word16 r_l[]      /* (i/o)   : Autocorrelations  (lsb)          */
);

void Levinson(
  Word16 Rh[],      /* (i)     : Rh[m+1] Vector of autocorrelations (msb) */
  Word16 Rl[],      /* (i)     : Rl[m+1] Vector of autocorrelations (lsb) */
  Word16 A[],       /* (o) Q12 : A[m]    LPC coefficients  (m = 10)       */
  Word16 rc[]       /* (o) Q15 : rc[M]   Relection coefficients.          */
);

void Az_lsp(
  Word16 a[],        /* (i) Q12 : predictor coefficients              */
  Word16 lsp[],      /* (o) Q15 : line spectral pairs                 */
  Word16 old_lsp[]   /* (i)     : old lsp[] (in case not found 10 roots) */
);

void Lsp_Az(
  Word16 lsp[],    /* (i) Q15 : line spectral frequencies            */
  Word16 a[]       /* (o) Q12 : predictor coefficients (order = 10)  */
);

void Lsf_lsp(
  Word16 lsf[],    /* (i) Q15 : lsf[m] normalized (range: 0.0<=val<=0.5) */
  Word16 lsp[],    /* (o) Q15 : lsp[m] (range: -1<=val<1)                */
  Word16 m         /* (i)     : LPC order                                */
);

void Lsp_lsf(
  Word16 lsp[],    /* (i) Q15 : lsp[m] (range: -1<=val<1)                */
  Word16 lsf[],    /* (o) Q15 : lsf[m] normalized (range: 0.0<=val<=0.5) */
  Word16 m         /* (i)     : LPC order                                */
);

void Int_qlpc(
 Word16 lsp_old[], /* input : LSP vector of past frame              */
 Word16 lsp_new[], /* input : LSP vector of present frame           */
 Word16 Az[]       /* output: interpolated Az() for the 2 subframes */
);

void Weight_Az(
  Word16 a[],      /* (i) Q12 : a[m+1]  LPC coefficients             */
  Word16 gamma,    /* (i) Q15 : Spectral expansion factor.           */
  Word16 m,        /* (i)     : LPC order.                           */
  Word16 ap[]      /* (o) Q12 : Spectral expanded LPC coefficients   */
);

void Residu(
  Word16 a[],    /* (i) Q12 : prediction coefficients                     */
  Word16 x[],    /* (i)     : speech (values x[-m..-1] are needed (m=10)  */
  Word16 y[],    /* (o)     : residual signal                             */
  Word16 lg      /* (i)     : size of filtering                           */
);

void Syn_filt(
  Word16 a[],     /* (i) Q12 : a[m+1] prediction coefficients   (m=10)  */
  Word16 x[],     /* (i)     : input signal                             */
  Word16 y[],     /* (o)     : output signal                            */
  Word16 lg,      /* (i)     : size of filtering                        */
  Word16 mem[],   /* (i/o)   : memory associated with this filtering.   */
  Word16 update   /* (i)     : 0=no update, 1=update of memory.         */
);

void Convolve(
  Word16 x[],      /* (i)     : input vector                           */
  Word16 h[],      /* (i) Q12 : impulse response                       */
  Word16 y[],      /* (o)     : output vector                          */
  Word16 L         /* (i)     : vector size                            */
);

/*--------------------------------------------------------------------------*
 *       LTP constant parameters                                            *
 *--------------------------------------------------------------------------*/

#define UP_SAMP         3
#define L_INTER10       10
#define FIR_SIZE_SYN    (UP_SAMP*L_INTER10+1)

/*-----------------------*
 * Pitch functions.      *
 *-----------------------*/

Word16 Pitch_ol_fast(  /* output: open loop pitch lag                        */
   Word16 signal[],    /* input : signal used to compute the open loop pitch */
                       /*     signal[-pit_max] to signal[-1] should be known */
   Word16   pit_max,   /* input : maximum pitch lag                          */
   Word16   L_frame    /* input : length of frame to compute pitch           */
);

Word16 Pitch_fr3_fast(/* (o)     : pitch period.                          */
  Word16 exc[],       /* (i)     : excitation buffer                      */
  Word16 xn[],        /* (i)     : target vector                          */
  Word16 h[],         /* (i) Q12 : impulse response of filters.           */
  Word16 L_subfr,     /* (i)     : Length of subframe                     */
  Word16 t0_min,      /* (i)     : minimum value in the searched range.   */
  Word16 t0_max,      /* (i)     : maximum value in the searched range.   */
  Word16 i_subfr,     /* (i)     : indicator for first subframe.          */
  Word16 *pit_frac    /* (o)     : chosen fraction.                       */
);

Word16 G_pitch(      /* (o) Q14 : Gain of pitch lag saturated to 1.2       */
  Word16 xn[],       /* (i)     : Pitch target.                            */
  Word16 y1[],       /* (i)     : Filtered adaptive codebook.              */
  Word16 g_coeff[],  /* (i)     : Correlations need for gain quantization. */
  Word16 L_subfr     /* (i)     : Length of subframe.                      */
);

Word16 Enc_lag3(     /* output: Return index of encoding */
  Word16 T0,         /* input : Pitch delay              */
  Word16 T0_frac,    /* input : Fractional pitch delay   */
  Word16 *T0_min,    /* in/out: Minimum search delay     */
  Word16 *T0_max,    /* in/out: Maximum search delay     */
  Word16 pit_min,    /* input : Minimum pitch delay      */
  Word16 pit_max,    /* input : Maximum pitch delay      */
  Word16 pit_flag    /* input : Flag for 1st subframe    */
);

void Dec_lag3(        /* output: return integer pitch lag       */
  Word16 index,       /* input : received pitch index           */
  Word16 pit_min,     /* input : minimum pitch lag              */
  Word16 pit_max,     /* input : maximum pitch lag              */
  Word16 i_subfr,     /* input : subframe flag                  */
  Word16 *T0,         /* output: integer part of pitch lag      */
  Word16 *T0_frac     /* output: fractional part of pitch lag   */
);

Word16 Interpol_3(      /* (o)  : interpolated value  */
  Word16 *x,            /* (i)  : input vector        */
  Word16 frac           /* (i)  : fraction            */
);

void Pred_lt_3(
  Word16   exc[],       /* in/out: excitation buffer */
  Word16   T0,          /* input : integer pitch lag */
  Word16   frac,        /* input : fraction of lag   */
  Word16   L_subfr      /* input : subframe size     */
);

Word16 Parity_Pitch(    /* output: parity bit (XOR of 6 MSB bits)    */
   Word16 pitch_index   /* input : index for which parity to compute */
);

Word16  Check_Parity_Pitch( /* output: 0 = no error, 1= error */
  Word16 pitch_index,       /* input : index of parameter     */
  Word16 parity             /* input : parity bit             */
);

void Cor_h_X(
     Word16 h[],        /* (i) Q12 :Impulse response of filters      */
     Word16 X[],        /* (i)     :Target vector                    */
     Word16 D[]         /* (o)     :Correlations between h[] and D[] */
                        /*          Normalized to 13 bits            */
);

/*-----------------------*
 * Innovative codebook.  *
 *-----------------------*/

#define DIM_RR  616 /* size of correlation matrix                            */
#define NB_POS  8   /* Number of positions for each pulse                    */
#define STEP    5   /* Step betweem position of the same pulse.              */
#define MSIZE   64  /* Size of vectors for cross-correlation between 2 pulses*/

/* The following constants are Q15 fractions.
   These fractions is used to keep maximum precision on "alp" sum */

#define _1_2    (Word16)(16384)
#define _1_4    (Word16)( 8192)
#define _1_8    (Word16)( 4096)
#define _1_16   (Word16)( 2048)

Word16  ACELP_Code_A(    /* (o)     :index of pulses positions    */
  Word16 x[],            /* (i)     :Target vector                */
  Word16 h[],            /* (i) Q12 :Inpulse response of filters  */
  Word16 T0,             /* (i)     :Pitch lag                    */
  Word16 pitch_sharp,    /* (i) Q14 :Last quantized pitch gain    */
  Word16 code[],         /* (o) Q13 :Innovative codebook          */
  Word16 y[],            /* (o) Q12 :Filtered innovative codebook */
  Word16 *sign           /* (o)     :Signs of 4 pulses            */
);

void Decod_ACELP(
  Word16 sign,      /* (i)     : signs of 4 pulses.                       */
  Word16 index,     /* (i)     : Positions of the 4 pulses.               */
  Word16 cod[]      /* (o) Q13 : algebraic (fixed) codebook excitation    */
);
/*--------------------------------------------------------------------------*
 *       LSP constant parameters                                            *
 *--------------------------------------------------------------------------*/

#define   NC            5      /*  NC = M/2 */
#define   MA_NP         4      /* MA prediction order for LSP */
#define   MODE          2      /* number of modes for MA prediction */
#define   NC0_B         7      /* number of first stage bits */
#define   NC1_B         5      /* number of second stage bits */
#define   NC0           (1<<NC0_B)
#define   NC1           (1<<NC1_B)

#define   L_LIMIT          40   /* Q13:0.005 */
#define   M_LIMIT       25681   /* Q13:3.135 */

#define   GAP1          10     /* Q13 */
#define   GAP2          5      /* Q13 */
#define   GAP3          321    /* Q13 */
#define GRID_POINTS     50

#define PI04      ((Word16)1029)        /* Q13  pi*0.04 */
#define PI92      ((Word16)23677)       /* Q13  pi*0.92 */
#define CONST10   ((Word16)10*(1<<11))  /* Q11  10.0 */
#define CONST12   ((Word16)19661)       /* Q14  1.2 */

/*-------------------------------*
 * LSP VQ functions.             *
 *-------------------------------*/

void Lsf_lsp2(
  Word16 lsf[],    /* (i) Q13 : lsf[m] (range: 0.0<=val<PI) */
  Word16 lsp[],    /* (o) Q15 : lsp[m] (range: -1<=val<1)   */
  Word16 m         /* (i)     : LPC order                   */
);

void Lsp_lsf2(
  Word16 lsp[],    /* (i) Q15 : lsp[m] (range: -1<=val<1)   */
  Word16 lsf[],    /* (o) Q13 : lsf[m] (range: 0.0<=val<PI) */
  Word16 m         /* (i)     : LPC order                   */
);

void Qua_lsp(
  Word16 lsp[],       /* (i) Q15 : Unquantized LSP            */
  Word16 lsp_q[],     /* (o) Q15 : Quantized LSP              */
  Word16 ana[]        /* (o)     : indexes                    */
);

void Get_wegt(
  Word16 flsp[],    /* Q13 */
  Word16 wegt[]     /* Q11 -> normalized */
);

void Lsp_encw_reset(
  void
);

void Lsp_qua_cs(
  Word16 flsp_in[M],    /* Q13 */
  Word16 lspq_out[M],   /* Q13 */
  Word16 *code
);

void Lsp_expand_1(
  Word16 buf[],          /* Q13 */
  Word16 gap             /* Q13 */
);

void Lsp_expand_2(
  Word16 buf[],         /* Q13 */
  Word16 gap            /* Q13 */
);

void Lsp_expand_1_2(
  Word16 buf[],         /* Q13 */
  Word16 gap            /* Q13 */
);

void Lsp_get_quant(
  Word16 lspcb1[][M],      /* Q13 */
  Word16 lspcb2[][M],      /* Q13 */
  Word16 code0,
  Word16 code1,
  Word16 code2,
  Word16 fg[][M],            /* Q15 */
  Word16 freq_prev[][M],     /* Q13 */
  Word16 lspq[],                /* Q13 */
  Word16 fg_sum[]               /* Q15 */
);

void Lsp_get_tdist(
  Word16 wegt[],        /* normalized */
  Word16 buf[],         /* Q13 */
  Word32 *L_tdist,      /* Q27 */
  Word16 rbuf[],        /* Q13 */
  Word16 fg_sum[]       /* Q15 */
);

void Lsp_last_select(
  Word32 L_tdist[],     /* Q27 */
  Word16 *mode_index
);

void Lsp_pre_select(
  Word16 rbuf[],              /* Q13 */
  Word16 lspcb1[][M],      /* Q13 */
  Word16 *cand
);

void Lsp_select_1(
  Word16 rbuf[],              /* Q13 */
  Word16 lspcb1[],            /* Q13 */
  Word16 wegt[],              /* normalized */
  Word16 lspcb2[][M],      /* Q13 */
  Word16 *index
);

void Lsp_select_2(
  Word16 rbuf[],              /* Q13 */
  Word16 lspcb1[],            /* Q13 */
  Word16 wegt[],              /* normalized */
  Word16 lspcb2[][M],      /* Q13 */
  Word16 *index
);

void Lsp_stability(
  Word16 buf[]     /* Q13 */
);

void Relspwed(
  Word16 lsp[],                          /* Q13 */
  Word16 wegt[],                         /* normalized */
  Word16 lspq[],                         /* Q13 */
  Word16 lspcb1[][M],                 /* Q13 */
  Word16 lspcb2[][M],                 /* Q13 */
  Word16 fg[MODE][MA_NP][M],          /* Q15 */
  Word16 freq_prev[MA_NP][M],         /* Q13 */
  Word16 fg_sum[MODE][M],             /* Q15 */
  Word16 fg_sum_inv[MODE][M],         /* Q12 */
  Word16 code_ana[]
);

void D_lsp(
  Word16 prm[],          /* (i)     : indexes of the selected LSP */
  Word16 lsp_q[],        /* (o) Q15 : Quantized LSP parameters    */
  Word16 erase           /* (i)     : frame erase information     */
);

void Lsp_decw_reset(
  void
);

void Lsp_iqua_cs(
 Word16 prm[],          /* input : codes of the selected LSP*/
 Word16 lsp_q[],        /* output: Quantized LSP parameters*/
 Word16 erase           /* input : frame erase information */
);

void Lsp_prev_compose(
  Word16 lsp_ele[],             /* Q13 */
  Word16 lsp[],                 /* Q13 */
  Word16 fg[][M],            /* Q15 */
  Word16 freq_prev[][M],     /* Q13 */
  Word16 fg_sum[]               /* Q15 */
);

void Lsp_prev_extract(
  Word16 lsp[M],                 /* Q13 */
  Word16 lsp_ele[M],             /* Q13 */
  Word16 fg[MA_NP][M],           /* Q15 */
  Word16 freq_prev[MA_NP][M],    /* Q13 */
  Word16 fg_sum_inv[M]           /* Q12 */
);

void Lsp_prev_update(
  Word16 lsp_ele[M],             /* Q13 */
  Word16 freq_prev[MA_NP][M]     /* Q13 */
);

/*-------------------------------*
 * gain VQ constants.            *
 *-------------------------------*/

#define NCODE1_B  3                /* number of Codebook-bit */
#define NCODE2_B  4                /* number of Codebook-bit */
#define NCODE1    (1<<NCODE1_B)    /* Codebook 1 size */
#define NCODE2    (1<<NCODE2_B)    /* Codebook 2 size */
#define NCAN1     4                /* Pre-selecting order for #1 */
#define NCAN2     8                /* Pre-selecting order for #2 */
#define INV_COEF  -17103           /* Q19 */

/*--------------------------------------------------------------------------*
 * gain VQ functions.                                                       *
 *--------------------------------------------------------------------------*/

Word16 Qua_gain(
  Word16 code[],    /* (i) Q13 : Innovative vector.                         */
  Word16 g_coeff[], /* (i)     : Correlations <xn y1> -2<y1 y1>             */
                    /*            <y2,y2>, -2<xn,y2>, 2<y1,y2>              */
  Word16 exp_coeff[],/* (i)    : Q-Format g_coeff[]                         */
  Word16 L_subfr,   /* (i)     : Subframe length.                           */
  Word16 *gain_pit, /* (o) Q14 : Pitch gain.                                */
  Word16 *gain_cod, /* (o) Q1  : Code gain.                                 */
  Word16 tameflag   /* (i)     : flag set to 1 if taming is needed          */
);

void Dec_gain(
  Word16 index,     /* (i)     : Index of quantization.                     */
  Word16 code[],    /* (i) Q13 : Innovative vector.                         */
  Word16 L_subfr,   /* (i)     : Subframe length.                           */
  Word16 bfi,       /* (i)     : Bad frame indicator                        */
  Word16 *gain_pit, /* (o) Q14 : Pitch gain.                                */
  Word16 *gain_cod  /* (o) Q1  : Code gain.                                 */
);

void Gain_predict(
  Word16 past_qua_en[],/* (i) Q10 :Past quantized energies                  */
  Word16 code[],    /* (i) Q13 : Innovative vector.                         */
  Word16 L_subfr,   /* (i)     : Subframe length.                           */
  Word16 *gcode0,   /* (o) Qxx : Predicted codebook gain                    */
  Word16 *exp_gcode0 /* (o)    : Q-Format(gcode0)                           */
);

void Gain_update(
  Word16 past_qua_en[],/* (i) Q10 :Past quantized energies                  */
  Word32 L_gbk12    /* (i) Q13 : gbk1[indice1][1]+gbk2[indice2][1]          */
);

void Gain_update_erasure(
  Word16 past_qua_en[]/* (i) Q10 :Past quantized energies                   */
);

void Corr_xy2(
      Word16 xn[],           /* (i) Q0  :Target vector.                  */
      Word16 y1[],           /* (i) Q0  :Adaptive codebook.              */
      Word16 y2[],           /* (i) Q12 :Filtered innovative vector.     */
      Word16 g_coeff[],      /* (o) Q[exp]:Correlations between xn,y1,y2 */
      Word16 exp_g_coeff[]   /* (o)       :Q-format of g_coeff[]         */
);

/*-----------------------*
 * Bitstream function    *
 *-----------------------*/

void  prm2bits_ld8k(Word16 prm[], Word16 bits[]);
void  bits2prm_ld8k(Word16 bits[], Word16 prm[]);
#define BIT_0     (short)0x007f /* definition of zero-bit in bit-stream      */
#define BIT_1     (short)0x0081 /* definition of one-bit in bit-stream       */
#define SYNC_WORD (short)0x6b21 /* definition of frame erasure flag          */
#define SIZE_WORD (short)80     /* number of speech bits                     */


/*-----------------------------------*
 * Post-filter functions.            *
 *-----------------------------------*/

#define L_H 22     /* size of truncated impulse response of A(z/g1)/A(z/g2) */

#define GAMMAP      16384   /* 0.5               (Q15) */
#define INV_GAMMAP  21845   /* 1/(1+GAMMAP)      (Q15) */
#define GAMMAP_2    10923   /* GAMMAP/(1+GAMMAP) (Q15) */

#define  GAMMA2_PST 18022 /* Formant postfilt factor (numerator)   0.55 Q15 */
#define  GAMMA1_PST 22938 /* Formant postfilt factor (denominator) 0.70 Q15 */

#define  MU       26214   /* Factor for tilt compensation filter   0.8  Q15 */
#define  AGC_FAC  29491   /* Factor for automatic gain control     0.9  Q15 */
#define  AGC_FAC1 (Word16)(32767 - AGC_FAC)    /* 1-AGC_FAC in Q15          */


void Init_Post_Filter(void);

void Post_Filter(
  Word16 *syn,       /* in/out: synthesis speech (postfiltered is output)    */
  Word16 *Az_4,       /* input : interpolated LPC parameters in all subframes */
  Word16 *T            /* input : decoded pitch lags in all subframes          */
);

void pit_pst_filt(
  Word16 *signal,      /* (i)     : input signal                        */
  Word16 *scal_sig,    /* (i)     : input signal (scaled, divided by 4) */
  Word16 t0_min,       /* (i)     : minimum value in the searched range */
  Word16 t0_max,       /* (i)     : maximum value in the searched range */
  Word16 L_subfr,      /* (i)     : size of filtering                   */
  Word16 *signal_pst   /* (o)     : harmonically postfiltered signal    */
);

void preemphasis(
  Word16 *signal,  /* (i/o)   : input signal overwritten by the output */
  Word16 g,        /* (i) Q15 : preemphasis coefficient                */
  Word16 L         /* (i)     : size of filtering                      */
);

void agc(
  Word16 *sig_in,   /* (i)     : postfilter input signal  */
  Word16 *sig_out,  /* (i/o)   : postfilter output signal */
  Word16 l_trm      /* (i)     : subframe size            */
);

/*--------------------------------------------------------------------------*
 * Constants and prototypes for taming procedure.                           *
 *--------------------------------------------------------------------------*/

#define GPCLIP      15564      /* Maximum pitch gain if taming is needed Q14*/
#define GPCLIP2     481        /* Maximum pitch gain if taming is needed Q9 */
#define GP0999      16383      /* Maximum pitch gain if taming is needed    */
#define L_THRESH_ERR 983040000L /* Error threshold taming 16384. * 60000.   */

void   Init_exc_err(void);
void   update_exc_err(Word16 gain_pit, Word16 t0);
Word16 test_err(Word16 t0, Word16 t0_frac);

/*--------------------------------------------------------------------------*
 * Prototypes for auxiliary functions.                                      *
 *--------------------------------------------------------------------------*/

void Copy(
  Word16 x[],      /* (i)   : input vector   */
  Word16 y[],      /* (o)   : output vector  */
  Word16 L         /* (i)   : vector length  */
);

void Set_zero(
  Word16 x[],       /* (o)    : vector to clear     */
  Word16 L          /* (i)    : length of vector    */
);

Word16 Random(void);


