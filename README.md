# IOHIDFamily-368.21

This is patched version of IOHIDFamily-368.21 to use USB device from [Chrome/Chromium](../../../chromium-legacy) on 10.7.5 (build 11G63).  
Chrome/Chromium needs raw device descriptor (kIOHIDReportDescriptorKey) to recognize USB devices, but IOHIDDevice on 10.7 does not expose raw value.  
Actual diff is [only 1 byte (`#if 0` to `#if 1`)](https://github.com/blueboxd/IOHIDFamily-368.21/blob/818e57180235ad043df340a5ff3f700cb25d1920/IOHIDFamily/IOHIDDevice.cpp#L312) from original.

## build
Some workarounds may need to build, so [pre-built binary](https://github.com/blueboxd/IOHIDFamily-368.21/releases/tag/main) is available.

## usage
Replace existing IOHIDFamily.kext with new one, and rebuild kext cache with `sudo kextcache -system-caches`, then reboot.