// lua func declaration
declare function print(s: string): void;


/**
 * Native (C++) Api
 */
declare class MyNativeBinding {
  public static New(): MyNativeBinding;

  public getKeyboardButtonPressed(key: string): boolean;
  public spawn(x: number, y: number, z: number): void;
}

const binding = MyNativeBinding.New();

function globalFunction(dt: number): String {

  if (binding.getKeyboardButtonPressed("A")) {
    binding.spawn(10, 20, 30);
    print("Key pressed");

  }

  return `no press (${dt})`
}
