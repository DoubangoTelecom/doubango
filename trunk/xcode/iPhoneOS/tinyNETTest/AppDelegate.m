#import "AppDelegate.h"

#include "tsk.h"
#include "tinynet.h"

#include "test_sockets.h"
#include "test_transport.h"
#include "test_auth.h"
#include "test_stun.h"
#include "test_nat.h"
#include "test_ifaces.h"
#include "test_dns.h"
#include "test_dhcp.h"
#include "test_dhcp6.h"
#include "test_tls.h"

#define RUN_TEST_ALL		0
#define RUN_TEST_SOCKETS	0 /* FIXME: Android */
#define RUN_TEST_TRANSPORT	0
#define RUN_TEST_AUTH		0
#define RUN_TEST_STUN		0
#define RUN_TEST_NAT		0
#define RUN_TEST_IFACES		0
#define RUN_TEST_DNS		0
#define RUN_TEST_DHCP		0
#define RUN_TEST_DHCP6		1
#define RUN_TEST_TLS		0

@implementation AppDelegate

@synthesize window;

- (void)applicationDidFinishLaunching:(UIApplication *)application {    
    // Override point for customization after application launch
    [window makeKeyAndVisible];
}

- (void)dealloc {
    [window release];
    [super dealloc];
}

- (IBAction)start:(id)sender {
#if RUN_TEST_ALL  || RUN_TEST_SOCKETS
    test_sockets();
#endif
    
#if RUN_TEST_ALL  || RUN_TEST_TRANSPORT
    test_transport();
#endif
    
#if RUN_TEST_ALL || RUN_TEST_AUTH
    test_auth();
#endif
    
#if RUN_TEST_ALL || RUN_TEST_STUN
    test_stun();
#endif
    
#if RUN_TEST_ALL || RUN_TEST_NAT
    test_nat();
#endif
    
#if RUN_TEST_ALL || RUN_TEST_IFACES
    test_ifaces();
#endif
    
#if RUN_TEST_ALL || RUN_TEST_DNS
    test_dns();
#endif
    
#if RUN_TEST_ALL || RUN_TEST_DHCP
    test_dhcp();
#endif
    
#if RUN_TEST_ALL || RUN_TEST_DHCP6
    test_dhcp6();
#endif
    
#if RUN_TEST_ALL || RUN_TEST_TLS
    test_tls();
#endif
}

@end
