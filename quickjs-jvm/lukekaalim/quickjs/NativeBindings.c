/* edit this file - its made by a human ;) */
#include <jni.h>
#include "quickjs.h"
#include "JSValue.h"
#include <stdlib.h>
#include <string.h>
/* Header for class NativeBindings */

#ifndef _Included_NativeBindings
#define _Included_NativeBindings
#ifdef __cplusplus
extern "C"
{
#endif

  inline jlong addr_to_java(void *p)
  {
    return (uintptr_t)p;
  }
  inline void *java_to_addr(jlong p)
  {
    return (void *)p;
  }

  /*
   * Class:     NativeBindings
   * Method:    JS_NewRuntime
   * Signature: ()J
   */
  JNIEXPORT jlong JNICALL Java_lukekaalim_quickjs_NativeBindings_JS_1NewRuntime(JNIEnv *, jclass)
  {
    return JS_NewRuntime();
  }

  /*
   * Class:     NativeBindings
   * Method:    JS_FreeRuntime
   * Signature: (J)V
   */
  JNIEXPORT void JNICALL Java_lukekaalim_quickjs_NativeBindings_JS_1FreeRuntime(JNIEnv *, jclass, jlong);

  /*
   * Class:     NativeBindings
   * Method:    JS_SetRuntimeInfo
   * Signature: (JLjava/lang/String;)V
   */
  JNIEXPORT void JNICALL Java_lukekaalim_quickjs_NativeBindings_JS_1SetRuntimeInfo(JNIEnv *, jclass, jlong, jstring);

  /*
   * Class:     NativeBindings
   * Method:    JS_SetMemoryLimit
   * Signature: (JJ)V
   */
  JNIEXPORT void JNICALL Java_lukekaalim_quickjs_NativeBindings_JS_1SetMemoryLimit(JNIEnv *, jclass, jlong, jlong);

  /*
   * Class:     NativeBindings
   * Method:    JS_SetGCThreshold
   * Signature: (JJ)V
   */
  JNIEXPORT void JNICALL Java_lukekaalim_quickjs_NativeBindings_JS_1SetGCThreshold(JNIEnv *, jclass, jlong, jlong);

  /*
   * Class:     NativeBindings
   * Method:    JS_SetMaxStackSize
   * Signature: (JJ)V
   */
  JNIEXPORT void JNICALL Java_lukekaalim_quickjs_NativeBindings_JS_1SetMaxStackSize(JNIEnv *, jclass, jlong, jlong);

  /*
   * Class:     NativeBindings
   * Method:    JS_NewContext
   * Signature: (J)J
   */
  JNIEXPORT jlong JNICALL Java_lukekaalim_quickjs_NativeBindings_JS_1NewContext(JNIEnv *, jclass, jlong runtime)
  {
    return (jlong)JS_NewContext(runtime);
  }

  /*
   * Class:     NativeBindings
   * Method:    JS_FreeContext
   * Signature: (J)V
   */
  JNIEXPORT void JNICALL Java_lukekaalim_quickjs_NativeBindings_JS_1FreeContext(JNIEnv *, jclass, jlong);


  void throwJavaException(
    JNIEnv *env,
    char *exceptionClass,
    char *message
  ) {
      jclass e = (*env)->FindClass(env, exceptionClass);
      (*env)->ThrowNew(env, e, message);
      (*env)->DeleteLocalRef(env, e);
  };
  void checkException(
    JNIEnv *env,
    jlong context,
    JSValue value
  ) {
    if (JS_IsException(value)) {
      JSValue error = JS_GetException(context);
      JSValue message = JS_GetPropertyStr(context, error, "message");
      const char *msg_str = JS_ToCString(context, message);
      throwJavaException(env, "java/lang/IllegalArgumentException", msg_str);
    }
  }

  /*
   * Class:     NativeBindings
   * Method:    JS_Eval
   * Signature: (JLjava/lang/String;Ljava/lang/String;I)J
   */
  JNIEXPORT jobject JNICALL Java_lukekaalim_quickjs_NativeBindings_JS_1Eval
    (JNIEnv *env, jclass, jlong context, jstring input)
  {
    JSValue value = JS_Eval(
        context,
        (*env)->GetStringUTFChars(env, input, JNI_FALSE),
        (*env)->GetStringUTFLength(env, input), 
        "<eval>",
        JS_EVAL_FLAG_STRICT
    );
    checkException(env, context, value);
    return createJSValueObject(env, value, context);
  }
  JNIEXPORT void JNICALL Java_lukekaalim_quickjs_NativeBindings_JS_1LoadModule
    (JNIEnv *env, jclass, jlong context, jstring input, jstring filename) {
      JSValue value = JS_Eval(
          context,
          (*env)->GetStringUTFChars(env, input, JNI_FALSE),
          (*env)->GetStringUTFLength(env, input), 
          filename,
          JS_EVAL_FLAG_STRICT | JS_EVAL_TYPE_MODULE
      );
      checkException(env, context, value);
    }

  /*
  * Class:     NativeBindings
  * Method:    JS_GetPropertyStr
  * Signature: (JJLjava/lang/String;)LJSValue;
  */
  JNIEXPORT jobject JNICALL Java_lukekaalim_quickjs_NativeBindings_JS_1GetPropertyStr
    (JNIEnv *env, jclass, jlong context, jlong objectPointer, jstring string) {
      JSValue jsObject = JS_MKPTR(-1, objectPointer);
      char* prop = (*env)->GetStringUTFChars(env, string, JNI_FALSE);
      JSValue value = JS_GetPropertyStr(context, jsObject, prop);
      return createJSValueObject(env, value, context);
    }

  JNIEXPORT void JNICALL Java_lukekaalim_quickjs_NativeBindings_JS_1SetPropertyStr
    (JNIEnv *env, jclass, jlong context, jobject object, jstring string, jobject value) {
      JSValue jsObject = createJSValue(env, object);
      JSValue jsValue = createJSValue(env, value);
      char* prop = (*env)->GetStringUTFChars(env, string, JNI_FALSE);
      JS_SetPropertyStr(context, jsObject, prop, jsValue);
    }

  /*
  * Class:     NativeBindings
  * Method:    JS_ToFloat64
  * Signature: (J)D
  */
  JNIEXPORT jdouble JNICALL Java_lukekaalim_quickjs_NativeBindings_JS_1ToFloat64
    (JNIEnv *env, jclass, jlong context, jobject jsValue) {
      JSValue value = createJSValue(env, jsValue);
      jdouble e = -1;
      int suc = JS_ToFloat64(context, &e, value);
      return e;
    }
  JNIEXPORT jlong JNICALL Java_lukekaalim_quickjs_NativeBindings_JS_1ToInt64
    (JNIEnv *env, jclass, jlong context, jobject jsValue) {
      JSValue value = createJSValue(env, jsValue);
      jlong e = -1;
      int suc = JS_ToInt64(context, &e, value);
      return e;
    }
  JNIEXPORT jint JNICALL Java_lukekaalim_quickjs_NativeBindings_JS_1ToInt32
    (JNIEnv *env, jclass, jlong context, jobject jsValue) {
      JSValue value = createJSValue(env, jsValue);
      jint e = -1;
      int suc = JS_ToInt32(context, &e, value);
      return e;
    }

  JNIEXPORT jstring JNICALL Java_lukekaalim_quickjs_NativeBindings_JS_1ToString
    (JNIEnv *env, jclass, jlong context, jobject jsValue) {
      JSValue jsString = createJSValue(env, jsValue);

      return (*env)->NewStringUTF(env, JS_ToCString(context, jsString));
    }

#ifdef __cplusplus
}
#endif
#endif
