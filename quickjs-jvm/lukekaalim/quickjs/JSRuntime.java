package lukekaalim.quickjs;

public class JSRuntime {
  private long pointer;
  private QuickjsJava quickjs;
  
  native JSContext NativeCreateContext(
    long referencePointer, long runtimePointer
  );
  
  public JSContext CreateContext() {
    JSContext context = NativeCreateContext(quickjs.pointer, pointer);
    return context;
  }

  private native void NativeSetModuleProvider(
    long referencePointer,
    long runtimePointer,
    IModuleProvider provider
  );
  public void SetModuleProvider(IModuleProvider provider) {
    NativeSetModuleProvider(quickjs.pointer, pointer, provider);
  }
}
