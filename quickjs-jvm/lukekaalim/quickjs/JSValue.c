#include <jni.h>
#include <stdlib.h>

#include "quickjs.h"

jobject createJSValueObject(JNIEnv *env, JSValue value, JSContext *context) {
  jclass cls = (*env)->FindClass(env, "lukekaalim/quickjs/JSValue");
  jobject jsValueInstance =   (*env)->AllocObject(env, cls);

  jfieldID tagField =         (*env)->GetFieldID(env, cls, "tag", "I");
  jfieldID pointerField =     (*env)->GetFieldID(env, cls, "pointer", "J");
  jfieldID contextPointerField =     (*env)->GetFieldID(env, cls, "contextPointer", "J");

  (*env)->SetIntField(env, jsValueInstance, tagField, JS_VALUE_GET_TAG(value));
  (*env)->SetLongField(env, jsValueInstance, pointerField, JS_VALUE_GET_PTR(value));
  (*env)->SetLongField(env, jsValueInstance, contextPointerField, context);

  return jsValueInstance;
};

JSValue createJSValue(JNIEnv *env, jobject jsValueInstance) {
  jclass cls = (*env)->FindClass(env, "lukekaalim/quickjs/JSValue");
  jfieldID tagField =         (*env)->GetFieldID(env, cls, "tag", "I");
  jfieldID pointerField =     (*env)->GetFieldID(env, cls, "pointer", "J");

  int tag = (*env)->GetIntField(env, jsValueInstance, tagField);
  long pointer = (*env)->GetLongField(env, jsValueInstance, pointerField);

  return JS_MKPTR(tag, pointer);
};

JNIEXPORT jobject JNICALL Java_lukekaalim_quickjs_JSValue_Call
  (JNIEnv *env, jobject jsFunctionValue, jarray argumentsArray) {
    jsize length = (*env)->GetArrayLength(env, argumentsArray);
    JSValue argumentValues[length];
    for (int i = 0; i < length; i++) {
      argumentValues[i] = createJSValue(env, (*env)->GetObjectArrayElement(env, argumentsArray, i));
    }
  
    jclass cls = (*env)->FindClass(env, "lukekaalim/quickjs/JSValue");
    jfieldID contextField = (*env)->GetFieldID(env, cls, "contextPointer", "J");
    JSContext *context = (*env)->GetLongField(env, jsFunctionValue, contextField);

    JSValue value = createJSValue(env, jsFunctionValue);

    JSValue output = JS_Call(context, value, JS_NULL, length, argumentValues);
    return createJSValueObject(env, output, context);
  }