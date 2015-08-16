/*
   ITU-T G.729A Annex B     ANSI-C Source Code
   Version 1.3    Last modified: August 1997
   Copyright (c) 1996, France Telecom, Rockwell International,
                       Universite de Sherbrooke.
   All rights reserved.
*/

/*--------------------------------------------------------------------------*
 * Prototypes for DTX/CNG                                                   *
 *--------------------------------------------------------------------------*/

/* Encoder DTX/CNG functions */
void Init_Cod_cng(void);
void Cod_cng(
  Word16 *exc,          /* (i/o) : excitation array                     */
  Word16 pastVad,       /* (i)   : previous VAD decision                */
  Word16 *lsp_old_q,    /* (i/o) : previous quantized lsp               */
  Word16 *Aq,           /* (o)   : set of interpolated LPC coefficients */
  Word16 *ana,          /* (o)   : coded SID parameters                 */
  Word16 freq_prev[MA_NP][M],
                        /* (i/o) : previous LPS for quantization        */
  Word16 *seed          /* (i/o) : random generator seed                */
);
void Update_cng(
  Word16 *r_h,      /* (i) :   MSB of frame autocorrelation        */
  Word16 exp_r,     /* (i) :   scaling factor associated           */
  Word16 Vad        /* (i) :   current Vad decision                */
);

/* SID gain Quantization */
void Qua_Sidgain(
  Word16 *ener,     /* (i)   array of energies                   */
  Word16 *sh_ener,  /* (i)   corresponding scaling factors       */
  Word16 nb_ener,   /* (i)   number of energies or               */
  Word16 *enerq,    /* (o)   decoded energies in dB              */
  Word16 *idx       /* (o)   SID gain quantization index         */
);

/* CNG excitation generation */
void Calc_exc_rand(
  Word16 cur_gain,      /* (i)   :   target sample gain                 */
  Word16 *exc,          /* (i/o) :   excitation array                   */
  Word16 *seed,         /* (i)   :   current Vad decision               */
  Flag flag_cod         /* (i)   :   encoder/decoder flag               */
);

/* SID LSP Quantization */
void Get_freq_prev(Word16 x[MA_NP][M]);
void Update_freq_prev(Word16 x[MA_NP][M]);
void Get_decfreq_prev(Word16 x[MA_NP][M]);
void Update_decfreq_prev(Word16 x[MA_NP][M]);

/* Decoder CNG generation */
void Init_Dec_cng(void);
void Dec_cng(
  Word16 past_ftyp,     /* (i)   : past frame type                      */
  Word16 sid_sav,       /* (i)   : energy to recover SID gain           */
  Word16 sh_sid_sav,    /* (i)   : corresponding scaling factor         */
  Word16 *parm,         /* (i)   : coded SID parameters                 */
  Word16 *exc,          /* (i/o) : excitation array                     */
  Word16 *lsp_old,      /* (i/o) : previous lsp                         */
  Word16 *A_t,          /* (o)   : set of interpolated LPC coefficients */
  Word16 *seed,         /* (i/o) : random generator seed                */
  Word16 freq_prev[MA_NP][M]
                        /* (i/o) : previous LPS for quantization        */
);
Word16 read_frame(FILE *f_serial, Word16 *parm);

/*--------------------------------------------------------------------------*
 * Constants for DTX/CNG                                                    *
 *--------------------------------------------------------------------------*/

/* DTX constants */
#define FLAG_COD        (Flag)1
#define FLAG_DEC        (Flag)0
#define INIT_SEED       11111
#define FR_SID_MIN      3
#define NB_SUMACF       3
#define NB_CURACF       2
#define NB_GAIN         2
#define FRAC_THRESH1    4855
#define FRAC_THRESH2    3161
#define A_GAIN0         28672

#define SIZ_SUMACF      (NB_SUMACF * MP1)
#define SIZ_ACF         (NB_CURACF * MP1)
#define A_GAIN1         4096    /* 32768L - A_GAIN0 */

#define RATE_8000       80      /* Full rate  (8000 bit/s)       */
#define RATE_SID        15      /* SID                           */
#define RATE_0           0      /* 0 bit/s rate                  */

/* CNG excitation generation constant */
                                        /* alpha = 0.5 */
#define FRAC1           19043           /* (sqrt(40)xalpha/2 - 1) * 32768 */
#define K0              24576           /* (1 - alpha ** 2) in Q15        */
#define G_MAX           5000

