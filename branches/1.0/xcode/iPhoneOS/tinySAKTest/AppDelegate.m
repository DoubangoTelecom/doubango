#import "AppDelegate.h"

#include "tsk.h"

#define RUN_TEST_ALL				1
#define RUN_TEST_LISTS				0
#define RUN_TEST_HEAP				0
#define RUN_TEST_STRINGS			0
#define RUN_TEST_URL				0
#define RUN_TEST_THREADS			0
#define RUN_TEST_MUTEX				0
#define RUN_TEST_CONDWAIT			0
#define RUN_TEST_SEMAPHORE			0 /* FIXME: android */
#define RUN_TEST_SAFEOBJECT			0
#define RUN_TEST_OBJECT				0
#define RUN_TEST_PARAMS				1
#define RUN_TEST_OPTIONS			0
#define RUN_TEST_TIMER				1
#define RUN_TEST_RUNNABLE			0
#define RUN_TEST_BUFFER				0
#define RUN_TEST_MD5				0
#define RUN_TEST_SHA1				0
#define RUN_TEST_BASE64				0
#define RUN_TEST_UUID				0
#define RUN_TEST_FSM				0

#if RUN_TEST_LISTS || RUN_TEST_ALL
#include "test_lists.h"
#endif

#if RUN_TEST_HEAP || RUN_TEST_ALL
#include "test_heap.h"
#endif

#if RUN_TEST_STRINGS || RUN_TEST_ALL
#include "test_strings.h"
#endif

#if RUN_TEST_URL || RUN_TEST_ALL
#include "test_url.h"
#endif

#if RUN_TEST_THREADS || RUN_TEST_ALL
#include "test_threads.h"
#endif

#if RUN_TEST_MUTEX || RUN_TEST_ALL
#include "test_mutex.h"
#endif

#if RUN_TEST_CONDWAIT || RUN_TEST_ALL
#include "test_condwait.h"
#endif

#if RUN_TEST_SEMAPHORE || RUN_TEST_ALL
#include "test_semaphore.h"
#endif

#if RUN_TEST_SAFEOBJECT || RUN_TEST_ALL
//#include "test_safeobject.h"
#endif

#if RUN_TEST_OBJECT || RUN_TEST_ALL
#include "test_object.h"
#endif

#if RUN_TEST_PARAMS || RUN_TEST_ALL
#include "test_params.h"
#endif

#if RUN_TEST_OPTIONS || RUN_TEST_ALL
#include "test_options.h"
#endif

#if RUN_TEST_TIMER || RUN_TEST_ALL
#include "test_timer.h"
#endif

#if RUN_TEST_RUNNABLE || RUN_TEST_ALL
#include "test_runnable.h"
#endif

#if RUN_TEST_BUFFER || RUN_TEST_ALL
#include "test_buffer.h"
#endif

#if RUN_TEST_MD5 || RUN_TEST_ALL
#include "test_md5.h"
#endif

#if RUN_TEST_SHA1 || RUN_TEST_ALL
#include "test_sha1.h"
#endif

#if RUN_TEST_BASE64 || RUN_TEST_ALL
#include "test_base64.h"
#endif

#if RUN_TEST_UUID || RUN_TEST_ALL
#include "test_uuid.h"
#endif

#if RUN_TEST_FSM || RUN_TEST_ALL
#include "test_fsm.h"
#endif

@implementation AppDelegate

@synthesize window;

- (void)applicationDidFinishLaunching:(UIApplication *)application {    
    // Override point for customization after application launch
    [window makeKeyAndVisible];
}

- (void)dealloc {
    [window release];
    [super dealloc];
    
    NSIndexPath *path = [NSIndexPath indexPathWithIndex:1];
}

- (IBAction)start:(id)sender {
#if RUN_TEST_LISTS || RUN_TEST_ALL
    /* linked lists */
    test_basic_list();
    printf("\n\n");
    test_complex_list();
    printf("\n\n");
    test_filtered_list();
    printf("\n\n");
#endif
    
#if RUN_TEST_HEAP || RUN_TEST_ALL
    /* heap */
    test_heap();
    printf("\n\n");
#endif
    
#if RUN_TEST_STRINGS || RUN_TEST_ALL
    /* strings */
    test_strings();
    printf("\n\n");
#endif
    
#if RUN_TEST_URL || RUN_TEST_ALL
    /* url */
    test_url();
    printf("\n\n");
#endif
    
#if RUN_TEST_THREADS || RUN_TEST_ALL
    /* threads */
    test_threads();
    printf("\n\n");
#endif
    
#if RUN_TEST_MUTEX || RUN_TEST_ALL
    /* mutex */
    test_mutex();
    printf("\n\n");
#endif
    
#if RUN_TEST_CONDWAIT || RUN_TEST_ALL
    /* condwait */
    test_condwait();
    printf("\n\n");
#endif
    
#if RUN_TEST_SEMAPHORE || RUN_TEST_ALL
    /* semaphore */
    test_semaphore();
    printf("\n\n");
#endif
    
#if RUN_TEST_SAFEOBJECT || RUN_TEST_ALL
	/* safe object */
    //test_safeobject();
    printf("\n\n");
#endif
    
#if RUN_TEST_OBJECT || RUN_TEST_ALL
	/* object */
    //test_object();
    printf("\n\n");
#endif
    
#if RUN_TEST_PARAMS || RUN_TEST_ALL
    /* parameters */
    test_params();
    printf("\n\n");
#endif
    
#if RUN_TEST_OPTIONS || RUN_TEST_ALL
    /* options */
    test_options();
    printf("\n\n");
#endif
    
#if RUN_TEST_TIMER || RUN_TEST_ALL
    /* timer */
    test_timer();
    printf("\n\n");
#endif
    
#if RUN_TEST_RUNNABLE || RUN_TEST_ALL
    /* test runnable. */
    test_runnable();
    printf("\n\n");
#endif
    
    
#if RUN_TEST_BUFFER || RUN_TEST_ALL
    /* test buffer */
    test_buffer();
#endif
    
#if RUN_TEST_MD5 || RUN_TEST_ALL
    /* test md5 and hmac_md5 */
    test_md5();
    test_hmac_md5();
#endif
    
#if RUN_TEST_SHA1 || RUN_TEST_ALL
    /* test sha1 and hmac_sha-1 */
    test_sha1();
    test_hmac_sha1();
#endif
    
#if RUN_TEST_BASE64 || RUN_TEST_ALL
    /* test base64 encoding/decoding */
    test_base64();
#endif
    
#if RUN_TEST_UUID || RUN_TEST_ALL
    /* test fake UUID (version5) */
    test_uuid();
#endif
    
#if RUN_TEST_FSM || RUN_TEST_ALL
    /* test FSM */
    test_fsm();
#endif
}

@end
