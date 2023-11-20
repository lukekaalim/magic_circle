#include <jni.h>
#include "quickjs.h"

jobject createJSValueObject(JNIEnv *env, JSValue value, JSContext *context);

JSValue createJSValue(JNIEnv *env, jobject jsValueInstance);
