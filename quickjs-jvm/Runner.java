import lukekaalim.quickjs.*;

public class Runner {
  public static void main(String[] args) throws Exception {
    System.out.println("Loading Library");
    QuickjsJava lib = QuickjsJava.Lib();
    System.out.println("Creating Runtime");
    JSRuntime myRuntime = lib.NewRuntime();
    System.out.println("Creating Context");
    JSContext myContext = myRuntime.CreateContext();

    class MyClass implements IExternalJSFunction {
      public JSValue Call(JSValue[] args) {
        System.out.println("Hello from Java!");
        return myContext.String("Whoo hoo!");
      }
    }
    class Logger implements IExternalJSFunction {
      public JSValue Call(JSValue[] args) {
        System.out.println(args[0].String());
        return null;
      }
    }

    System.out.println("Creating Function");
    JSValue myJsFunc = myContext.Function(new MyClass());
    JSValue logger = myContext.Function(new Logger());

    System.out.println("Getting Global");
    JSValue global = myContext.GetGlobal();
    global.SetProperty(
      "Hello",
      myContext.String("World!")
    );
    global.SetProperty(
      "run",
      myJsFunc
    );
    global.SetProperty(
      "console",
      myContext.Object()
        .SetProperty("log", logger)
    );

    String input = (
        "export function myFunc(arg) { console.log(arg) }\n"
    );
    String input2 = (
        "import { myFunc } from 'mymodule1.js'\n" +
        "myFunc('Hello from javascript!')"
    );
    System.out.println("Running Eval!");
    class ModuleProvider implements IModuleProvider {
      public void LoadModule(JSContext context, String filename) {
        throw new UnsupportedOperationException("Unimplemented method 'LoadModule'");
      }
    }
    myRuntime.SetModuleProvider(new ModuleProvider());
    JSValue output = myContext.LoadModule(input, "mymodule1.js");
    JSValue output2 = myContext.LoadModule(input2, "mymodule2.js");
    System.out.println("Tag of Module Load: " + output2.GetTag());
    myContext.RunModule(output2);
    //System.out.println(output.Call().Call(myContext.String("INTO")).String());
  }
}
