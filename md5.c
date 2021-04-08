#include "ft_ssl.h"

// // All variables are unsigned 32 bit and wrap modulo 2^32 when calculating

// // s specifies the per-round shift amounts
uint32_t s[64] = {
    7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
    5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
    4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
    6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21
};

// // Use binary integer part of the sines of integers (Radians) as constants:
// for i from 0 to 63 do
//     K[i] = floor(232 × abs (sin(i + 1)))
// end for
// (Or just use the following precomputed table):

uint32_t K[64] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

uint32_t a0, b0, c0, d0;

static void preprocess(uint8_t message[], uint8_t chunks[])
{
    int     len8 = strlen((char *)message);
    int     n = (len8 / 64) + 1;                // # chunks of 512 bits

    // set all zeros
    memset(chunks, 0, n * 64);
    
    // copy message
    memcpy(chunks, message, len8);

    // append 1
    chunks[len8] = 1 << 7;

    // append size of message
    unsigned long long size = len8 * 8;         // must be 64 bits
    memcpy(&chunks[(n * 64) - 8], &size, sizeof size);
}

static void hash_chunk(uint8_t chunk[])
{
    uint32_t A = a0;
    uint32_t B = b0;
    uint32_t C = c0;
    uint32_t D = d0;

    for (int i = 0; i < 64; ++i)
    {
        uint32_t F, g;
        if (i < 16)
        {
            F = (B & C) | ((~B) & D);
            g = i;
        }
        else if (i < 32)
        {
            F = (D & B) | ((~D) & C);
            g = ((5 * i) + 1) % 16;
        }
        else if (i < 48)
        {
            F = B ^ C ^ D;
            g = ((3 * i) + 5) % 16;
        }
        else
        {
            F = C ^ (B | (~D));
            g = (7 * i) % 16;
        }
        g *= 4;                                 // because chunk is a uint8_t array and the algo is for uint32_t
        F = F + A + K[i] + (chunk[g + 3] << 24) + (chunk[g + 2] << 16) + (chunk[g + 1] << 8) + chunk[g];
        A = D;
        D = C;
        C = B;
        B += (F << s[i]) | (F >> (32 - s[i]));
    }
    a0 += A;
    b0 += B;
    c0 += C;
    d0 += D;
}

void        md5(uint8_t message[], uint8_t digest[16])
{    
    int     len8 = strlen((char *)message);
    int     n = (len8 / 64) + 1;                // # chunks of 512 bits
    uint8_t chunks[n * 64];

    preprocess(message, chunks);

    a0 = 0x67452301;
    b0 = 0xefcdab89;
    c0 = 0x98badcfe;
    d0 = 0x10325476;
    for (int i = 0; i < n; ++i)
    {
        hash_chunk(&chunks[i * 64]);
    }

    // (Output is in little-endian)
    memcpy(&digest[0], &a0, sizeof a0);
    memcpy(&digest[4], &b0, sizeof b0);
    memcpy(&digest[8], &c0, sizeof c0);
    memcpy(&digest[12], &d0, sizeof d0);
}