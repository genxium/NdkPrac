#include "include/com_genxium_ndkprac_NativeWrapper.h"
#include <pthread.h> 
#include <android/log.h>
#include <stdlib.h>     /* srand, rand are only available since android-21*/
#include <time.h>       /* time */
#include <unistd.h>		/* usleep */
#include <assert.h>
#include <signal.h>

#define  LOG_TAG    "NATIVEWRAPPER"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#define ALARM_FREE 0
#define ALARM_SCHEDULED 1
#define ALARM_SET 2

#define ABORT_FREE 0
#define ABORT_SCHEDULED 1

struct sigaction sa_new, sa_old;
volatile int global_signum = 0;
// `abort_set` not used yet
int alarm_set = ALARM_FREE, abort_set = ABORT_FREE;
int count = 0;
void sig_handler(int sig) {
	// do not call any non-reentrant functions within signal handler
	if (sig == SIGALRM) __sync_val_compare_and_swap(&alarm_set, ALARM_SCHEDULED, ALARM_SET);
	if (sig == SIGABRT) {
		if (__sync_val_compare_and_swap(&abort_set, ABORT_SCHEDULED, ABORT_FREE) == ABORT_SCHEDULED) {
			__android_log_write(ANDROID_LOG_ERROR, LOG_TAG, "SIGABRT consumed");
		}
	}
}

void register_handler() {
	// only available since android-21
	/*
	// The signals SIGKILL and SIGSTOP cannot be caught or ignored.
	(void) signal(SIGALRM, sig_handler);
	(void) signal(SIGABRT, sig_handler);
	LOGE("Signal handlers are all registered");
	*/
}

void* detach_routine(void* params) {
	// only available since android-21
	/*
	while (true) {
		srand(time(NULL));
		int n = rand() % 42;
		if (n % 3 == 0)	{
			if (__sync_val_compare_and_swap(&alarm_set, ALARM_FREE, ALARM_SCHEDULED) == ALARM_FREE) {
				alarm(1);
				LOGE("scheduled SIGALRM");
			}
		}
		usleep(500000); // 500ms
	}
	*/
	pthread_exit(NULL);
}

void start_magic() {
    pthread_t to_detach;
    int th = pthread_create(&to_detach, NULL, detach_routine, NULL);
    pthread_detach(to_detach);
}

JNIEXPORT jint JNICALL Java_com_genxium_ndkprac_NativeWrapper_add
  (JNIEnv * env, jobject obj, jint a, jint b) {
    return (a + b);
}

JNIEXPORT void JNICALL Java_com_genxium_ndkprac_NativeWrapper_start
  (JNIEnv *, jobject) {
	start_magic();
	while (true) {
		if (__sync_val_compare_and_swap(&alarm_set, ALARM_SET, ALARM_FREE) == ALARM_SET) {
			// upon alarm set
			++count;
			LOGE("alarm_set consumed, count = %d", count);
		}

		if (count >= 3 && __sync_val_compare_and_swap(&abort_set, ABORT_FREE, ABORT_SCHEDULED) == ABORT_FREE) abort();
		else pause();
	}
}

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    JNIEnv* env = NULL;
    jint result = -1;

    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) return -1;
    register_handler();
    return JNI_VERSION_1_4;
}
