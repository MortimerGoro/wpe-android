#include <jni.h>

#include "logging.h"
#include "glue.h"
#include <dlfcn.h>
#include <stdlib.h>

extern "C" {
    JNIEXPORT void JNICALL Java_com_wpe_wpe_session_SessionGlue_init(JNIEnv*, jobject, jobject, jint, jint);
    JNIEXPORT void JNICALL Java_com_wpe_wpe_session_SessionGlue_deinit(JNIEnv*, jobject);

    JNIEXPORT void JNICALL Java_com_wpe_wpe_session_SessionGlue_setPageURL(JNIEnv*, jobject, jstring);

    JNIEXPORT void JNICALL Java_com_wpe_wpe_session_SessionGlue_frameComplete(JNIEnv*, jobject);

    JNIEXPORT void JNICALL Java_com_wpe_wpe_session_SessionGlue_touchEvent(JNIEnv*, jobject, jlong, jint, jfloat, jfloat);
}

// These are used by WebKit to call into the Java layer.
JNIEXPORT JNIEnv* s_WPEUIProcessGlue_env = 0;
JNIEXPORT jobject s_WPEUIProcessGlue_object = 0;

JNIEXPORT void JNICALL
Java_com_wpe_wpe_session_SessionGlue_init(JNIEnv* env, jobject obj, jobject glueObj,
                                          jint width, jint height)
{
    ALOGV("SessionGlue.init()");
    s_WPEUIProcessGlue_env = env;
    s_WPEUIProcessGlue_object = glueObj;
    wpe_session_glue_init(width, height);
}

JNIEXPORT void JNICALL
Java_com_wpe_wpe_session_SessionGlue_deinit(JNIEnv*, jobject)
{
    ALOGV("SessionGlue.deinit()");
    wpe_session_glue_deinit();
}

JNIEXPORT void JNICALL
Java_com_wpe_wpe_session_SessionGlue_setPageURL(JNIEnv* env, jobject, jstring url)
{
    const char* urlChars = env->GetStringUTFChars(url, 0);
    jsize urlLength = env->GetStringUTFLength(url);
    wpe_session_glue_set_page_url(urlChars, urlLength);
}

JNIEXPORT void JNICALL
Java_com_wpe_wpe_session_SessionGlue_frameComplete(JNIEnv*, jobject)
{
    wpe_session_glue_frame_complete();
}

JNIEXPORT void JNICALL
Java_com_wpe_wpe_session_SessionGlue_touchEvent(JNIEnv*, jobject, jlong time, jint type, jfloat x, jfloat y)
{
    wpe_session_glue_touch_event(time, type, x, y);
}
