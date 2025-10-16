
#include <iostream>
#include <emscripten.h>
#include <thread>
std::thread t ;

void fun(int n)
{
    std::cout << "Call fun: " << n << std::endl;
}



int main()
{
    emscripten_run_script("console.log('Hello emscripten')");
    std::cout << "Hello, World!" << std::endl;
    t = std::thread([]()
                    { std::cout << "Hello from a thread!" << std::endl; });
    t.join();

    std::cout << "sizeof(int*): " << sizeof(int*) << std::endl;

    fun(5);
    return 0;
}


