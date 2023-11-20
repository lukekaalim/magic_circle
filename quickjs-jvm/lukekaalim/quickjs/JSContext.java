package lukekaalim.quickjs;

/**
 * QuickJS Context gains unique globals and
 * can share objects with other contexts
 * within the same runtime.
 */
public class JSContext {
  public long pointer;
  public JSRuntime runtime;

  public native JSValue Function(IExternalJSFunction func);
  public native JSValue String(String value);
  public native JSValue Object();

  public native JSValue GetGlobal();

  public native JSValue Eval(String program);
  public native JSValue LoadModule(String module, String filename);
  public native JSValue RunModule(JSValue module);
}