package lukekaalim.quickjs;

public class JSValue {
  private long contextPointer = 0;

  public enum TagType {
    INT,
    BOOL,
    NULL,
    UNDEFINED,
    UNINITIALIZED,
    CATCH_OFFSET,
    EXCEPTION,
    FLOAT64,

    BIG_DECIMAL,
    BIG_INT,
    BIG_FLOAT,
    SYMBOL,
    STRING,
    MODULE,
    FUNCTION_BYTECODE,
    OBJECT,
  }
  
  public String String() {
    return NativeBindings.JS_ToString(contextPointer, this);
  }
  public Double Double() {
    return NativeBindings.JS_ToFloat64(contextPointer, this);
  }

  public JSValue GetProperty(String name) {
    return NativeBindings.JS_GetPropertyStr(contextPointer, pointer, name);
  }
  public JSValue SetProperty(String name, JSValue value) {
    NativeBindings.JS_SetPropertyStr(contextPointer, this, name, value);
    return this;
  }

  public native JSValue Call(JSValue ...arguments);

  public TagType GetTag() throws Exception {
    switch (tag) {
      case -8:
        return TagType.SYMBOL;
      case -7:
        return TagType.STRING;
      case -3:
        return TagType.MODULE;
      case -1:
        return TagType.OBJECT;
      case 0:
        return TagType.INT;
      case 1:
        return TagType.BOOL;
      case 2:
        return TagType.NULL;
      case 3:
        return TagType.UNDEFINED;
      case 4:
        return TagType.UNINITIALIZED;
      case 5:
        return TagType.CATCH_OFFSET;
      case 6:
        return TagType.EXCEPTION;
      case 7:
        return TagType.FLOAT64;
      default:
        throw new Exception("Unknown Tag Type");
    }
  }

  public int tag;
  private long pointer = 0;
}
