#import "ApplicationDelegate.h"
#import "Server.h"
#import "PadConfigController.h"

@implementation ApplicationDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
}

- (void)applicationWillTerminate:(NSNotification *)aNotification
{
	delete padConfigController.server; // important clean up
	padConfigController.server = 0;
}

@end
