#ifndef _FILEIO_HPP_
#define _FILEIO_HPP_

#include <string>
#include <cstdint>

using byte = uint8_t;

// File IO
byte* read_file(const std::string& path, size_t& size);
void write_file(const std::string& path, const byte* buffer, size_t size);
std::string replace_file_extension(const std::string& filepath, const std::string& new_extension);


#endif