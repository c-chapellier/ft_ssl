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

// Initial values
uint32_t a0 = 0x67452301;
uint32_t b0 = 0xefcdab89;
uint32_t c0 = 0x98badcfe;
uint32_t d0 = 0x10325476;

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
        g *= 4;
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

static void hash(uint8_t message[], uint8_t digest[16])
{    
    int     len8 = strlen((char *)message);
    int     n = (len8 / 64) + 1;                // # chunks of 512 bits
    uint8_t chunks[n * 64];

    preprocess(message, chunks);

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

void fill_args_d(int n, args_t args[n])
{
    args[0].name = "-p";
    args[0].value = ARG_P;
    args[1].name = "-q";
    args[1].value = ARG_Q;
    args[2].name = "-r";
    args[2].value = ARG_R;
    args[3].name = "-s";
    args[3].value = ARG_S;
}

char    *read_file(const char *filename)
{
    int     fd = open(filename, O_RDONLY);
    int     len;
    char    *file_content;

    if (fd == -1)
    {
        printf("md5: %s: No such file or directory\n", filename);
        return (NULL);
    }
    len = lseek(fd, 0, SEEK_END);
    file_content = mmap(0, len, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);
    return (file_content);
}

uint8_t fill_args(int argc, char *argv[], uint8_t *message[])
{
    int     n = 4;
    args_t  args_d[n];
    uint8_t args = 0;

    (void)argc;
    fill_args_d(n, args_d);
    for (int i = 0; argv[i] != NULL; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            if (strcmp(argv[i], args_d[j].name) == 0)
            {
                args |= args_d[j].value;
                if (args_d[j].value == ARG_S)
                {
                    *message = (uint8_t *)argv[i + 1];
                    if (*message == NULL)
                    {
                        printf("md5: option requires an argument -- s\n");
                        printf("usage: md5 [-pqr] [-s string] [files ...]\n");
                        return (EXIT_FAILURE);
                    }
                }
            }
        }
    }
    return (args);
}

int md5(int argc, char *argv[])
{
    uint8_t args;
    char    *filename = NULL;
    uint8_t *message = NULL;
    uint8_t digest[16];                         // 128 bits

    args = fill_args(argc, argv, &message);
    
    if (argc == 0)
    {
        printf("md5: reading from stdin is not yet supported.\n");
        return (EXIT_FAILURE);
    }
    filename = argv[argc - 1];
    if ((args & ARG_S) == 0)
    {
        message = (uint8_t *)read_file(filename);
        if (message == NULL)
        {
            return (EXIT_FAILURE);
        }
        // printf("file(%s) = [%s]\n", filename, message);
    }

    // print_uint8(args);
    // printf("\n");
    // printf("message = [%s]\n", message);

    hash(message, digest);

    if ((args & ARG_Q) == 0 && (args & ARG_R) == 0)
    {
        if (args & ARG_S)
        {
            printf("MD5 (\"%s\") = ", (char *)message);
        }
        else
        {
            printf("MD5 (%s) = ", filename);
        }
    }

    for (int i = 0; i < 16; ++i)
    {
        printf("%02x", digest[i]);
    }

    if ((args & ARG_Q) == 0 && args & ARG_R)
    {
        if (args & ARG_S)
        {
            printf(" \"%s\"", (char *)message);
        }
        else
        {
            printf(" %s", filename);
        }
    }
    printf("\n");
    return (EXIT_SUCCESS);
}