#ifndef FT_SSL_H
#define FT_SSL_H

#include <fcntl.h>      // open
#include <unistd.h>     // close, read, write
#include <stdlib.h>     // malloc, free
#include <stdio.h>      // printf
#include <string.h>     // strdup, strcmp, strlen
#include <stdint.h>     // uint32_t
#include <sys/mman.h>   // mmap

typedef struct  algorithm_s
{
    char    *name;
    int     (*algorithm)(int argc, char *argv[]);
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

int     md5(int argc, char *argv[]);
void    md5_hash(uint8_t message[], uint8_t digest[16]);
int     sha256(int argc, char *argv[]);

void    print_uint8(uint8_t n);
void    print_uint32(uint32_t n);
char    *read_file(const char *filename);
void    read_fd(int fd, int n, char buffer[n]);

#endif