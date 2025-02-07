
#include <iostream>
#include <emscripten.h>

int main() 
{

    emscripten_run_script("console.log('Hello emscripten')");
    std::cout << "Hello, World!" << std::endl;
    return 0;
}


