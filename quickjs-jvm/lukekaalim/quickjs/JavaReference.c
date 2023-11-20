#include <jni.h>
#include <stdlib.h>
#include "JavaReference.h";

jclass getClassRef(JNIEnv *env, const char* name) {
  return (*env)->NewGlobalRef(env, (*env)->FindClass(
    env, name
  ));
}
jfieldID getFieldRef(JNIEnv *env, jclass javaClass, const char* name, const char* typeSigniture) {
  // Do i need to make global references for field ids?
  // I feel like they are just numbers, not objects...
  // NewGlobalRef crashes if I feed it a getFieldId result tho.
  return (*env)->GetFieldID(
    env, javaClass, name, typeSigniture
  );
}

/**
 * Grab some java objects we might need.
*/
struct QuickjsJavaReferenceStruct* createReferenceStruct(JNIEnv *env) {
  long size = sizeof(struct QuickjsJavaReferenceStruct);
  struct QuickjsJavaReferenceStruct *p = (
    (struct QuickjsJavaReferenceStruct *)malloc(size)
  );

  p->runtimeClass = getClassRef(env, "lukekaalim/quickjs/JSRuntime");
  p->runtimePointerField = getFieldRef(env, p->runtimeClass, "pointer", "J");
  p->runtimeQuickjsPointer = getFieldRef(env, p->runtimeClass, "quickjs", "Llukekaalim/quickjs/QuickjsJava;");

  p->valueClass = getClassRef(env, "lukekaalim/quickjs/JSValue");
  p->valuePointerField = getFieldRef(env, p->valueClass, "pointer", "J");
  p->valueContextPointerField = getFieldRef(env, p->valueClass, "contextPointer", "J");
  p->valueTagField = getFieldRef(env, p->valueClass, "tag", "I");

  p->contextClass = getClassRef(env, "lukekaalim/quickjs/JSContext");
  p->contextPointerField = getFieldRef(env, p->contextClass, "pointer", "J");
  p->contextRuntimeField = getFieldRef(env, p->contextClass, "runtime", "Llukekaalim/quickjs/JSRuntime;");

  return p;
};