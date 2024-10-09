#include <cassert>
#include <string>

#include "hash.hpp"

struct Hash_Data
{
    byte* input;
    Hash hash;
    size_t length;
    size_t chunks;
};

// 512 bits == 64 bytes
struct Sequence512
{
    uint32_t seq[16];

    Sequence512() : seq() { }
    Sequence512(const byte* data) {
        std::memcpy(seq, data, sizeof(seq));
    }
};

#define V_A 0x6a09e667
#define V_B 0xbb67ae85
#define V_C 0x3c6ef372
#define V_D 0xa54ff53a
#define V_E 0x510e527f
#define V_F 0x9b05688c
#define V_G 0x1f83d9ab
#define V_H 0x5be0cd19

uint32_t K[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 
    0x59f111f1, 0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 
    0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 
    0xc19bf174, 0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da, 0x983e5152, 
    0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 
    0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 
    0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 
    0xd6990624, 0xf40e3585, 0x106aa070, 0x19a4c116, 0x1e376c08, 
    0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 
    0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

size_t get_block_count(size_t start)
{
    size_t rv = start / 512 + 1;
    if(rv - 64 < start) rv++;
    return rv;
}

Hash_Data prep_input(const byte* bytes, size_t size)
{
    // size in bytes
    size_t multiples_of_512 = get_block_count(size * 8); // bits

    // copy original data
    size_t bytesize = multiples_of_512 * 64; // * 512 / 8 (bits to bytes)
    byte* data = new byte[bytesize];
    std::memcpy(data, bytes, size);

    // fill padded bits
    data[size] = 0b10000000;
    memset(data + size + 1, 0, bytesize - size - 1);

    // fill length bits
    size_t* last64 = reinterpret_cast<size_t*>(data + bytesize - 4);
    *last64 = size * 8;

    Hash_Data rv = Hash_Data();
    rv.input = data;
    rv.length = bytesize;
    rv.chunks = multiples_of_512;
    
    // Initialize Hash values
    uint32_t* hs = rv.hash.hash;

    hs[0] = V_A;
    hs[1] = V_C;
    hs[2] = V_C;
    hs[3] = V_D;
    hs[4] = V_E;
    hs[5] = V_F;
    hs[6] = V_G;
    hs[7] = V_H;

    return rv;
}

uint32_t rotate_left32(uint32_t n, int rotations)
{
    uint32_t lower = n << rotations; // stores lower order bits
    uint32_t higher = n >> (32 - rotations);

    return lower | higher;
}

uint32_t rotate_right32(uint32_t n, int rotations)
{
    uint32_t higher = n >> rotations;
    uint32_t lower = n << (32 - rotations);

    return higher | lower;
}

// this is so gross lol
#define H(x) (current_hash.hash[(#x)[0] - 'a'])

void process_chunk(Sequence512 chunk, Hash_Data& data)
{
    // Setup
    uint32_t schedule[64];
    std::memcpy(schedule, chunk.seq, sizeof(chunk.seq)); // copy data into first 16 words

    Hash current_hash = data.hash;

    for(int i = 16; i < 64; i++)
    {
        uint32_t s0 = rotate_right32(schedule[i - 15], 7) ^ rotate_right32(schedule[i - 15], 18) ^ (schedule[i - 15] >> 3);
        uint32_t s1 = rotate_right32(schedule[i - 2], 17) ^ rotate_right32(schedule[i - 2], 19) ^ (schedule[i - 2] >> 10);
        schedule[i] = schedule[i - 16] + s0 + schedule[i - 7] + s1;
    }

    // Compression
    for(int i = 0; i < 64; i++)
    {
        uint32_t S1 = rotate_right32(H(e), 6) ^ rotate_right32(H(e), 11) ^ rotate_right32(H(e), 25);
        uint32_t ch = (H(e) & H(f)) ^ ((~H(e)) & H(g));
        uint32_t temp1 = H(h) + S1 + ch + K[i] + schedule[i];
        uint32_t S0 = rotate_right32(H(a), 2) ^ rotate_right32(H(a), 13) ^ rotate_right32(H(a), 22);
        uint32_t maj = (H(a) & H(b)) ^ (H(a) & H(c)) ^ (H(b) & H(c));
        uint32_t temp2 = S0 + maj;

        H(h) = H(g);
        H(g) = H(f);
        H(f) = H(e);
        H(e) = H(d) + temp1;
        H(d) = H(c);
        H(c) = H(b);
        H(b) = H(a);
        H(a) = temp1 + temp2;
    }

    uint32_t* hs = data.hash.hash;

    hs[0] += H(a);
    hs[1] += H(b);
    hs[2] += H(c);
    hs[3] += H(d);
    hs[4] += H(e);
    hs[5] += H(f);
    hs[6] += H(g);
    hs[7] += H(h);

}

Hash hash(const byte* bytes, size_t size)
{
    Hash_Data hdata = prep_input(bytes, size);

    byte* iterator = hdata.input;
    for(int i = 0; i < hdata.chunks; i++)
    {
        process_chunk(Sequence512(iterator), hdata);
        iterator += 64;
    }

    delete[] hdata.input;
    return hdata.hash;
}

Hash hash_string(const std::string& str)
{
    const byte* asbytes = reinterpret_cast<const byte*>(str.c_str());
    size_t bytesize = str.length() * sizeof(str[0]);

    return hash(asbytes, bytesize);
}

// Ostream

std::ostream& operator<<(std::ostream& stream, const Hash& _hash)
{
    stream << std::hex;
    for (int i = 0; i < 8; i++)
        stream << _hash.hash[i];
    stream << std::dec;

    return stream;
}

std::ostream& operator<<(std::ostream& stream, const Hash_Data& _data)
{
    stream << "Chunks: " << _data.chunks << std::endl
        << "Length: " << _data.length << std::endl
        << "Current Hash: " << _data.hash << std::endl
        << "Pointer Address: " << std::hex << reinterpret_cast<int*>(_data.input) << std::dec << std::endl;

    return stream;
}