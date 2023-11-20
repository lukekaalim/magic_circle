#include <jni.h>
#include <stdlib.h>
#include "JavaReference.h";

#include "quickjs.h"

JNIEXPORT jlong JNICALL Java_lukekaalim_quickjs_QuickjsJava_LoadNative
  (JNIEnv *env, jclass) {
    return createReferenceStruct(env);
  }

JNIEXPORT jobject JNICALL Java_lukekaalim_quickjs_QuickjsJava_NativeNewRuntime
  (JNIEnv *env, jobject quickjs, struct QuickjsJavaReferenceStruct *p) {
    JSRuntime *runtimePointer = JS_NewRuntime();
    jobject runtimeObject = (*env)->AllocObject(env, p->runtimeClass);
    (*env)->SetLongField(env, runtimeObject, p->runtimePointerField, runtimePointer);

    (*env)->SetObjectField(env, runtimeObject, p->runtimeQuickjsPointer, quickjs);
    return runtimeObject;
  }