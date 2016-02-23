/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
*
* This file is part of Open Source Doubango Framework.
*
* DOUBANGO is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* DOUBANGO is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

/**@file tsip_timers.h
 * @brief SIP timers.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef TINYSIP_TIMERS_H
#define TINYSIP_TIMERS_H

#include "tinysip_config.h"

#include "tsk_timer.h"

TSIP_BEGIN_DECLS

#define TSIP_TIMER_GET(TX)	tsip_timers_get##TX()

//extern const char *timerT1;
//extern const char *timerT2;
//extern const char *timerT4;
//extern const char *timerA;
//extern const char *timerB;
//extern const char *timerC;
//extern const char *timerD;
//extern const char *timerE;
//extern const char *timerF;
//extern const char *timerG;
//extern const char *timerH;
//extern const char *timerI;
//extern const char *timerJ;
//extern const char *timerK;
//extern const char *timerL;
//extern const char *timerM;

typedef struct tsip_timer_s {
    tsk_timer_id_t id;
    uint64_t timeout;
}
tsip_timer_t;

typedef struct tsip_timers_s {
    uint32_t T1;
    uint32_t T2;
    uint32_t T4;
    uint32_t A;
    uint32_t B;
    uint32_t C;
    uint32_t D;
    uint32_t E;
    uint32_t F;
    uint32_t G;
    uint32_t H;
    uint32_t I;
    uint32_t J;
    uint32_t K;
    uint32_t L;
    uint32_t M;
}
tsip_timers_t;

TINYSIP_API void tsip_timers_setT1(uint32_t t1);
TINYSIP_API void tsip_timers_setT2(uint32_t t2);
TINYSIP_API void tsip_timers_setT4(uint32_t t4);
TINYSIP_API void tsip_timers_setA(uint32_t a);
TINYSIP_API void tsip_timers_setB(uint32_t b);
TINYSIP_API void tsip_timers_setC(uint32_t c);
TINYSIP_API void tsip_timers_setD(uint32_t d);
TINYSIP_API void tsip_timers_setE(uint32_t e);
TINYSIP_API void tsip_timers_setF(uint32_t f);
TINYSIP_API void tsip_timers_setG(uint32_t g);
TINYSIP_API void tsip_timers_setH(uint32_t h);
TINYSIP_API void tsip_timers_setI(uint32_t i);
TINYSIP_API void tsip_timers_setJ(uint32_t j);
TINYSIP_API void tsip_timers_setK(uint32_t k);
TINYSIP_API void tsip_timers_setL(uint32_t l);
TINYSIP_API void tsip_timers_setM(uint32_t m);

TINYSIP_API uint32_t tsip_timers_getT1();
TINYSIP_API uint32_t tsip_timers_getT2();
TINYSIP_API uint32_t tsip_timers_getT4();
TINYSIP_API uint32_t tsip_timers_getA();
TINYSIP_API uint32_t tsip_timers_getB();
TINYSIP_API uint32_t tsip_timers_getC();
TINYSIP_API uint32_t tsip_timers_getD();
TINYSIP_API uint32_t tsip_timers_getE();
TINYSIP_API uint32_t tsip_timers_getF();
TINYSIP_API uint32_t tsip_timers_getG();
TINYSIP_API uint32_t tsip_timers_getH();
TINYSIP_API uint32_t tsip_timers_getI();
TINYSIP_API uint32_t tsip_timers_getJ();
TINYSIP_API uint32_t tsip_timers_getK();
TINYSIP_API uint32_t tsip_timers_getL();
TINYSIP_API uint32_t tsip_timers_getM();

TSIP_END_DECLS

#endif /* TINYSIP_TIMERS_H */

