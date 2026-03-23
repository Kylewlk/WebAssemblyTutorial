#include <iostream>
#include <fstream>



int main()
{
    std::fstream file1 ("/d/t1.txt", std::ios::in);
    if (!file1.is_open())
    {
        std::cerr << "Failed to open file t1.txt" << std::endl;
        return 1;
    }
    std::string t1;
    std::getline(file1, t1);
    std::cout << "t1: " << t1 << std::endl;
    file1.close();

    std::fstream file2 ("/d/t2.txt", std::ios::in|std::ios::binary);
    if (!file2.is_open())
    {
        std::cerr << "Failed to open file t2.txt" << std::endl;
        return 1;
    }
    std::string t2;
    std::getline(file2, t2);
    std::cout << "t2: " << t2 << std::endl;
    file2.close();


    return 0;
}
