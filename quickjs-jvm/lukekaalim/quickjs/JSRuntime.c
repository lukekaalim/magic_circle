#include <jni.h>
#include <stdlib.h>

#include "quickjs.h"
#include "JavaReference.h"

JNIEXPORT jobject JNICALL Java_lukekaalim_quickjs_JSRuntime_NativeCreateContext
  (JNIEnv *env, jobject this, struct QuickjsJavaReferenceStruct *p, JSContext *runtime) {
    JSContext *contextPointer = JS_NewContext(runtime);

    jobject contextObject = (*env)->AllocObject(env, p->contextClass);
    (*env)->SetLongField(env, contextObject, p->contextPointerField, contextPointer);
    (*env)->SetObjectField(env, contextObject, p->contextRuntimeField, this);

    return contextObject;
  }

struct ModuleLoader {
  jobject moduleLoaderRef;
  struct QuickjsJavaReferenceStruct *refs;
  JNIEnv *env;
};

int MakeMyModule(JSContext *ctx, JSModuleDef *m) {
  return 0;
}

JSModuleDef *HandleModuleLoad(JSContext *ctx, const char *module_name, struct ModuleLoader *loader) {
  printf("Tried to load a module\n");
  //printf(module_name);
  //const char *content = "export const nothing = null;";
  //JS_Eval(ctx, content, sizeof(content), module_name, JS_EVAL_TYPE_MODULE | JS_EVAL_FLAG_COMPILE_ONLY);
  //JSModuleDef *myMod = JS_NewCModule(ctx, module_name, &MakeMyModule);
  return 0;
}

JNIEXPORT void JNICALL Java_lukekaalim_quickjs_JSRuntime_NativeSetModuleProvider
  (JNIEnv *env, jobject, struct QuickjsJavaReferenceStruct *p, JSContext *runtime, jobject moduleProvider) {
    //JSContext* context = getContext(env, contextObject);
    struct ModuleLoader *loader = malloc(sizeof(struct ModuleLoader));
    jobject globalModuleRef = (*env)->NewGlobalRef(env, moduleProvider);

    loader->env = env;
    loader->refs = p;
    loader->moduleLoaderRef = globalModuleRef;

    return JS_SetModuleLoaderFunc(runtime, 0, &HandleModuleLoad, &loader);
  };
