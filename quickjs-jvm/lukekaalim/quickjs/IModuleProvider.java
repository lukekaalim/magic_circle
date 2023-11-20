package lukekaalim.quickjs;

public interface IModuleProvider {
  void LoadModule(JSContext context, String filename);
}
