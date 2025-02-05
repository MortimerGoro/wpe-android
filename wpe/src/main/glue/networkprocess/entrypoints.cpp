#include <jni.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

#include "logging.h"

extern "C" {
    JNIEXPORT void JNICALL Java_com_wpe_wpe_services_NetworkProcessGlue_initializeXdg(JNIEnv*, jobject, jstring);
    JNIEXPORT void JNICALL Java_com_wpe_wpe_services_NetworkProcessGlue_initializeGioExtraModulesPath(JNIEnv*, jobject, jstring);
    JNIEXPORT void JNICALL Java_com_wpe_wpe_services_NetworkProcessGlue_initializeMain(JNIEnv*, jobject, jint);
}

JNIEXPORT void JNICALL
Java_com_wpe_wpe_services_NetworkProcessGlue_initializeXdg(JNIEnv* env, jobject, jstring xdgRuntimePath)
{
    ALOGV("Glue::initializeXdg()");

    const char* cachePath = env->GetStringUTFChars(xdgRuntimePath, 0);
    ALOGV("  runtimePath %s", cachePath);

    setenv("XDG_RUNTIME_DIR", cachePath, 1);
}

JNIEXPORT void JNICALL
Java_com_wpe_wpe_services_NetworkProcessGlue_initializeGioExtraModulesPath(JNIEnv* env, jobject, jstring extraModulesPath)
{
    ALOGV("Glue::initializeGIOExtraModulesPath(), path %p", extraModulesPath);
    jsize pathLength = env->GetStringUTFLength(extraModulesPath);
    const char* pathChars = env->GetStringUTFChars(extraModulesPath, 0);
    ALOGV("  pathLength %d, pathChars %s", pathLength, pathChars);

    setenv("GIO_EXTRA_MODULES", pathChars, 1);
}

JNIEXPORT void JNICALL
Java_com_wpe_wpe_services_NetworkProcessGlue_initializeMain(JNIEnv*, jobject, jint fd)
{
    pipe_stdout_to_logcat();

    using NetworkProcessEntryPoint = int(int, char**);
    auto* entrypoint = reinterpret_cast<NetworkProcessEntryPoint*>(dlsym(RTLD_DEFAULT, "android_NetworkProcess_main"));

    ALOGV("Glue::initializeMain(), fd %d, entrypoint %p", fd, entrypoint);

    char pidString[32];
    snprintf(pidString, sizeof(pidString), "%d", getpid());
    char fdString[32];
    snprintf(fdString, sizeof(fdString), "%d", fd);

    char* argv[3];
    argv[0] = (char*)"WPENetworkProcess";
    argv[1] = pidString;
    argv[2] = fdString;
    (*entrypoint)(3, argv);
}
