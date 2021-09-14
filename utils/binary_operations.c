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

uint8_t *add_extra_bytes(uint8_t *nb, uint32_t size, uint32_t new_size)
{
    if (size == new_size || new_size < size)
        return nb;
    uint8_t *res = calloc(new_size, 1);
    memcpy(&res[new_size - size], nb, size);
    free(nb);
    return res;
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

uint8_t *multiplication(uint8_t *nb1, uint8_t *nb2, uint32_t size, uint32_t *new_size)
{
    // make a copy of nb1 and nb2 with bigger buff
    uint32_t double_size = size * 2;
    uint8_t *a = calloc(double_size, 1);
    uint8_t *b = calloc(double_size, 1);
    uint8_t *res = calloc(double_size, 1);
    memcpy(&a[size], nb1, size);
    memcpy(&b[size], nb2, size);

    uint32_t current_size = double_size;
    // While second number doesn't become 1
    while (is_zero(b, current_size) == false)
    {
        if(b[current_size -1] & 1){
            uint32_t tmp_size = 0;
            res = addition(res, a, current_size, &tmp_size);
            res = add_extra_bytes(res, tmp_size, current_size);
        }
        // Double the first number and halve the second number
        shift_left(a, current_size, 1);
        shift_right(b, current_size, 1);
    }
    res = discard_useless_bytes(res, current_size, new_size);
    return res;
}

static void    print_decimal(uint8_t *nb, uint32_t size) {
    uint8_t tmp;
    char    base[10] = "0123456789";
    printf("decimal\n");
    for(uint32_t i = 0; i < size; ++i) {
        
        tmp = nb[i] >> 4;
        if(i == 0 && tmp == 0){}else{write(1, &base[tmp], 1);}
        tmp = (nb[i] << 4);
        tmp = tmp >> 4;
        write(1, &base[tmp], 1);
    }
    write(1, "\n", 1);
}

uint8_t *double_dabble(uint8_t *nb, uint32_t size, uint32_t *new_size) {
    uint32_t nb_turn = size * 8;
    uint32_t triple_size = size * 3;
    uint8_t *res = calloc(triple_size, 1);

    for (uint32_t i = 0; i < nb_turn; ++i) {
        // print_nb("res", res, triple_size);
        // print_nb("nb", nb, size);
        for(uint32_t k = triple_size - 1; k < triple_size; --k) {
            uint8_t tmp;
            tmp = 0;
            tmp = res[k] << 4;
            tmp >>= 4;
            if(tmp >= 5){
                uint32_t nb_tmp_size;
                uint8_t *nb_tmp = calloc(triple_size, 1);
                uint8_t *three = calloc(triple_size, 1);
                three[k] = 3;
                memcpy(nb_tmp, res, triple_size);
                nb_tmp = addition(nb_tmp, three, triple_size, &nb_tmp_size);
                nb_tmp = add_extra_bytes(nb_tmp, nb_tmp_size, triple_size);
                res = nb_tmp;
                // printf("ADD-3\n");
                // print_nb("res", res, triple_size);
                // print_nb("nb", nb, size);
                // tmp += 3;
                // res[k] >>= 4;
                // res[k] <<= 4;
                // res[k] |= tmp;
                // break ;
            }
            tmp = 0;
            tmp = res[k] >> 4;
            if(tmp >= 5){
                uint32_t nb_tmp_size;
                uint8_t *nb_tmp = calloc(triple_size, 1);
                uint8_t *three = calloc(triple_size, 1);
                three[k] = 3 << 4;
                memcpy(nb_tmp, res, triple_size);
                nb_tmp = addition(nb_tmp, three, triple_size, &nb_tmp_size);
                nb_tmp = add_extra_bytes(nb_tmp, nb_tmp_size, triple_size);
                res = nb_tmp;
                // printf("ADD-3\n");
                // print_nb("res", res, triple_size);
                // print_nb("nb", nb, size);
                // tmp += 3;
                // res[k] <<= 4;
                // res[k] >>= 4;
                // res[k] |= (tmp << 4);
                // break ;
            }
        }
        // printf("SHIFT\n");
        shift_left(res, triple_size, 1);
        res[triple_size - 1] |= (nb[0] & 128) >> 7;
        shift_left(nb, size, 1);
    }
    // print_nb("res", res, triple_size);
    // print_nb("nb", nb, size);
    res = discard_useless_bytes(res, triple_size, new_size);
    print_decimal(res, *new_size);
    return res;
}