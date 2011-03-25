#import "AppDelegate.h"
#include "tinydav.h"

#include "test_sessions.h"

#define LOOP						0

#define RUN_TEST_ALL				0
#define RUN_TEST_SESSIONS			1

@implementation AppDelegate

@synthesize window;

// Querying the current route
-(void)logCurrentRoute {
    CFStringRef route;
    UInt32 size = sizeof(route);
    
    // Get the name of the current route. 
    AudioSessionGetProperty(kAudioSessionProperty_AudioRoute,
                            &size, &route);
    
    NSLog(@"Route is %@", route);
}

void MyPropListener (void* clientData,
                     AudioSessionPropertyID inID, 
                     UInt32 dataSize, 
                     const void* inData) {
    // The inData argument is a CFDictionary containing the reason for the route change and the name of the old route
    CFDictionaryRef dict = (CFDictionaryRef)inData;
    
    // Get the reason for the route change
    CFNumberRef reason = CFDictionaryGetValue(dict, 
                                              CFSTR(kAudioSession_AudioRouteChangeKey_Reason));
    
    // Get the name of the old route
    CFStringRef oldRoute = CFDictionaryGetValue(dict, 
                                                CFSTR(kAudioSession_AudioRouteChangeKey_OldRoute)); 
    
    NSLog(@"Reason was %d", [reason intValue]);
    NSLog(@"Old route was %@", oldRoute);
}

- (void)beginInterruption {
    NSLog(@"beginInterruption");
}

- (void)endInterruption {
    NSLog(@"endInterruption");
    
    NSError *error = nil;
    AVAudioSession *session = [AVAudioSession sharedInstance];
    [session setActive:YES error:&error];
    [session setCategory:AVAudioSessionCategoryPlayAndRecord error:&error];
}

- (void)endInterruptionWithFlags:(NSUInteger)flags {
    NSLog(@"endInterruptionWithFlags");
    
    NSError *error = nil;
    AVAudioSession *session = [AVAudioSession sharedInstance];
    [session setActive:YES error:&error];
    [session setCategory:AVAudioSessionCategoryPlayAndRecord error:&error];
}

- (void)applicationDidFinishLaunching:(UIApplication *)application {    
	tnet_startup();
	tdav_init();
    
    NSError *error = nil;
    AVAudioSession *session = [AVAudioSession sharedInstance];
    [session setDelegate:self];
    [session setActive:YES error:&error];
    [session setCategory:AVAudioSessionCategoryPlayAndRecord error:&error];
    
    AudioSessionAddPropertyListener(kAudioSessionProperty_AudioRouteChange, MyPropListener, self );
    
    // Override point for customization after application launch
    [window makeKeyAndVisible];
}

- (void)applicationWillTerminate:(UIApplication *)application {
	tdav_deinit();
	tnet_cleanup();
}

- (void)dealloc {
    [window release];
    [super dealloc];
}

- (IBAction)start:(id)sender {
    
	do{
		/* Print copyright information */
		printf("Doubango Project\nCopyright (C) 2009-2010 Mamadou Diop \n\n");
        
#if RUN_TEST_SESSIONS || RUN_TEST_ALL
		test_sessions();
#endif
        
	}
	while(LOOP);
    
}

- (IBAction)change:(id)sender {
    UISwitch *switchButton = sender;
    if ([switchButton isOn]) {
        UInt32 override = kAudioSessionOverrideAudioRoute_Speaker;
        AudioSessionSetProperty (kAudioSessionProperty_OverrideAudioRoute, sizeof(override), &override );
    } else {
        // TODO
    }
}

@end
