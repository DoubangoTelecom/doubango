#ifndef _H_STATEMAP
#define _H_STATEMAP

/*
 * The contents of this file are subject to the Mozilla Public
 * License Version 1.1 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of
 * the License at http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS
 * IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Original Code is State Machine Compiler (SMC).
 *
 * The Initial Developer of the Original Code is Charles W. Rapp.
 *
 * Port to C by Francois Perrad, francois.perrad@gadz.org
 * Copyright 2004, Francois Perrad.
 * All Rights Reserved.
 *
 * Contributor(s):
 *
 * Description
 *
 * RCS ID
 * $Id: statemap.h,v 1.4 2009/03/01 18:20:40 cwrapp Exp $
 *
 * Change Log
 * $Log: statemap.h,v $
 * Revision 1.4  2009/03/01 18:20:40  cwrapp
 * Preliminary v. 6.0.0 commit.
 *
 * Revision 1.3  2008/05/30 21:14:48  fperrad
 * - define TRACE only when undefined
 *
 * Revision 1.2  2007/08/05 12:58:11  cwrapp
 * Version 5.0.1 check-in. See net/sf/smc/CODE_README.txt for more information.
 *
 * Revision 1.1  2005/06/16 18:08:17  fperrad
 * Added C, Perl & Ruby.
 *
 */

#include <stdio.h>
#include <string.h>

#ifndef TRACE
#define TRACE printf
#endif

#define STATE_MEMBERS \
    const char *_name; \
    int _id;

struct State {
    STATE_MEMBERS
};

#define getName(state) \
    (state)->_name
#define getId(state) \
    (state)->_id

#define State_Default(fsm) \
    assert(0)

#define FSM_MEMBERS(app) \
    const struct app##State * _state; \
    const struct app##State * _previous_state; \
    const struct app##State ** _stack_start; \
    const struct app##State ** _stack_curr; \
    const struct app##State ** _stack_max; \
    const char * _transition; \
    int _debug_flag;

struct FSMContext {
    FSM_MEMBERS(_)
};

#define FSM_INIT(fsm, state)    \
    (fsm)->_state = (state); \
    (fsm)->_previous_state = NULL; \
    (fsm)->_stack_start = NULL; \
    (fsm)->_stack_curr = NULL; \
    (fsm)->_stack_max = NULL; \
    (fsm)->_transition = NULL; \
    (fsm)->_debug_flag = 0

#define FSM_STACK(fsm, stack) \
    (fsm)->_stack_start = &(stack)[0]; \
    (fsm)->_stack_curr = &(stack)[0]; \
    (fsm)->_stack_max = &(stack)[0] + (sizeof(stack) / sizeof(void*))


#define getState(fsm) \
    (fsm)->_state
#define clearState(fsm) \
    (fsm)->_previous_state = (fsm)->_state; \
    (fsm)->_state = NULL
#define setState(fsm, state) \
    (fsm)->_state = (state); \
    if ((fsm)->_debug_flag != 0) { \
        TRACE("NEW STATE    : %s\n", getName(state)); \
    }
#define pushState(fsm, state) \
    if ((fsm)->_stack_curr >= (fsm)->_stack_max) { \
        assert(0 == "STACK OVERFLOW"); \
    } \
    *((fsm)->_stack_curr) = (fsm)->_state; \
    (fsm)->_stack_curr ++; \
    (fsm)->_state = state; \
    if ((fsm)->_debug_flag != 0) { \
        TRACE("PUSH TO STATE: %s\n", getName(state)); \
    }
#define popState(fsm) \
    (fsm)->_stack_curr --; \
    (fsm)->_state = *((fsm)->_stack_curr); \
    if ((fsm)->_debug_flag != 0) { \
        TRACE("POP TO STATE : %s\n", getName((fsm)->_state)); \
    }
#define emptyStateStack(fsm) \
    (fsm)->_stack_curr = (fsm)->_stack_start
#define setTransition(fsm, transition) \
    (fsm)->_transition = (transition)
#define getTransition(fsm) \
    (fsm)->_transition
#define getDebugFlag(fsm) \
    (fsm)->_debug_flag
#define setDebugFlag(fsm, flag) \
    (fsm)->_debug_flag = (flag)

#endif
