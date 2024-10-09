#include <fstream>
#include <iostream>
#include "cipher.hpp"

#define CONST_HEADER_SIZE 8

byte* read_file(const std::string& path, size_t& size)
{
    std::ifstream stream;
    stream.open(path, std::ifstream::binary | std::ifstream::ate);

    if(!stream.is_open())
    {
        std::cout << "Failed to open read stream" << std::endl;
        return nullptr;
    }

    size = static_cast<size_t>(stream.tellg());
    stream.seekg(0, std::ifstream::beg);

    byte* buffer = new byte[size];
    stream.read(reinterpret_cast<char*>(buffer), size);

    stream.close();

    return buffer;
}


void write_file(const std::string& path, const byte* buffer, size_t size)
{
    std::ofstream stream(path, std::ofstream::binary);

    if(!stream.is_open())
    {
        std::cout << "Failed to open write stream" << std::endl;
        return;
    }

    stream.write(reinterpret_cast<const char*>(buffer), size);
    stream.close();

}

std::string replace_file_extension(const std::string& filepath, const std::string& new_extension)
{
    if(std::find(filepath.cbegin(), filepath.cend(), '.') == filepath.cend())
        return filepath + "." + new_extension;
    return filepath.substr(0, filepath.find_last_of('.')) + new_extension;
}
