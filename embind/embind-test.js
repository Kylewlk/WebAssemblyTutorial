
console.log("Module.lerp(1, 2, 0.5) = " + Module.lerp(1, 2, 0.5)); // 1.5

console.log(Module.MyClass.getClassString());
var instance = new Module.MyClass(10, "hello");
instance.incrementX();
console.log(instance.x); 
instance.x = 20; 
console.log(instance.x);
instance.setString("world");
console.log(instance.getString()); 
instance.delete();