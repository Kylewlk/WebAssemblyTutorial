
#include <iostream>
#include <string>


std::string liba()
{
    return "liba-test-library";
}

int funAdd(int n, int m)
{
    std::cout << "funAdd: " << n << " + " << m << " = " << n + m << std::endl;
    return n + m;
}
