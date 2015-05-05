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

#include "haxebridge.h"

namespace haxebridge {

        // Local listener
    native_listener local_listener = NULL;

        // Haxe listener
    AutoGCRoot *haxe_listener = NULL;

        // Send message from native to haxe
    const char* native_send(const char* channel, const char* message) {
            // Check if there is a registered haxe listener
        if (haxe_listener != NULL) {
                // Yes, then call it
            value result = val_call2(haxe_listener->get(), alloc_string(channel), (message == NULL ? alloc_null() : alloc_string(message)) );
            if (val_is_string(result)) {
                return val_string(result);
            }
            return NULL;
        }
            // No, then return null
        return NULL;
    }

        // Listen to messages sent from haxe
    void native_listen(native_listener listener) {
            // Assign listener to the local one
        local_listener = listener;
    }

        // Send message from haxe to native
    value haxe_send(value channel, value message) {
            // Check if there is a registered local listener
        if (local_listener != NULL) {
                // Yes, then call it
            return (*local_listener)(val_string(channel), val_string(message));
        }
            // No, then return null
        return alloc_null();
    }
    DEFINE_PRIM(haxe_send, 2);

        // Listen to messages sent from native
    void haxe_listen(value listener) {
            // Assign listener
        haxe_listener = new AutoGCRoot(listener);
    }
    DEFINE_PRIM(haxe_listen, 1);

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
