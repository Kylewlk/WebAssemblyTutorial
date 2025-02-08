
#include <iostream>
#include <emscripten.h>

void fun( int n)
{
    std::cout << "Call fun: " << n << std::endl;
}



int main() 
{
    emscripten_run_script("console.log('Hello emscripten')");
    std::cout << "Hello, World!" << std::endl;
 
    std::cout << "sizeof(int*): " << sizeof(int*) << std::endl;

    fun(5);
    return 0;
}


