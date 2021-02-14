#include <jni.h>

#include "logging.h"
#include "wpeuiprocessglue.h"
#include <dlfcn.h>
#include <stdlib.h>

extern "C" {
    JNIEXPORT void JNICALL Java_com_wpe_wpe_UIProcess_Glue_init(JNIEnv*, jobject, jobject, jint, jint, jstring, jstring);
    JNIEXPORT void JNICALL Java_com_wpe_wpe_UIProcess_Glue_deinit(JNIEnv*, jobject);

    JNIEXPORT void JNICALL Java_com_wpe_wpe_UIProcess_Glue_setPageURL(JNIEnv*, jobject, jstring);

    JNIEXPORT void JNICALL Java_com_wpe_wpe_UIProcess_Glue_frameComplete(JNIEnv*, jobject);

    JNIEXPORT void JNICALL Java_com_wpe_wpe_UIProcess_Glue_touchEvent(JNIEnv*, jobject, jlong, jint, jfloat, jfloat);

    JNIEXPORT JNIEnv* s_WPEUIProcessGlue_env = 0;
    JNIEXPORT jobject s_WPEUIProcessGlue_object = 0;
}

JNIEXPORT void JNICALL
Java_com_wpe_wpe_UIProcess_Glue_init(JNIEnv* env, jobject obj, jobject glueObj,
                                     jint width, jint height, jstring webkitExecPath,
                                     jstring ldLibraryPath)
{
    ALOGV("Glue.init() webkitExecPath %p", webkitExecPath);

    s_WPEUIProcessGlue_env = env;
    s_WPEUIProcessGlue_object = glueObj;
    ALOGV("Glue initialized VM to %p, its address %p", env, &s_WPEUIProcessGlue_env);
    ALOGV("Glue initialized object to %p, its address %p", obj, &s_WPEUIProcessGlue_object);

    const char* webkitExecPathChars = env->GetStringUTFChars(webkitExecPath, 0);
    ALOGV("WEBKIT_EXEC_PATH %s", webkitExecPathChars);
    setenv("WEBKIT_EXEC_PATH", webkitExecPathChars, 1);

    const char* ldLibraryPathChars = env->GetStringUTFChars(ldLibraryPath, 0);
    ALOGV("LD_LIBRARY_PATH %s", ldLibraryPathChars);
    setenv("LD_LIBRARY_PATH", ldLibraryPathChars, 1);

    wpe_uiprocess_glue_init(env, glueObj, width, height);
}

JNIEXPORT void JNICALL
Java_com_wpe_wpe_UIProcess_Glue_deinit(JNIEnv*, jobject)
{
    ALOGV("Glue.deinit()");
    s_WPEUIProcessGlue_env = 0;
    s_WPEUIProcessGlue_object = 0;
    wpe_uiprocess_glue_deinit();
}

JNIEXPORT void JNICALL
Java_com_wpe_wpe_UIProcess_Glue_setPageURL(JNIEnv* env, jobject, jstring url)
{
    const char* urlChars = env->GetStringUTFChars(url, 0);
    jsize urlLength = env->GetStringUTFLength(url);

    wpe_uiprocess_glue_set_page_url(urlChars, urlLength);
}

JNIEXPORT void JNICALL
Java_com_wpe_wpe_UIProcess_Glue_frameComplete(JNIEnv*, jobject)
{
    wpe_uiprocess_glue_frame_complete();
}

JNIEXPORT void JNICALL
Java_com_wpe_wpe_UIProcess_Glue_touchEvent(JNIEnv*, jobject, jlong time, jint type, jfloat x, jfloat y)
{
    wpe_uiprocess_glue_touch_event(time, type, x, y);
}
