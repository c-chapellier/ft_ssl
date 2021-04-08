#include "../ft_ssl.h"

uint32_t right_rotate_32(uint32_t value, unsigned int count)
{
    assert(count < 32), assert(count > 0);
    return (value >> count | value << (32 - count));
}

uint32_t left_rotate_32(uint32_t value, unsigned int count)
{
    assert(count < 32), assert(count > 0);
    return (value << count | value >> (32 - count));
}

uint32_t big_to_small_endian_32(uint32_t n)
{
    return (((n >> 24) & 0xff) | ((n << 8) & 0xff0000) | ((n >> 8) & 0xff00) | ((n << 24) & 0xff000000));
}

uint64_t small_to_big_endian_64(uint64_t n)
{
    n = ((n << 8) & 0xFF00FF00FF00FF00ULL) | ((n >> 8) & 0x00FF00FF00FF00FFULL);
    n = ((n << 16) & 0xFFFF0000FFFF0000ULL) | ((n >> 16) & 0x0000FFFF0000FFFFULL);
    n = (n << 32) | (n >> 32);
    return (n);
}
