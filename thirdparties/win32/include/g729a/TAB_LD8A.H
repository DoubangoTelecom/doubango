/*
   ITU-T G.729A Speech Coder    ANSI-C Source Code
   Version 1.1    Last modified: September 1996

   Copyright (c) 1996,
   AT&T, France Telecom, NTT, Universite de Sherbrooke
   All rights reserved.
*/

extern Word16 hamwindow[L_WINDOW];
extern Word16 lag_h[M];
extern Word16 lag_l[M];
extern Word16 table[65];
extern Word16 slope[64];
extern Word16 table2[64];
extern Word16 slope_cos[64];
extern Word16 slope_acos[64];
extern Word16 lspcb1[NC0][M];
extern Word16 lspcb2[NC1][M];
extern Word16 fg[2][MA_NP][M];
extern Word16 fg_sum[2][M];
extern Word16 fg_sum_inv[2][M];
extern Word16 grid[GRID_POINTS+1];
extern Word16 inter_3l[FIR_SIZE_SYN];
extern Word16 pred[4];
extern Word16 gbk1[NCODE1][2];
extern Word16 gbk2[NCODE2][2];
extern Word16 map1[NCODE1];
extern Word16 map2[NCODE2];
extern Word16 coef[2][2];
extern Word32 L_coef[2][2];
extern Word16 thr1[NCODE1-NCAN1];
extern Word16 thr2[NCODE2-NCAN2];
extern Word16 imap1[NCODE1];
extern Word16 imap2[NCODE2];
extern Word16 b100[3];
extern Word16 a100[3];
extern Word16 b140[3];
extern Word16 a140[3];
extern Word16 bitsno[PRM_SIZE];
extern Word16 tabpow[33];
extern Word16 tablog[33];
extern Word16 tabsqr[49];
extern Word16 tab_zone[PIT_MAX+L_INTERPOL-1];

