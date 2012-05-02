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

/**@file tsip_timers.c
 * @brief SIP timers.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/tsip_timers.h"

/*
	Timer    Value            Section               Meaning
	----------------------------------------------------------------------
	T1       500ms default    Section 17.1.1.1     RTT Estimate
	T2       4s               Section 17.1.2.2     The maximum retransmit
												   interval for non-INVITE
												   requests and INVITE
												   responses
	T4       5s               Section 17.1.2.2     Maximum duration a
												   message will
												   remain in the network
	Timer A  initially T1     Section 17.1.1.2     INVITE request retransmit
												   interval, for UDP only
	Timer B  64*T1            Section 17.1.1.2     INVITE transaction
												   timeout timer
	Timer C  > 3min           Section 16.6         proxy INVITE transaction
							   bullet 11            timeout
	Timer D  > 32s for UDP    Section 17.1.1.2     Wait time for response
			 0s for TCP/SCTP                       retransmits
	Timer E  initially T1     Section 17.1.2.2     non-INVITE request
												   retransmit interval,
												   UDP only
	Timer F  64*T1            Section 17.1.2.2     non-INVITE transaction
												   timeout timer
	Timer G  initially T1     Section 17.2.1       INVITE response
												   retransmit interval
	Timer H  64*T1            Section 17.2.1       Wait time for
												   ACK receipt
	Timer I  T4 for UDP       Section 17.2.1       Wait time for
			 0s for TCP/SCTP                       ACK retransmits
	Timer J  64*T1 for UDP    Section 17.2.2       Wait time for
			 0s for TCP/SCTP                       non-INVITE request
												   retransmits
	Timer K  T4 for UDP       Section 17.1.2.2     Wait time for
			 0s for TCP/SCTP                       response retransmits
	
    Timer L  64*T1             Section 17.2.1         Wait time for
                                                     accepted INVITE
                                                     request retransmits

   Timer M  64*T1             Section 17.1.1         Wait time for
                                                     retransmission of
                                                     2xx to INVITE or
                                                     additional 2xx from
                                                     other branches of
                                                     a forked INVITE
*/

#define TIMER_T1 500
#define TIMER_T4 5000

//const char *timerT1 = "timerT1";
//const char *timerT2 = "timerT2";
//const char *timerT4 = "timerT4";
//const char *timerA = "timerA";
//const char *timerB = "timerB";
//const char *timerC = "timerC";
//const char *timerD = "timerD";
//const char *timerE = "timerE";
//const char *timerF = "timerF";
//const char *timerG = "timerG";
//const char *timerH = "timerH";
//const char *timerI = "timerI";
//const char *timerJ = "timerJ";
//const char *timerK = "timerK";
//const char *timerL = "timerL";
//const char *timerM = "timerM";


static uint32_t T1 = TIMER_T1;
static uint32_t T2 = 4000;
static uint32_t T4 = TIMER_T4;
static uint32_t A = TIMER_T1;
static uint32_t B = 64*TIMER_T1;
static uint32_t C = 5*60000; /* >3min */
static uint32_t D = 50000; /*> 32s*/
static uint32_t E = TIMER_T1;
static uint32_t F = 64*TIMER_T1;
static uint32_t G = TIMER_T1;
static uint32_t H = 64*TIMER_T1;
static uint32_t I = TIMER_T4;
static uint32_t J = 64*TIMER_T1;
static uint32_t K = TIMER_T4;
static uint32_t L = 64*TIMER_T1; // draft-sparks-sip-invfix
static uint32_t M = 64*TIMER_T1; // draft-sparks-sip-invfix




void tsip_timers_setT1(uint32_t t1){
	T1 = t1;
	A = E = G = T1;
	B = F = H = J = (T1*64);
}

void tsip_timers_setT2(uint32_t t2){
	T2 = t2;
}

void tsip_timers_setT4(uint32_t t4){
	T4 = t4;
	I = K = T4;
}

void tsip_timers_setA(uint32_t a){
	A = a;
}

void tsip_timers_setB(uint32_t b){
	B = b;
}

void tsip_timers_setC(uint32_t c){
	C = c;
}

void tsip_timers_setD(uint32_t d){
	D = d;
}

void tsip_timers_setE(uint32_t e){
	E = e;
}

void tsip_timers_setF(uint32_t f){
	F = f;
}

void tsip_timers_setG(uint32_t g){
	G = g;
}

void tsip_timers_setH(uint32_t h){
	H = h;
}

void tsip_timers_setI(uint32_t i){
	I = i;
}

void tsip_timers_setJ(uint32_t j){
	J = j;
}

void tsip_timers_setK(uint32_t k){
	K = k;
}

void tsip_timers_setL(uint32_t l){
	L = l;
}

void tsip_timers_setM(uint32_t m){
	M = m;
}

uint32_t tsip_timers_getT1(){
	return T1;
}

uint32_t tsip_timers_getT2(){
	return T2;
}

uint32_t tsip_timers_getT4(){
	return T4;
}

uint32_t tsip_timers_getA(){
	return A;
}

uint32_t tsip_timers_getB(){
	return B;
}

uint32_t tsip_timers_getC(){
	return C;
}

uint32_t tsip_timers_getD(){
	return D;
}

uint32_t tsip_timers_getE(){
	return E;
}

uint32_t tsip_timers_getF(){
	return F;
}

uint32_t tsip_timers_getG(){
	return G;
}

uint32_t tsip_timers_getH(){
	return H;
}

uint32_t tsip_timers_getI(){
	return I;
}

uint32_t tsip_timers_getJ(){
	return J;
}

uint32_t tsip_timers_getK(){
	return K;
}

uint32_t tsip_timers_getL(){
	return L;
}

uint32_t tsip_timers_getM(){
	return M;
}
