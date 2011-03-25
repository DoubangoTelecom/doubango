#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>
#import <AudioToolbox/AudioToolbox.h>

@interface AppDelegate : NSObject <UIApplicationDelegate, AVAudioSessionDelegate> {
    UIWindow *window;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;

- (IBAction)start:(id)sender;

- (IBAction)change:(id)sender;

@end
