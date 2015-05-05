//
//  haxebridge.h
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

#ifndef _HAXEBRIDGE_H_
#define _HAXEBRIDGE_H_

#include <hx/CFFI.h>

namespace haxebridge {

        // Native listener type
    typedef value (*native_listener)(const char* channel, const char *message);

        // Send message from haxe to native
    value haxe_send(value channel, value message);

        // Listen to messages sent from native
    void haxe_listen(value listener);

        // Send message from native to haxe
    const char* native_send(const char* channel, const char* message);

        // Listen to messages sent from haxe
    void native_listen(native_listener listener);

}

#endif //_HAXEBRIDGE_H_
