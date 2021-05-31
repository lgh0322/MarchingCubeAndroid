#include <jni.h>
#include <string>
#include "table.h"

#include <android/log.h>
#include "input_parse.h"


extern "C" JNIEXPORT jstring JNICALL
Java_com_vaca_myapplication_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";

    infix2Rpn("  x  + y * z");
    double x=equationFunction(3,4,5);
    hello+=to_string(x);


    return env->NewStringUTF(hello.c_str());
}