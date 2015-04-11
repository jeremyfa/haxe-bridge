#!/bin/bash
#fail on errors
set -e

# Build
haxelib run flow build ios --clean --project haxebridge.flow --archs armv6,armv7,armv7s,arm64,sim,sim64 --d static_link
