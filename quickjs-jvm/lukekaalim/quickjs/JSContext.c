#include <jni.h>
#include <stdlib.h>

#include "quickjs.h"
#include "JSValue.h"

void throwJavaIfException(
  JNIEnv *env,
  jlong context,
  JSValue value
) {
  if (JS_IsException(value)) {
    JSValue error = JS_GetException(context);
    JSValue message = JS_GetPropertyStr(context, error, "message");
    const char *msg_str = JS_ToCString(context, message);
    jclass e = (*env)->FindClass(env, "java/lang/IllegalArgumentException");
    (*env)->ThrowNew(env, e, msg_str);
    (*env)->DeleteLocalRef(env, e);
  }
}

JSValue Invoke(
  JSContext *ctx,
  JSValueConst this_val,
  int argc,
  JSValueConst *argv,
  int magic,
  JSValue *func_data
) {
  JNIEnv *env = 0;
  jobject object = 0;
  JS_ToInt64(ctx, &env, func_data[0]);
  JS_ToInt64(ctx, &object, func_data[1]);

  jclass cls = (*env)->FindClass(env, "lukekaalim/quickjs/IExternalJSFunction");
  jclass valueCls = (*env)->FindClass(env, "lukekaalim/quickjs/JSValue");

  jmethodID fld = (*env)->GetMethodID(env, cls, "Call", "([Llukekaalim/quickjs/JSValue;)Llukekaalim/quickjs/JSValue;");

  jobjectRefType refType = (*env)->GetObjectRefType(env, object);

  jobjectArray argumentArray = (*env)->NewObjectArray(env, argc, valueCls, 0);
  for (int i = 0; i < argc; i++) {
    (*env)->SetObjectArrayElement(env, argumentArray, i, createJSValueObject(env, argv[i], ctx));
  }
  (*env)->CallObjectMethod(env, object, fld, argumentArray);
  return JS_NULL;
}

JSContext* getContext(JNIEnv *env, jobject contextObject) {
    jclass cls = (*env)->FindClass(env, "lukekaalim/quickjs/JSContext");
    jfieldID fld = (*env)->GetFieldID(env, cls, "pointer", "J");
    JSContext* context = (*env)->GetLongField(env, contextObject, fld);
    return context;
}

/*
 * Class:     lukekaalim_quickjs_JSContext
 * Method:    Function
 * Signature: (Llukekaalim/quickjs/IExternalJSFunction;)Llukekaalim/quickjs/JSValue;
 */
JNIEXPORT jobject JNICALL Java_lukekaalim_quickjs_JSContext_Function
  (JNIEnv *env, jobject contextObject, jobject functionObject) {
    JSContext* context = getContext(env, contextObject);

    // TODO memory leak here lol
    jobject ref = (*env)->NewGlobalRef(env, functionObject);
    jclass cls = (*env)->FindClass(env, "lukekaalim/quickjs/IExternalJSFunction");
    
    JSValue data[] = { JS_NewInt64(context, env), JS_NewInt64(context, ref) };

    JSValue value = JS_NewCFunctionData(context, &Invoke, 2, 0, 2, data);
    return createJSValueObject(env, value, context);
  }


JNIEXPORT jobject JNICALL Java_lukekaalim_quickjs_JSContext_String
  (JNIEnv *env, jobject contextObject, jstring string) {
    JSContext* context = getContext(env, contextObject);

    char* prop = (*env)->GetStringUTFChars(env, string, JNI_FALSE);
    JSValue value = JS_NewString(context, prop);
    return createJSValueObject(env, value, context);
  }

JNIEXPORT jobject JNICALL Java_lukekaalim_quickjs_JSContext_Object
  (JNIEnv *env, jobject contextObject) {
    JSContext* context = getContext(env, contextObject);

    JSValue value = JS_NewObject(context);
    return createJSValueObject(env, value, context);
  }

JNIEXPORT jobject JNICALL Java_lukekaalim_quickjs_JSContext_GetGlobal
  (JNIEnv *env, jobject contextObject) {
    JSContext* context = getContext(env, contextObject);
    JSValue global = JS_GetGlobalObject(context);
    jobject assignment = createJSValueObject(env, global, context);
    return assignment;
  };

JNIEXPORT jobject JNICALL Java_lukekaalim_quickjs_JSContext_Eval
  (JNIEnv *env, jobject contextObject, jstring evalString) {
    JSContext* context = getContext(env, contextObject);
    const char *evalCString = (*env)->GetStringUTFChars(env, evalString, JNI_FALSE);
    int evalStringLength = (*env)->GetStringUTFLength(env, evalString);

    JSValue result = JS_Eval(context, evalCString, evalStringLength, "<Eval>", JS_EVAL_FLAG_STRICT);
    throwJavaIfException(env, context, result);
    return createJSValueObject(env, result, context);
  };

JNIEXPORT jobject JNICALL Java_lukekaalim_quickjs_JSContext_LoadModule
  (JNIEnv *env, jobject contextObject, jstring moduleContents, jstring filename) {
    JSContext* context = getContext(env, contextObject);
    const char *moduleString = (*env)->GetStringUTFChars(env, moduleContents, JNI_FALSE);
    const char *filenameString = (*env)->GetStringUTFChars(env, filename, JNI_FALSE);
    int moduleLength = (*env)->GetStringUTFLength(env, moduleContents);
    int flags = JS_EVAL_FLAG_STRICT | JS_EVAL_TYPE_MODULE | JS_EVAL_FLAG_COMPILE_ONLY;
    JSValue result = JS_Eval(context, moduleString, moduleLength, filenameString, flags);

    throwJavaIfException(env, context, result);
    return createJSValueObject(env, result, contextObject);
  };
JNIEXPORT jobject JNICALL Java_lukekaalim_quickjs_JSContext_RunModule
  (JNIEnv *env, jobject contextObject, jobject moduleValue) {
    JSContext* context = getContext(env, contextObject);
    JSValue module = createJSValue(env, moduleValue);
    JSValue result = JS_EvalFunction(context, module);
    throwJavaIfException(env, context, result);
    return createJSValueObject(env, result, context);
  };