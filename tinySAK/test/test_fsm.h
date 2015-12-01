/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango.org>
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
#ifndef _TEST_FSM_H_
#define _TEST_FSM_H_

typedef struct test_fsm_ctx_s
{
	unsigned unsubscribing:1;
}
test_fsm_ctx_t;

/* ======================== actions ======================== */
typedef enum test_fsm_action_e
{
	test_fsm_action_send,
	test_fsm_action_1xx,
	test_fsm_action_2xx,
	test_fsm_action_401_407_421_494,
	test_fsm_action_423,
	test_fsm_action_300_to_699,
	test_fsm_action_cancel,
	test_fsm_action_notify,
	test_fsm_action_unsubscribe,
	test_fsm_action_refresh,
	test_fsm_action_transporterror,
	test_fsm_action_error,
}
test_fsm_action_t;

/* ======================== execs ======================== */
int test_fsm_exec_Started_2_Trying_X_send(va_list *app)
{
	const test_fsm_ctx_t* ctx = va_arg(*app, const  test_fsm_ctx_t *);
	const void* message = va_arg(*app, const void *);

	return 0;
}

int test_fsm_exec_Trying_2_Trying_X_1xx(va_list *app)
{
	const test_fsm_ctx_t* ctx = va_arg(*app, const  test_fsm_ctx_t *);
	const void* message = va_arg(*app, const void *);

	return 0;
}

int test_fsm_exec_Trying_2_Terminated_X_2xx(va_list *app)
{
	const test_fsm_ctx_t* ctx = va_arg(*app, const  test_fsm_ctx_t *);
	const void* message = va_arg(*app, const void *);

	return 0;
}

int test_fsm_exec_Trying_2_Connected_X_2xx(va_list *app)
{
	const test_fsm_ctx_t* ctx = va_arg(*app, const  test_fsm_ctx_t *);
	const void* message = va_arg(*app, const void *);

	return 0;
}

int test_fsm_exec_Trying_2_Trying_X_401_407_421_494(va_list *app)
{
	const test_fsm_ctx_t* ctx = va_arg(*app, const  test_fsm_ctx_t *);
	const void* message = va_arg(*app, const void *);

	return 0;
}

int test_fsm_exec_Trying_2_Trying_X_423(va_list *app)
{
	const test_fsm_ctx_t* ctx = va_arg(*app, const  test_fsm_ctx_t *);
	const void* message = va_arg(*app, const void *);

	return 0;
}

int test_fsm_exec_Trying_2_Terminated_X_300_to_699(va_list *app)
{
	const test_fsm_ctx_t* ctx = va_arg(*app, const  test_fsm_ctx_t *);
	const void* message = va_arg(*app, const void *);

	return 0;
}

int test_fsm_exec_Trying_2_Terminated_X_cancel(va_list *app)
{
	const test_fsm_ctx_t* ctx = va_arg(*app, const  test_fsm_ctx_t *);
	const void* message = va_arg(*app, const void *);

	return 0;
}

int test_fsm_exec_Trying_2_Trying_X_NOTIFY(va_list *app)
{
	const test_fsm_ctx_t* ctx = va_arg(*app, const  test_fsm_ctx_t *);
	const void* message = va_arg(*app, const void *);

	return 0;
}

int test_fsm_exec_Connected_2_Trying_X_unsubscribe(va_list *app)
{
	const test_fsm_ctx_t* ctx = va_arg(*app, const  test_fsm_ctx_t *);
	const void* message = va_arg(*app, const void *);

	return 0;
}

int test_fsm_exec_Connected_2_Trying_X_refresh(va_list *app)
{
	const test_fsm_ctx_t* ctx = va_arg(*app, const  test_fsm_ctx_t *);
	const void* message = va_arg(*app, const void *);

	return 0;
}

int test_fsm_exec_Connected_2_Connected_X_NOTIFY(va_list *app)
{
	const test_fsm_ctx_t* ctx = va_arg(*app, const  test_fsm_ctx_t *);
	const void* message = va_arg(*app, const void *);

	return 0;
}

int test_fsm_exec_Connected_2_Terminated_X_NOTIFY(va_list *app)
{
	const test_fsm_ctx_t* ctx = va_arg(*app, const  test_fsm_ctx_t *);
	const void* message = va_arg(*app, const void *);

	return 0;
}

int test_fsm_exec_Any_2_Trying_X_hangup(va_list *app)
{
	const test_fsm_ctx_t* ctx = va_arg(*app, const  test_fsm_ctx_t *);
	const void* message = va_arg(*app, const void *);

	return 0;
}

int test_fsm_exec_Any_2_Terminated_X_transportError(va_list *app)
{
	const test_fsm_ctx_t* ctx = va_arg(*app, const  test_fsm_ctx_t *);
	const void* message = va_arg(*app, const void *);

	return 0;
}

int test_fsm_exec_Any_2_Terminated_X_Error(va_list *app)
{
	const test_fsm_ctx_t* ctx = va_arg(*app, const  test_fsm_ctx_t *);
	const void* message = va_arg(*app, const void *);

	return 0;
}

/* ======================== conds ======================== */
int test_fsm_cond_unsubscribing(test_fsm_ctx_t* ctx, void* data)
{
	return ctx->unsubscribing ? 1 : 0;
}
int test_fsm_cond_subscribing(test_fsm_ctx_t* ctx, void* data)
{
	return !test_fsm_cond_unsubscribing(ctx, data);
}

int test_fsm_cond_notify_terminated(test_fsm_ctx_t* ctx, void* sipmessage)
{
	return 0;
}

int test_fsm_cond_notify_not_terminated(test_fsm_ctx_t* ctx, void* sipmessage)
{
	return !test_fsm_cond_notify_terminated(ctx, sipmessage);
}

typedef enum test_fsm_state_e
{
	__nil = 0,
	Started,
	Trying,
	Connected,
	Terminated
}
test_fsm_state_t;


int test_fsm_onterminated(const test_fsm_ctx_t* ctx)
{
	TSK_DEBUG_INFO("FSM in terminal state.");
	return 0;
}

#define TEST_FSM_ACTIONS_COUNT 5
test_fsm_action_t test_fsm_tests[TEST_FSM_ACTIONS_COUNT][TEST_FSM_ACTIONS_COUNT] =
{
	{ test_fsm_action_send, test_fsm_action_300_to_699 },
	{ test_fsm_action_send, test_fsm_action_401_407_421_494, test_fsm_action_2xx, test_fsm_action_refresh, test_fsm_action_300_to_699},
	{ test_fsm_action_send, test_fsm_action_401_407_421_494, test_fsm_action_2xx, test_fsm_action_refresh,  test_fsm_action_2xx},
	{ test_fsm_action_send, test_fsm_action_2xx, test_fsm_action_2xx, test_fsm_action_refresh, test_fsm_action_2xx},
	{ test_fsm_action_send, test_fsm_action_423, test_fsm_action_2xx, test_fsm_action_refresh, test_fsm_action_401_407_421_494},
};


void test_fsm()
{
	size_t i;

	for(i=0; i<TEST_FSM_ACTIONS_COUNT; i++)
	{
		size_t j;
		tsk_fsm_t* fsm = tsk_fsm_create(Started, Terminated);
		test_fsm_ctx_t ctx;
		ctx.unsubscribing = 0;
		
		tsk_fsm_set_callback_terminated(fsm, test_fsm_onterminated, &ctx);

		tsk_fsm_set(fsm,

			/*=======================
			* === Any === 
			*/
			// Any -> (transport error) -> Terminated
			TSK_FSM_ADD_ALWAYS(tsk_fsm_state_any, test_fsm_action_transporterror, Terminated, test_fsm_exec_Any_2_Terminated_X_transportError, "test_fsm_exec_Any_2_Terminated_X_transportError"),
			// Any -> (transport error) -> Terminated
			TSK_FSM_ADD_ALWAYS(tsk_fsm_state_any, test_fsm_action_error, Terminated, test_fsm_exec_Any_2_Terminated_X_Error, "test_fsm_exec_Any_2_Terminated_X_Error"),
			// Any -> (hangup) -> Terminated
			// Any -> (hangup) -> Trying
			
			/*=======================
			* === Started === 
			*/
			// Started -> (Send) -> Trying
			TSK_FSM_ADD_ALWAYS(Started, test_fsm_action_send, Trying, test_fsm_exec_Started_2_Trying_X_send, "test_fsm_exec_Started_2_Trying_X_send"),
			// Started -> (Any) -> Started
			TSK_FSM_ADD_ALWAYS_NOTHING(Started, "test_fsm_exec_Started_2_Started_X_any"),
			

			/*=======================
			* === Trying === 
			*/
			// Trying -> (1xx) -> Trying
			TSK_FSM_ADD_ALWAYS(Trying, test_fsm_action_1xx, Trying, test_fsm_exec_Trying_2_Trying_X_1xx, "test_fsm_exec_Trying_2_Trying_X_1xx"),
			// Trying -> (2xx) -> Terminated
			TSK_FSM_ADD(Trying, test_fsm_action_2xx, test_fsm_cond_unsubscribing, Terminated, test_fsm_exec_Trying_2_Terminated_X_2xx, "test_fsm_exec_Trying_2_Terminated_X_2xx"),
			// Trying -> (2xx) -> Connected
			TSK_FSM_ADD(Trying, test_fsm_action_2xx, test_fsm_cond_subscribing, Connected, test_fsm_exec_Trying_2_Connected_X_2xx, "test_fsm_exec_Trying_2_Connected_X_2xx"),
			// Trying -> (401/407/421/494) -> Trying
			TSK_FSM_ADD_ALWAYS(Trying, test_fsm_action_401_407_421_494, Trying, test_fsm_exec_Trying_2_Trying_X_401_407_421_494, "test_fsm_exec_Trying_2_Trying_X_401_407_421_494"),
			// Trying -> (423) -> Trying
			TSK_FSM_ADD_ALWAYS(Trying, test_fsm_action_423, Trying, test_fsm_exec_Trying_2_Trying_X_423, "test_fsm_exec_Trying_2_Trying_X_423"),
			// Trying -> (300_to_699) -> Terminated
			TSK_FSM_ADD_ALWAYS(Trying, test_fsm_action_300_to_699, Terminated, test_fsm_exec_Trying_2_Terminated_X_300_to_699, "test_fsm_exec_Trying_2_Terminated_X_300_to_699"),
			// Trying -> (cancel) -> Terminated
			TSK_FSM_ADD_ALWAYS(Trying, test_fsm_action_cancel, Terminated, test_fsm_exec_Trying_2_Terminated_X_cancel, "test_fsm_exec_Trying_2_Terminated_X_cancel"),
			// Trying -> (Notify) -> Trying
			TSK_FSM_ADD_ALWAYS(Trying, test_fsm_action_notify, Trying, test_fsm_exec_Trying_2_Trying_X_NOTIFY, "test_fsm_exec_Trying_2_Trying_X_NOTIFY"),
			// Trying -> (Any) -> Trying
			TSK_FSM_ADD_ALWAYS_NOTHING(Trying, "test_fsm_exec_Trying_2_Trying_X_any"),


			/*=======================
			* === Connected === 
			*/
			// Connected -> (unsubscribe) -> Trying
			TSK_FSM_ADD_ALWAYS(Connected, test_fsm_action_unsubscribe, Trying, test_fsm_exec_Connected_2_Trying_X_unsubscribe, "test_fsm_exec_Connected_2_Trying_X_unsubscribe"),
			// Connected -> (refresh) -> Trying
			TSK_FSM_ADD_ALWAYS(Connected, test_fsm_action_refresh, Trying, test_fsm_exec_Connected_2_Trying_X_refresh, "test_fsm_exec_Connected_2_Trying_X_refresh"),
			// Connected -> (NOTIFY) -> Connected
			TSK_FSM_ADD(Connected, test_fsm_action_notify, test_fsm_cond_notify_not_terminated, Connected, test_fsm_exec_Connected_2_Connected_X_NOTIFY, "test_fsm_exec_Connected_2_Connected_X_NOTIFY"),
			// Connected -> (NOTIFY) -> Terminated
			TSK_FSM_ADD(Connected, test_fsm_action_notify, test_fsm_cond_notify_terminated, Terminated, test_fsm_exec_Connected_2_Terminated_X_NOTIFY, "test_fsm_exec_Connected_2_Terminated_X_NOTIFY"),
			// Connected -> (Any) -> Connected
			TSK_FSM_ADD_ALWAYS_NOTHING(Connected, "test_fsm_exec_Connected_2_Connected_X_any"),

			TSK_FSM_ADD_NULL());


		for(j=0; j<TEST_FSM_ACTIONS_COUNT; j++){
			tsk_fsm_act(fsm, test_fsm_tests[i][j], &ctx, tsk_null, &ctx, tsk_null /*message*/);
		}
		
		TSK_OBJECT_SAFE_FREE(fsm);

		printf("\n\n");
	}
}

#endif /* _TEST_FSM_H_ */
