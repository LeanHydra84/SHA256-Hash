#include <iostream>
#include <iomanip>
#include <fstream>

// void print_chars(const char* buf, size_t count)
// {
//     for (size_t i = 0; i < count; i++)
//     {
//         std::cout << static_cast<int>(buf[i]) << ", ";
//     }
// }

int main(int argc, char** argv)
{
    std::string path = argv[1];

    std::cout << path << std::endl;

    // char buffer[1024];
    // size_t amount = 0;

    std::ifstream stream(path, std::ifstream::binary);
    if(!stream.is_open())
    {
        std::cout << "File not opened" << std::endl;
        return -1;
    }

    unsigned char x;

    std::cout << std::hex << std::uppercase;

    while(stream >> x)
    {
        // amount = static_cast<size_t>(stream.readsome(buffer, 1024));

        // std::cout << amount;
        // print_chars(buffer, amount);

        // if(amount != 1024)
        //     break;

        std::cout << static_cast<int>(x) << ", ";
    }

}