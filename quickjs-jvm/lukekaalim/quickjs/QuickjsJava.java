package lukekaalim.quickjs;

public class QuickjsJava {
  private static QuickjsJava lib = null;
  protected long pointer = 0;

  private static native long LoadNative();

  public static QuickjsJava Lib() {
    return Lib("jni_shared");
  }
  public static QuickjsJava Lib(String libname) {
    if (lib == null) {
      System.loadLibrary(libname);
      lib = new QuickjsJava();
      lib.pointer = LoadNative();
    }
    return lib;
  }

  private native JSRuntime NativeNewRuntime(long pointer);

  public JSRuntime NewRuntime() {
    return NativeNewRuntime(pointer);
  }
}
