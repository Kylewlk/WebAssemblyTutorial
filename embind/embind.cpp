
#include <iostream>
#include <emscripten.h>
#include <string>


#include <emscripten/bind.h>
using namespace emscripten;

float lerp(float a, float b, float t) {
    return (1 - t) * a + t * b;
}

class MyClass {
    public:
      MyClass(int x, std::string str) : x(x) , str(str) {}
      void incrementX() { ++x; }
      int getX() const { return x; }
      void setX(int x_) { x = x_; }
      void setString(const std::string& s) { str = s; }
      const std::string& getString() const { return str; }
      static std::string getClassString() { return "MyClass";}

    private:
      int x;
      std::string str;
};


EMSCRIPTEN_BINDINGS(my_module) {
    function("lerp", &lerp);

    class_<MyClass>("MyClass")
    .constructor<int, std::string>()
    .function("incrementX", &MyClass::incrementX)
    .property("x", &MyClass::getX, &MyClass::setX)
    .property("x_readonly", &MyClass::getX)
    .function("setString", &MyClass::setString)
    .function("getString", &MyClass::getString)
    .class_function("getClassString", &MyClass::getClassString)
    ;
}