package lukekaalim.quickjs;

// TODO Remote these legacy methods
public class NativeBindings {
  // RUNTIME
  public static native long JS_NewRuntime();
  public static native void JS_FreeRuntime(long runtime);

  public static native void JS_SetRuntimeInfo(long runtime, String info);
  public static native void JS_SetMemoryLimit(long runtime, long limit);
  public static native void JS_SetGCThreshold(long runtime, long gc_threshold);
  public static native void JS_SetMaxStackSize(long runtime, long stack_size);
  // CONTEXT
  public static native long JS_NewContext(long runtime);
  public static native void JS_FreeContext(long context);

  // EVAL
  public static native JSValue JS_Eval(
    long context,
    String input
  );
  public static native void JS_LoadModule(
    long context,
    String input,
    String filename
  );
  public static native JSValue JS_GetPropertyStr(
    long context,
    long object,
    String property
  );
  public static native void JS_SetPropertyStr(
    long context,
    JSValue object,
    String property,
    JSValue value
  );

  // JSVALUE
  public static native double JS_ToFloat64(long context, JSValue jsValue);
  public static native long JS_ToInt64(long context, JSValue jsValue);
  public static native int JS_ToInt32(long context, JSValue jsValue);
  public static native String JS_ToString(long context, JSValue jsValue);
}
