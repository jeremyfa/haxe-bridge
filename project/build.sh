#!/bin/bash
#fail on errors
set -e

# Build iOS
haxelib run hxcpp Build.xml -Diphoneos -DHXCPP_ARMV7
haxelib run hxcpp Build.xml -Diphoneos -DHXCPP_ARM64
haxelib run hxcpp Build.xml -Diphonesim
haxelib run hxcpp Build.xml -Diphonesim -DHXCPP_M64

# Build Android
haxelib run hxcpp Build.xml -Dandroid
haxelib run hxcpp Build.xml -Dandroid -DHXCPP_ARMV7
haxelib run hxcpp Build.xml -Dandroid -DHXCPP_X86
