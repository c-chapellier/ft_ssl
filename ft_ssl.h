#ifndef FT_SSL_H
#define FT_SSL_H

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h> 
#include <sys/stat.h>
#include <assert.h>

int md5(int argc, char *argv[]);
int sha256(int argc, char *argv[]);

int base64(int argc, char *argv[]);

uint32_t right_rotate_32(uint32_t value, unsigned int count);
uint32_t left_rotate_32(uint32_t value, unsigned int count);
uint32_t big_to_small_endian_32(uint32_t n);
uint64_t small_to_big_endian_64(uint64_t n);

char *read_file(const char *filename);
char *read_fd(int fd);
int write_file(const char *filename, const char *content);

void print_uint8(uint8_t n);
void print_uint32(uint32_t n);

#endif