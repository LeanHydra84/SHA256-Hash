#include <iostream>
#include "fileio.hpp"
#include "hash.hpp"

int main(int argc, char** argv)
{
    if(argc == 1)
    {
        std::cout << "No input!" << std::endl;
        return -1;
    }

    std::string asPath = argv[1];
    size_t fileSize;
    byte* data = read_file(asPath, fileSize);

    std::cout << "\"" << asPath << "\"" << ": " << std::endl;

    Hash asHash = hash(data, fileSize);
    std::cout << asHash << std::endl;
}