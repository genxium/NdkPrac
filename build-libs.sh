# ndk r10d is used
# reference $NDK_ROOT/build/core/build-local.mk
JNI_ROOT=./jni
ndk-build NDK_DEBUG=1 NDK_PROJECT_PATH=. NDK_APPLICATION_MK=$JNI_ROOT/Application.mk 
