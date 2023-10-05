#import <siga/dark_notify/impl/macos/nsapp.h>

#import <Cocoa/Cocoa.h>

namespace siga::dark_notify::impl::macos {

// dark_notify_t
void nsapp_t::register_callback(callback_t callback) {
    observer_ = [[NSDistributedNotificationCenter defaultCenter]
        addObserverForName:@"AppleInterfaceThemeChangedNotification"
                    object:nil
                     queue:nil
                usingBlock:^(NSNotification *) {
                  callback(query());
                }];
}

void nsapp_t::unregister_callback() {
    [[NSDistributedNotificationCenter defaultCenter]
        removeObserver:observer_
                  name:@"AppleInterfaceThemeChangedNotification"
                object:nil];
}

auto nsapp_t::query() -> appearance_t {
    NSOperatingSystemVersion ver =
        [NSProcessInfo processInfo].operatingSystemVersion;
    if(ver.majorVersion < 10 ||
       (ver.majorVersion == 10 && ver.minorVersion < 14)) {
        return appearance_t::unknown;
    }

    NSString *appearance = [[NSUserDefaults standardUserDefaults]
        stringForKey:@"AppleInterfaceStyle"];

    if([appearance isEqualToString:@"Dark"]) {
        return appearance_t::dark;
    } else {
        return appearance_t::light;
    }
}

void nsapp_t::tick() {
    NSApplication *sharedApp = [NSApplication sharedApplication];
    NSEvent *event = [sharedApp nextEventMatchingMask:NSEventMaskSystemDefined
                                            untilDate:nil
                                               inMode:NSDefaultRunLoopMode
                                              dequeue:YES];

    if(event) {
        [sharedApp sendEvent:event];
    }
}

void nsapp_t::run() {
    [[NSApplication sharedApplication] run];
}

void nsapp_t::stop() {
    NSApplication *sharedApp = [NSApplication sharedApplication];
    [sharedApp stop:nil];

    // call a dummy event to wake the event loop up
    NSEvent *dummyEvent =
        [NSEvent otherEventWithType:NSEventTypeApplicationDefined
                           location:NSZeroPoint
                      modifierFlags:0
                          timestamp:0
                       windowNumber:0
                            context:nil
                            subtype:0
                              data1:0
                              data2:0];
    [sharedApp postEvent:dummyEvent atStart:NO];
}

} // namespace siga::dark_notify::impl::macos
