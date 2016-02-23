/*
   ITU-T G.729A Annex B     ANSI-C Source Code
   Version 1.3    Last modified: August 1997
   Copyright (c) 1996, France Telecom, Rockwell International,
                       Universite de Sherbrooke.
   All rights reserved.
*/

#define         TRUE 1
#define         FALSE 0
#define         sqr(a)  ((a)*(a))
#define         R_LSFQ 10

void Init_lsfq_noise(void);
void lsfq_noise(Word16 *lsp_new, Word16 *lspq,
                Word16 freq_prev[MA_NP][M], Word16 *idx);
void sid_lsfq_decode(Word16 *index, Word16 *lspq,
                     Word16 freq_prev[MA_NP][M]);





