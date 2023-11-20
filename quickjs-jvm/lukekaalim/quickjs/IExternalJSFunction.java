package lukekaalim.quickjs;

public interface IExternalJSFunction {
  JSValue Call(JSValue[] arguments);
}
