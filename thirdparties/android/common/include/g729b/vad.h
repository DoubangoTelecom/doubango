/*
   ITU-T G.729A Annex B     ANSI-C Source Code
   Version 1.3    Last modified: August 1997
   Copyright (c) 1996, France Telecom, Rockwell International,
                       Universite de Sherbrooke.
   All rights reserved.
*/

#define     NP            12                  /* Increased LPC order */
#define     NOISE         0
#define     VOICE         1
#define     INIT_FRAME    32
#define     INIT_COUNT    20
#define     ZC_START      120
#define     ZC_END        200

void vad_init(void);

void vad(
    Word16 rc,
    Word16 *lsf,
    Word16 *r_h,
    Word16 *r_l,
    Word16 exp_R0,
    Word16 *sigpp,
    Word16 frm_count,
    Word16 prev_marker,
    Word16 pprev_marker,
    Word16 *marker);

