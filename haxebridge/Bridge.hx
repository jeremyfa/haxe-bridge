//
//  Bridge.hx
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

package haxebridge;

#if cpp
import cpp.Lib;
#end

class Bridge {

    #if cpp
        // Native C++ bindings
    private static var native_haxe_send = Lib.load("haxebridge", "haxe_send", 2);
    private static var native_haxe_listen = Lib.load("haxebridge", "haxe_listen", 2);
    #end

    public static function send(channel:String, message:String):String {
        #if cpp
            // Forward to C++ implementation
        return native_haxe_send(channel, message);
        #else
        return null;
        #end
    }

    public static function listen(channel:String, listener:String->String):Void {
        #if cpp
            // Forward to C++ implementation
        return native_haxe_listen(channel, listener);
        #else
        return null;
        #end
    }

}
