#ifndef _CIPHER_HPP_
#define _CIPHER_HPP_

#include "fileio.hpp"

// Substitution
byte substitute(byte in);
byte reverse_substitute(byte in);

// Encryption
byte encrypt_byte(byte in);
byte decrypt_byte(byte in);

#endif