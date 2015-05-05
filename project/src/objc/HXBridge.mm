
//
//  HXBridge.mm
//
//  @author Jeremy Faivre <contact@jeremyfa.com>, http://jeremyfa.com
//

// Copyright (c) 2015 Jeremy Faivre.
// This code is distributed under the terms and conditions of the MIT license.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#import "HXBridge.h"
#include "haxebridge.h"

    // Obj-C listener instances by channel
static NSMutableDictionary *sHXBridge_objc_listeners = nil;

    // Native listener function that links haxe extension's C++ code with Obj-C logic
value sHXBridge_native_listener(const char* channel, const char *message) {
        // Find listener from channel
    NSString *channel_nsstring = [[NSString alloc] initWithUTF8String:channel];
    id<HXBridgeChannelListener> listener = sHXBridge_objc_listeners[channel_nsstring];
        // If there is no listener, return null
    if (!listener) return NULL;
        // Call listener with message
    NSString *message_nsstring = message == NULL ? nil : [[NSString alloc] initWithUTF8String:message];
    NSString *result = [listener hxBridgeDidReceiveMessage:message_nsstring fromChannel:channel_nsstring];
        // Return result
    if (!result) {
        return alloc_null();
    }
    return alloc_string([result cStringUsingEncoding:NSUTF8StringEncoding]);
}

@implementation HXBridge

    // Send message from objective-c to haxe
+ (NSString *)sendMessage:(NSString *)message inChannel:(NSString *)channel
{
        // Call C++ send function
    const char *result = haxebridge::native_send([channel cStringUsingEncoding:NSUTF8StringEncoding], [message cStringUsingEncoding:NSUTF8StringEncoding]);
        // Return result
    if (result == NULL) return nil;
    return [[NSString alloc] initWithUTF8String:result];
}

    // Listen to messages sent from haxe
+ (void)listenChannel:(NSString *)channel withListener:(id<HXBridgeChannelListener>)listener
{
        // Initialize listeners mapping if needed
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sHXBridge_objc_listeners = [NSMutableDictionary dictionary];
            // Map the local listener on C++ side
        haxebridge::native_listen(&sHXBridge_native_listener);
    });

        // Keep track of listener on Obj-C side
    sHXBridge_objc_listeners[channel] = listener;
}

@end
