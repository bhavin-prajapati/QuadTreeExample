#import <Cocoa/Cocoa.h>
#import "QuadTreeRenderer.h"

@interface QuadTreeAppDelegate : NSObject <NSApplicationDelegate>
@property (nonatomic, strong) NSWindow *window;
@property (nonatomic, strong) QuadTreeView *quadTreeView;
@end

@implementation QuadTreeAppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)notification {
    // Create the main window
    NSRect windowRect = NSMakeRect(100, 100, 800, 600);
    _window = [[NSWindow alloc] initWithContentRect:windowRect
                                          styleMask:(NSWindowStyleMaskTitled | 
                                                   NSWindowStyleMaskClosable | 
                                                   NSWindowStyleMaskMiniaturizable |
                                                   NSWindowStyleMaskResizable)
                                            backing:NSBackingStoreBuffered
                                              defer:NO];
    
    [_window setTitle:@"QuadTree Visualization"];
    [_window center];
    
    // Create the quad tree view
    _quadTreeView = [[QuadTreeView alloc] initWithFrame:[[_window contentView] bounds]];
    [_quadTreeView setAutoresizingMask:(NSViewWidthSizable | NSViewHeightSizable)];
    
    // Set up the content view
    [_window setContentView:_quadTreeView];
    
    // Create menu bar
    [self setupMenuBar];
    
    // Add some initial random points
    [_quadTreeView addRandomPoints:50];
    
    // Show the window
    [_window makeKeyAndOrderFront:nil];
    
    NSLog(@"QuadTree Application Started!");
    NSLog(@"Instructions:");
    NSLog(@"- Left click: Add a point");
    NSLog(@"- Left drag: Create query rectangle");
    NSLog(@"- Right click: Clear all points");
    NSLog(@"- Menu: Add random points or clear");
}

- (void)setupMenuBar {
    NSMenu *mainMenu = [[NSMenu alloc] init];
    
    // Application menu
    NSMenuItem *appMenuItem = [[NSMenuItem alloc] init];
    NSMenu *appMenu = [[NSMenu alloc] init];
    [appMenu addItemWithTitle:@"Quit QuadTree" action:@selector(terminate:) keyEquivalent:@"q"];
    [appMenuItem setSubmenu:appMenu];
    [mainMenu addItem:appMenuItem];
    
    // QuadTree menu
    NSMenuItem *quadTreeMenuItem = [[NSMenuItem alloc] initWithTitle:@"QuadTree" action:nil keyEquivalent:@""];
    NSMenu *quadTreeMenu = [[NSMenu alloc] initWithTitle:@"QuadTree"];
    
    [quadTreeMenu addItemWithTitle:@"Add 10 Random Points" 
                            action:@selector(addSmallRandomPoints:) 
                     keyEquivalent:@"1"];
    [quadTreeMenu addItemWithTitle:@"Add 50 Random Points" 
                            action:@selector(addMediumRandomPoints:) 
                     keyEquivalent:@"2"];
    [quadTreeMenu addItemWithTitle:@"Add 200 Random Points" 
                            action:@selector(addLargeRandomPoints:) 
                     keyEquivalent:@"3"];
    [quadTreeMenu addItem:[NSMenuItem separatorItem]];
    [quadTreeMenu addItemWithTitle:@"Clear All Points" 
                            action:@selector(clearPoints:) 
                     keyEquivalent:@"c"];
    
    [quadTreeMenuItem setSubmenu:quadTreeMenu];
    [mainMenu addItem:quadTreeMenuItem];
    
    // Help menu
    NSMenuItem *helpMenuItem = [[NSMenuItem alloc] initWithTitle:@"Help" action:nil keyEquivalent:@""];
    NSMenu *helpMenu = [[NSMenu alloc] initWithTitle:@"Help"];
    [helpMenu addItemWithTitle:@"Show Instructions" action:@selector(showInstructions:) keyEquivalent:@"h"];
    [helpMenuItem setSubmenu:helpMenu];
    [mainMenu addItem:helpMenuItem];
    
    [NSApp setMainMenu:mainMenu];
}

// Menu actions
- (void)addSmallRandomPoints:(id)sender {
    [_quadTreeView addRandomPoints:10];
}

- (void)addMediumRandomPoints:(id)sender {
    [_quadTreeView addRandomPoints:50];
}

- (void)addLargeRandomPoints:(id)sender {
    [_quadTreeView addRandomPoints:200];
}

- (void)clearPoints:(id)sender {
    [_quadTreeView clearPoints];
}

- (void)showInstructions:(id)sender {
    NSAlert *alert = [[NSAlert alloc] init];
    [alert setMessageText:@"QuadTree Visualization Instructions"];
    [alert setInformativeText:@"• Left click anywhere to add a point\n"
                              @"• Left click and drag to create a query rectangle (red)\n"
                              @"• Right click to clear all points\n"
                              @"• Use the QuadTree menu to add random points\n"
                              @"• Watch how the tree subdivides as you add points!\n\n"
                              @"Green dots = points\n"
                              @"White lines = quad tree boundaries\n"
                              @"Red rectangle = query area\n"
                              @"Large red dots = points found in query"];
    [alert addButtonWithTitle:@"OK"];
    [alert runModal];
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender {
    return YES;
}

@end

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        NSApplication *app = [NSApplication sharedApplication];
        QuadTreeAppDelegate *delegate = [[QuadTreeAppDelegate alloc] init];
        [app setDelegate:delegate];
        [app run];
    }
    return 0;
}
