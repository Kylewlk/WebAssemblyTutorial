#include <iostream>
#include <fstream>
#include <emscripten.h>

std::string liba();

int funAdd(int n, int m);

int main()
{
    std::cout << liba() << std::endl;
    
    std::cout << funAdd(1, 2) << std::endl;
    return 0;
}
