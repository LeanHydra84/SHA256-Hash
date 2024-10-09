#ifndef __HASH_HPP_
#define __HASH_HPP_

#include <cstdint>
#include <ostream>

using byte = uint8_t;

struct Hash
{
    uint32_t hash[8];
};

std::ostream& operator<<(std::ostream& stream, const Hash& _hash);
Hash hash(const byte* bytes, size_t size);
Hash hash_string(const std::string& str);

#endif