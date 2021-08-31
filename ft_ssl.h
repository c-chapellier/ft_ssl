#ifndef FT_SSL_H
#define FT_SSL_H

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h> 
#include <sys/stat.h>
#include <limits.h>
#include <assert.h>
#include <errno.h>
#include <getopt.h>
#include <stdbool.h>

#define MD5_DIGEST_SIZE 16

#define PRINT_UINT64(n) print_uint8(&((uint8_t *)n)[0]), printf(" "), print_uint8(&((uint8_t *)n)[1]), printf(" "), print_uint8(&((uint8_t *)n)[2]), printf(" "), print_uint8(&((uint8_t *)n)[3]), printf(" "), print_uint8(&((uint8_t *)n)[4]), printf(" "), print_uint8(&((uint8_t *)n)[5]), printf(" "), print_uint8(&((uint8_t *)n)[6]), printf(" "), print_uint8(&((uint8_t *)n)[7])
#define PRINT_UINT56(n) print_uint8(&((uint8_t *)n)[0]), printf(" "), print_uint8(&((uint8_t *)n)[1]), printf(" "), print_uint8(&((uint8_t *)n)[2]), printf(" "), print_uint8(&((uint8_t *)n)[3]), printf(" "), print_uint8(&((uint8_t *)n)[4]), printf(" "), print_uint8(&((uint8_t *)n)[5]), printf(" "), print_uint8(&((uint8_t *)n)[6])
#define PRINT_UINT48(n) print_uint8(&((uint8_t *)n)[0]), printf(" "), print_uint8(&((uint8_t *)n)[1]), printf(" "), print_uint8(&((uint8_t *)n)[2]), printf(" "), print_uint8(&((uint8_t *)n)[3]), printf(" "), print_uint8(&((uint8_t *)n)[4]), printf(" "), print_uint8(&((uint8_t *)n)[5]), printf(" ")
#define PRINT_UINT40(n) print_uint8(&((uint8_t *)n)[0]), printf(" "), print_uint8(&((uint8_t *)n)[1]), printf(" "), print_uint8(&((uint8_t *)n)[2]), printf(" "), print_uint8(&((uint8_t *)n)[3]), printf(" "), print_uint8(&((uint8_t *)n)[4])
#define PRINT_UINT32(n) print_uint8(&((uint8_t *)n)[0]), printf(" "), print_uint8(&((uint8_t *)n)[1]), printf(" "), print_uint8(&((uint8_t *)n)[2]), printf(" "), print_uint8(&((uint8_t *)n)[3])
#define PRINT_UINT24(n) print_uint8(&((uint8_t *)n)[0]), printf(" "), print_uint8(&((uint8_t *)n)[1]), printf(" "), print_uint8(&((uint8_t *)n)[2])
#define PRINT_UINT16(n) print_uint8(&((uint8_t *)n)[0]), printf(" "), print_uint8(&((uint8_t *)n)[1])
#define PRINT_UINT8(n) print_uint8(&((uint8_t *)n)[0])

int genrsa(int argc, char *argv[]);
int rsa(int argc, char *argv[]);
int rsautl(int argc, char *argv[]);

int md5(int argc, char *argv[]);
void md5_hash(char *msg, uint8_t digest[MD5_DIGEST_SIZE]);
int sha256(int argc, char *argv[]);

int base64(int argc, char *argv[]);
int base64_encrypt(int n, char msg[n], char e[]);
int base64_decrypt(char *msg, char *e);

int des(int argc, char *argv[]);
int des_ecb(int argc, char *argv[]);

uint32_t right_rotate_32(uint32_t value, unsigned int count);
uint32_t left_rotate_32(uint32_t value, unsigned int count);
uint32_t big_to_small_endian_32(uint32_t n);
uint64_t small_to_big_endian_64(uint64_t n);

uint8_t     *addition(uint8_t *nb1, uint8_t *nb2, uint32_t size, uint32_t *new_size);
uint8_t     *substraction(uint8_t *nb1, uint8_t *nb2, uint32_t size, uint32_t *new_size);
uint8_t     *multiplication(uint8_t *nb1, uint8_t *nb2, uint32_t size, uint32_t *new_size);

char *read_file(const char *filename);
char *read_file_n(const char *filename, int *bytes_read);
char *read_fd(int fd);
char *read_fd_n(int fd, int *bytes_read);
int write_file(const char *filename, const char *content);
int write_buffer_to_file(const char *filename, int n, const char content[n]);

void print_uint8(uint8_t *n);

#endif