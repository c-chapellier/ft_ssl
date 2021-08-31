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

static bool is_zero(uint8_t *nb1, uint32_t size){
    for(uint32_t i = 0; i < size; ++i){
        if(nb1[i] != 0)
            return false;
    }
    return true;
}

uint8_t     *addition(uint8_t *nb1, uint8_t *nb2, uint32_t size)
{
    uint32_t double_size = size * 2;
    uint8_t x[double_size];
    uint8_t y[double_size];
    memset(x, 0, double_size);
    memcpy(&x[size], nb1, size);
    memset(y, 0, double_size);
    memcpy(&y[size], nb2, size);
    while (is_zero(y, double_size) == false)
    {
        // carry now contains common
        // set bits of x and y
        uint8_t carry[double_size];
        memset(carry, 0, double_size);
        for(uint32_t i = 0; i < double_size; ++i){
            carry[i] = x[i] & y[i];
        }
 
        // Sum of bits of x and y where at
        // least one of the bits is not set
        for(uint32_t i = 0; i < double_size; ++i){
            x[i] = x[i] ^ y[i];
        }
 
        // Carry is shifted by one so that adding
        // it to x gives the required sum
        uint8_t rest = 0;
        for(int i = (int)((double_size) - 1); i >= 0; --i){
            y[i] = carry[i] << 1 | rest;
            rest = 0;
            rest |= carry[i] >> 7;
        }
    }
    for(uint32_t i = 0; i < double_size; ++i)
        printf("%02x ", x[i]);
    puts("");
    uint8_t *res = malloc(double_size);
    memcpy(res, x, double_size);
    return res;
}