#ifndef FT_SSL_H
#define FT_SSL_H

#include <fcntl.h>      // open
#include <unistd.h>     // close, read, write, lseek
#include <stdlib.h>     // malloc, free
#include <stdio.h>      // printf
#include <string.h>     // strcmp, strlen
#include <stdint.h>     // uint32_t
#include <sys/mman.h>   // mmap
#include <assert.h>     // assert

typedef struct  algorithm_s
{
    char    *name;
    char    *uppercase_name;
    void    (*hash)(uint8_t message[], uint8_t digest[]);
    uint8_t digest_size_in_bytes;
}               algorithm_t;

#define ARG_P 1
#define ARG_Q 2
#define ARG_R 4
#define ARG_S 8
#define READ_FROM_STDIN 16

typedef struct  args_s
{
    char        *name;
    uint32_t    value;
}               args_t;

int     algorithm_cli(int argc, char *argv[], algorithm_t algorithm);
void    md5(uint8_t message[], uint8_t digest[16]);
void    sha256(uint8_t message[], uint8_t digest[32]);


uint32_t right_rotate_32(uint32_t value, unsigned int count);
uint32_t left_rotate_32(uint32_t value, unsigned int count);
uint32_t big_to_small_endian_32(uint32_t n);
uint64_t small_to_big_endian_64(uint64_t n);

char    *read_file(const char *filename);
void    read_fd(int fd, int n, char buffer[n]);

void    print_uint8(uint8_t n);
void    print_uint32(uint32_t n);

#endif