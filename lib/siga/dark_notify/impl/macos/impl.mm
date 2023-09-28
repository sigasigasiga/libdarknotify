#import <siga/dark_notify/impl/macos/impl.hpp>

#import <Cocoa/Cocoa.h>

namespace siga::dark_notify::impl::macos {

appearance_t query() {
  NSOperatingSystemVersion ver =
      [NSProcessInfo processInfo].operatingSystemVersion;
  if (ver.majorVersion < 10 ||
      (ver.majorVersion == 10 && ver.minorVersion < 14)) {
    return appearance_t::unknown;
  }

  NSString *appearance = [[NSUserDefaults standardUserDefaults]
      stringForKey:@"AppleInterfaceStyle"];

  if ([appearance isEqualToString:@"Dark"]) {
    return appearance_t::dark;
  } else {
    return appearance_t::light;
  }
}

void register_callback(callback_t callback) {
  [[NSDistributedNotificationCenter defaultCenter]
      addObserverForName:@"AppleInterfaceThemeChangedNotification"
                  object:nil
                   queue:nil
              usingBlock:^(NSNotification *) {
                callback(query());
              }];
}

void tick() {
  NSApplication *sharedApp = [NSApplication sharedApplication];
  NSEvent *event = [sharedApp nextEventMatchingMask:NSEventMaskSystemDefined
                                          untilDate:nil
                                             inMode:NSDefaultRunLoopMode
                                            dequeue:YES];

  if (event) {
    [sharedApp sendEvent:event];
  }
}

void run() {
  while (true) {
    tick();
  }
}

} // namespace siga::dark_notify::impl::macos
