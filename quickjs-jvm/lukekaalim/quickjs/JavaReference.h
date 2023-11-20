#include <jni.h>

struct QuickjsJavaReferenceStruct
{
  jclass runtimeClass;
  jfieldID runtimePointerField;
  jfieldID runtimeQuickjsPointer;

  jclass valueClass;
  jfieldID valuePointerField;
  jfieldID valueTagField;
  jfieldID valueContextPointerField;

  jclass contextClass;
  jfieldID contextPointerField;
  jfieldID contextRuntimeField;

  jclass iModuleProvider;
};

struct QuickjsJavaReferenceStruct* createReferenceStruct(JNIEnv *env);