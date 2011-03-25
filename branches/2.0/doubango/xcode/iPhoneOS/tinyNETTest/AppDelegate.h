#import <UIKit/UIKit.h>

@interface AppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;

- (IBAction)start:(id)sender;

@end
