//
//  haxebridge.cpp
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

#ifndef STATIC_LINK
#define IMPLEMENT_API
#endif

#if defined(HX_WINDOWS) || defined(HX_MACOS) || defined(HX_LINUX)
#define NEKO_COMPATIBLE
#endif

#include <hx/CFFI.h>
#include <string>
#include <map>

#include "haxebridge.h"

namespace haxebridge {

        // Haxe channel listeners mapping
    std::map<std::string, AutoGCRoot*> haxe_channel_listeners;

        // Native channel listeners mapping
    std::map<std::string, native_listener> native_channel_listeners;

        // Send message from native to haxe
    const char* native_send(const char* channel, const char* message) {
            // Check if there is a registered haxe listener for this channel
        std::string key = channel;
        auto search = haxe_channel_listeners.find(key);
        if (search != haxe_channel_listeners.end()) {
                // Yes, then call it
            value result = val_call1(search->second->get(), (message == NULL ? alloc_null() : alloc_string(message)) );
            if (val_is_string(result)) {
                return val_string(result);
            }
            return NULL;
        }
            // No, then return null
        return NULL;
    }

        // Listen to messages sent from haxe
    void native_listen(const char* channel, native_listener listener) {
            // Assign listener to the given channel in mapping
        std::string key = channel;
        native_channel_listeners[key] = listener;
    }

        // Send message from haxe to native
    value haxe_send(value channel, value message) {
            // Check if there is a registered native listener for this channel
        const char *channel_str = val_string(channel);
        std::string key(channel_str);
        auto search = native_channel_listeners.find(key);
        if (search != native_channel_listeners.end()) {
                // Yes, then call it
            const char *result = ((native_listener)(*(search->second)))(channel_str, val_string(message));
            if (result == NULL) {
                return alloc_null();
            }
            return alloc_string(result);
        }
            // No, then return null
        return alloc_null();
    }
    DEFINE_PRIM(haxe_send, 2);

        // Listen to messages sent from native
    void haxe_listen(value channel, value listener) {
            // Check if there is a registered haxe listener for this channel
        std::string key = val_string(channel);
        auto search = haxe_channel_listeners.find(key);
        if (search != haxe_channel_listeners.end()) {
                // Yes, then remove it
            haxe_channel_listeners.erase(search);
            delete search->second;
        }

            // Assign new listener
        haxe_channel_listeners[key] = new AutoGCRoot(listener);
    }
    DEFINE_PRIM(haxe_listen, 2);

        // Entry point
    extern "C" void haxebridge_entry_point() {
        val_int(0);
    }
    DEFINE_ENTRY_POINT(haxebridge_entry_point);

    extern "C" int haxebridge_register_prims() {

        haxebridge_entry_point();

        return 0;

    }

}
