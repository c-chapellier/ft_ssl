#include "../ft_ssl.h"

#define SHA256_DIGEST_SIZE 32

// // All variables are unsigned 32 bit and wrap modulo 2^32 when calculating

// Initialize hash values:
// (first 32 bits of the fractional parts of the square roots of the first 8 primes 2..19):
static uint32_t h[8];

// Initialize array of round constants:
// (first 32 bits of the fractional parts of the cube roots of the first 64 primes 2..311):
static uint32_t k[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

static int pflag = 0, qflag = 0, rflag = 0, sflag = 0;
static char *svalue = NULL;

static int hash_stdin();
static int hash_empty_string();
static int hash_args();
static int hash_files(char *filenames[]);
static void sha256_hash(char *msg, uint8_t digest[16]);
static void preprocess(char *msg, uint8_t chunks[]);
static void hash_chunk(uint8_t chunk[]);

int sha256(int argc, char *argv[])
{
    int c;

    opterr = 0;
    while ((c = getopt(argc, argv, "pqrs:")) != EOF)
    {
        switch (c)
        {
        case 'p':
            pflag = 1;
            break;
        case 'q':
            qflag = 1;
            break;
        case 'r':
            rflag = 1;
            break;
        case 's':
            sflag = 1;
            svalue = optarg;
            break;
        case '?':
            if (optopt == 's')
                fprintf(stderr, "sha256: option -%c requires an argument.\n", optopt);
            else
                fprintf(stderr, "Unknown option `-%c'.\n", optopt);
            return 1;
        default:
            fprintf(stderr, "usage: sha256 [-pqr] [-s string] [files ...]\n");
            return 1;
        }
    }

    if (argc == 1 || pflag || (!sflag && optind >= argc))
    {
        hash_stdin();
        if (pflag && (qflag || rflag) && !sflag)
            return hash_empty_string();
    }

    if (sflag)
        return hash_args();
    return hash_files(&argv[optind]);
}

static int hash_stdin()
{
    char *msg;
    uint8_t digest[SHA256_DIGEST_SIZE]; 
    
    msg = read_fd(STDIN_FILENO);
    sha256_hash(msg, digest);

    if (pflag)
        printf("%s", msg);
    for (int i = 0; i < SHA256_DIGEST_SIZE; ++i)
        printf("%02x", digest[i]);
    printf("\n");
    free(msg);
    return 0;
}

static int hash_empty_string()
{
    uint8_t digest[SHA256_DIGEST_SIZE]; 
    
    sha256_hash("", digest);

    if (!qflag && !rflag)
        printf("SHA256(\"%s\")= ", svalue);
    for (int i = 0; i < SHA256_DIGEST_SIZE; ++i)
        printf("%02x", digest[i]);
    printf("\n");
    return 0;
}

static int hash_args()
{
    uint8_t digest[SHA256_DIGEST_SIZE]; 
    
    sha256_hash(svalue, digest);

    if (!qflag && !rflag)
        printf("SHA256(\"%s\")= ", svalue);
    for (int i = 0; i < SHA256_DIGEST_SIZE; ++i)
        printf("%02x", digest[i]);
    if (!qflag && rflag)
        printf(" \"%s\"", svalue);
    printf("\n");
    return 0;
}

static int hash_files(char *filenames[])
{
    int rc = 0;
    char *msg;
    uint8_t digest[SHA256_DIGEST_SIZE]; 
    
    for (int i = 0; filenames[i] != NULL; ++i)
    {
        msg = read_file(filenames[i]);
        if (msg == NULL)
        {
            rc = 1;
            continue ;
        }
        sha256_hash(msg, digest);
        free(msg);

        if (!qflag && !rflag)
            printf("SHA256(%s)= ", filenames[i]);
        for (int i = 0; i < SHA256_DIGEST_SIZE; ++i)
            printf("%02x", digest[i]);
        if (!qflag && rflag)
            printf(" %s", filenames[i]);
        printf("\n");
    }
    return rc;
}

static void sha256_hash(char *msg, uint8_t digest[32])
{    
    int n_chunks = ((strlen(msg) + 8) / 64) + 1;         // # chunks of 512 bits
    uint8_t chunks[n_chunks * 64];

    preprocess(msg, chunks);

    h[0] = 0x6a09e667;
    h[1] = 0xbb67ae85;
    h[2] = 0x3c6ef372;
    h[3] = 0xa54ff53a;
    h[4] = 0x510e527f;
    h[5] = 0x9b05688c;
    h[6] = 0x1f83d9ab;
    h[7] = 0x5be0cd19;
    for (int i = 0; i < n_chunks; ++i)
    {
        hash_chunk(&chunks[i * 64]);
    }

    // (Output is in big-endian)
    for (int i = 0; i < 8; ++i)
    {
        h[i] = big_to_small_endian_32(h[i]);
        memcpy(&digest[i * 4], &h[i], sizeof h[i]);
    }
}

static void preprocess(char *msg, uint8_t chunks[])
{
    int msg_len = strlen(msg);
    int chunks_size = (((msg_len + 8) / 64) + 1) * 64;

    // set all zeros
    memset(chunks, 0, chunks_size);
    
    // copy msg
    memcpy(chunks, msg, msg_len);

    // append 1
    chunks[msg_len] = 1 << 7;

    // append size of msg
    uint64_t size = small_to_big_endian_64(msg_len * 8);
    memcpy(&chunks[chunks_size - 8], &size, 8);
}

static void hash_chunk(uint8_t chunk[64])
{
    // create a 64-entry msg schedule array w[0..63] of 32-bit words
    // (The initial values in w[0..63] don t matter, so many implementations zero them here)
    uint32_t w[64] = {0};

    // copy chunk into first 16 words w[0..15] of the msg schedule array
    for (int i = 0; i < 16; ++i)
    {
        int j = 4 * i;
        w[i] = (chunk[j] << 24) + (chunk[j + 1] << 16) + (chunk[j + 2] << 8) + chunk[j + 3];
    }

    // Extend the first 16 words into the remaining 48 words w[16..63] of the msg schedule array:
    for (int i = 16; i < 64; ++i)
    {
        uint32_t s0 = right_rotate_32(w[i - 15], 7) ^ right_rotate_32(w[i - 15], 18) ^ (w[i - 15] >> 3);
        uint32_t s1 = right_rotate_32(w[i - 2], 17) ^ right_rotate_32(w[i - 2], 19) ^ (w[i - 2] >> 10);
        w[i] = w[i - 16] + s0 + w[i - 7] + s1;
    }

    // Initialize working variables to current hash value:
    uint32_t a[8];
    memcpy(a, h, 32);

    // Compression function main loop:
    for (int i = 0; i < 64; ++i)
    {
        uint32_t S1 = right_rotate_32(a[4], 6) ^ right_rotate_32(a[4], 11) ^ right_rotate_32(a[4], 25);
        uint32_t ch = (a[4] & a[5]) ^ ((~a[4]) & a[6]);
        uint32_t temp1 = a[7] + S1 + ch + k[i] + w[i];
        uint32_t S0 = right_rotate_32(a[0], 2) ^ right_rotate_32(a[0], 13) ^ right_rotate_32(a[0], 22);
        uint32_t maj = (a[0] & a[1]) ^ (a[0] & a[2]) ^ (a[1] & a[2]);
        uint32_t temp2 = S0 + maj;
 
        a[7] = a[6];
        a[6] = a[5];
        a[5] = a[4];
        a[4] = a[3] + temp1;
        a[3] = a[2];
        a[2] = a[1];
        a[1] = a[0];
        a[0] = temp1 + temp2;
    }

    // Add the compressed chunk to the current hash value:
    for (int i = 0; i < 8; ++i)
    {
        h[i] += a[i];
    }
}
