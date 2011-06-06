/*
   ITU-T G.729A Speech Coder    ANSI-C Source Code
   Version 1.1    Last modified: September 1996

   Copyright (c) 1996,
   AT&T, France Telecom, NTT, Universite de Sherbrooke, Lucent Technologies
   All rights reserved.
*/

/* Double precision operations */

void   L_Extract(Word32 L_32, Word16 *hi, Word16 *lo);
Word32 L_Comp(Word16 hi, Word16 lo);
Word32 Mpy_32(Word16 hi1, Word16 lo1, Word16 hi2, Word16 lo2);
Word32 Mpy_32_16(Word16 hi, Word16 lo, Word16 n);
Word32 Div_32(Word32 L_num, Word16 denom_hi, Word16 denom_lo);

