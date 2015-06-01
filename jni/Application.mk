APP_PROJECT_PATH := $(call my-dir)/..
APP_BUILD_SCRIPT := $(APP_PROJECT_PATH)/jni/Android.mk
APP_ABI := armeabi-v7a
APP_STL := gnustl_static

# Meaning of APP_OPTIM can be found in $NDK_ROOT/docs/Programmers_Guide/html/md_3__key__topics__building__chapter_1-section_8_ndk-build.html. Basically "debug" means "GCC -O0" while "release" means "GCC -O2". However to enable debugging with "ndk-gdb" script, "ndk-build NDK_DEBUG=1" is still needed at least for ndk-r10d. 
# Enable ONLY one of the following
# APP_OPTIM := debug
# APP_OPTIM := release
