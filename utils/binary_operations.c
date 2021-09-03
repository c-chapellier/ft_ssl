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

static uint8_t     *discard_useless_bytes(uint8_t *nb, uint32_t size, uint32_t *new_size)
{
    uint32_t c = 0;

    for (uint32_t i = 0; i < size; ++i) {
        if(nb[i] != 0)
            break;
        ++c;
    }
    *new_size = size -c;
    uint8_t *res = calloc(size - c, 1);
    memcpy(res, &nb[c], size -c);
    return res;
}

static bool is_zero(uint8_t *nb1, uint32_t size){
    for(uint32_t i = 0; i < size; ++i){
        if(nb1[i] != 0)
            return false;
    }
    return true;
}

void shift_left(uint8_t *number, uint32_t size, uint32_t bit) {
    uint8_t rest = 0;
    uint8_t cpy[size];
    memcpy(cpy, number, size);

    for(int i = (int)((size) - 1); i >= 0; --i){
        number[i] = number[i] << bit | rest;
        rest = 0;
        rest |= cpy[i] >> (8 - bit);
    }
}

void shift_right(uint8_t *number, uint32_t size, uint32_t bit) {
    uint8_t rest = 0;
    uint8_t cpy[size];
    memcpy(cpy, number, size);

    for(int i = 0; i < (int)size; ++i){
        number[i] = number[i] >> bit | rest;
        rest = 0;
        rest |= cpy[i] << (8 - bit);
    }
}

uint8_t     *addition(uint8_t *nb1, uint8_t *nb2, uint32_t size, uint32_t *new_size)
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
    uint8_t *res = calloc(double_size, 1);
    memcpy(res, x, double_size);
    res = discard_useless_bytes(res, double_size, new_size);
    return res;
}

uint8_t     *substraction(uint8_t *nb1, uint8_t *nb2, uint32_t size, uint32_t *new_size)
{
    uint8_t x[size];
    uint8_t y[size];
    memset(x, 0, size);
    memcpy(x, nb1, size);
    memset(y, 0, size);
    memcpy(y, nb2, size);

    while (is_zero(y, size) == false)
    {
        // carry now contains common
        // set bits of x and y
        uint8_t borrow[size];
        memset(borrow, 0, size);
        for(uint32_t i = 0; i < size; ++i){
            borrow[i] = ~(x[i]) & y[i];
        }
 
        // Sum of bits of x and y where at
        // least one of the bits is not set
        for(uint32_t i = 0; i < size; ++i){
            x[i] = x[i] ^ y[i];
        }
 
        // Carry is shifted by one so that adding
        // it to x gives the required sum
        uint8_t rest = 0;
        for(int i = (int)((size) - 1); i >= 0; --i){
            y[i] = borrow[i] << 1 | rest;
            rest = 0;
            rest |= borrow[i] >> 7;
        }
    }
    uint8_t *res = calloc(size, 1);
    memcpy(res, x, size);
    res = discard_useless_bytes(res, size, new_size);
    return res;
}

uint8_t *multiplication(uint8_t *nb1, uint8_t *nb2, uint32_t size, uint32_t *new_size)
{
    uint32_t tmp_size = size * 2;
    uint8_t *a = calloc(tmp_size, 1);
    uint8_t *b = calloc(tmp_size, 1);
    uint8_t *res = calloc(tmp_size, 1);
    memcpy(&a[size], nb1, size);
    memcpy(&b[size], nb2, size);

    bool first = true;
 
    // While second number doesn't become 1
    while (is_zero(b, tmp_size) == false)
    {
        // If second number becomes odd, add the first number to result
        if (b[tmp_size - 1] & 1)
        {
            uint32_t s = 0;
            res = addition(res, a, tmp_size, &s);
            s = s + 1;
            uint8_t *tmptmp = calloc(s, 1);
            memcpy(&tmptmp[1], res, s);
            res = tmptmp;

            if(first){
                uint8_t *tmp1 = calloc(s, 1);
                memcpy(tmp1, &a[tmp_size - s], s);
                free(a);
                a = tmp1;

                uint8_t *tmp2 = calloc(s, 1);
                memcpy(tmp2, &b[tmp_size - s], s);
                free(b);
                b = tmp2;

                first = false;
            } else {
                uint8_t *tmp1 = calloc(s, 1);
                memcpy(&tmp1[s - tmp_size], a, tmp_size);
                free(a);
                a = tmp1;

                uint8_t *tmp2 = calloc(s, 1);
                memcpy(&tmp2[s - tmp_size], b, tmp_size);
                free(b);
                b = tmp2;
            }
            tmp_size = s;
        }

        // Double the first number and halve the second number
        shift_left(a, tmp_size, 1);
        shift_right(b, tmp_size, 1);
    }
    res = discard_useless_bytes(res, tmp_size, new_size);
    return res;
}