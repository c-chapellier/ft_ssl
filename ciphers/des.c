#include "../ft_ssl.h"

static int key_permutation_table[56] = {
    57, 49, 41, 33, 25, 17,  9,
     1, 58, 50, 42, 34, 26, 18,
    10,  2, 59, 51, 43, 35, 27,
    19, 11,  3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
     7, 62, 54, 46, 38, 30, 22,
    14,  6, 61, 53, 45, 37, 29,
    21, 13,  5, 28, 20, 12,  4
};

static int key_permutation_table2[48] = {
    14, 17, 11, 24,  1,  5,
     3, 28, 15,  6, 21, 10,
    23, 19, 12,  4, 26,  8,
    16,  7, 27, 20, 13,  2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32
};

static int circular_rotate_permutation_table[16] = {
    1, 1, 2, 2,
    2, 2, 2, 2,
    1, 2, 2, 2,
    2, 2, 2, 1
};

static int initial_block_permutation_table[64] = {
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17,  9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7
};

static int e_permutation_table[64] = {
    32,  1,  2,  3,  4,  5,
     4,  5,  6,  7,  8,  9,
     8,  9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32,  1
};

static int final_block_permutation_table[64] = {
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41,  9, 49, 17, 57, 25
};

static int s_box_permutation_table[32] = {
    16,   7,  20,  21,
    29,  12,  28,  17,
     1,  15,  23,  26,
     5,  18,  31,  10,
     2,   8,  24,  14,
    32,  27,   3,   9,
    19,  13,  30,   6,
    22,  11,   4,  25,
};

static int s_box_table[8][4][16] = { 
    { 
        { 14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7 },
        { 0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8 },
        { 4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0 },
        { 15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13 }
    },
    { 
        { 15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10 },
        { 3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5 },
        { 0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15 },
        { 13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9 }
    },
    { 
        { 10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8 },
        { 13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1 },
        { 13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7 },
        { 1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12 }
    },
    { 
        { 7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15 },
        { 13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9 },
        { 10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4 },
        { 3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14 }
    },
    { 
        { 2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9 },
        { 14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6 },
        { 4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14 },
        { 11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3 }
    },
    { 
        { 12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11 },
        { 10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8 },
        { 9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6 },
        { 4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13 }
    },
    { 
        { 4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1 },
        { 13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6 },
        { 1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2 },
        { 6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12 }
    },
    { 
        { 13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7 },
        { 1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2 },
        { 7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8 },
        { 2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11 }
    }
};

static int is_cbc = 1;
static uint64_t initialisation_vector = 0;
static int aflag = 0, dflag = 0, eflag = 1, iflag = 0, kflag = 0;
static int oflag = 0, pflag = 0, sflag = 0, vflag = 0;
static char *ivalue = NULL, *kvalue = NULL, *ovalue = NULL;
static char *pvalue = NULL, *svalue = NULL, *vvalue = NULL;

static char *pad_msg(char *msg, int *msglen);
static void des_algorithm(int n, const char msg[n], uint64_t key, uint8_t e[n]);
static void permute(int n, uint8_t block[n], int m, uint8_t permutated_block[m], int permutation_table[m]);
static void rotate_key(uint8_t key[7], int n);
static uint32_t s_box(uint8_t block[6]);

int des_ecb(int argc, char *argv[])
{
    is_cbc = 0;
    return des(argc, argv);
}

static char *get_password()
{
    char *password = strdup(getpass("Enter desencryption password:"));
    char *confirmed_password = strdup(getpass("Verifying - enter desencryption password:"));

    if (strcmp(password, confirmed_password) != 0)
    {
        fprintf(stderr, "Verify failure\nbad password read\n");
        exit(EXIT_FAILURE);
    }
    free(confirmed_password);
    return password;
}

static uint64_t verify_hex(const char *s)
{
    uint64_t ret;
    uint8_t *ret_p = (uint8_t *)&ret, tmp;
    char *endptr;

    if (strlen(s) < 16)
        fprintf(stderr, "hex string is too short, padding with zero bytes to length\n");
    if (strlen(s) > 16)
        fprintf(stderr, "hex string is too long, ignoring excess\n");

    errno = 0;
    ret = strtoul(s, &endptr, 16);
    if (endptr == s)
    {
        fprintf(stderr, "non-hex digit\ninvalid hex value\n");
        exit(1);
    }
    for (int i = 0; i < 4; ++i)
    {
        tmp = ret_p[i];
        ret_p[i] = ret_p[7 - i];
        ret_p[7 - i] = tmp;
    }
    return ret;
}

static uint64_t generate_key(char *password, uint64_t salt)
{
    uint64_t key;
    char *tmp;
    int n = strlen(password);

    while (n < 8 && n % 8 != 0)
    {
        tmp = calloc(2*n + 1, 1);
        memcpy(tmp, password, n);
        memcpy(&tmp[n], password, n);
        free(password);
        password = tmp;
    }

    memcpy(&key, password, 8);
    for (int i = 1; i < n / 8; ++i)
        key ^= *((uint64_t *)&password[i * 8]);

    return key ^ salt;
}

int des(int argc, char *argv[])
{
    char *msg, *e, *password = NULL;
    uint64_t salt, key;
    int c, n, base64_n;

    opterr = 0;
    while ((c = getopt(argc, argv, "adei:k:o:p:s:v:")) != EOF)
    {
        switch (c)
        {
        case 'a':
            aflag = 1;
            break;
        case 'd':
            dflag = 1;
            break;
        case 'e':
            eflag = 1;
            break;
        case 'i':
            iflag = 1;
            ivalue = optarg;
            break;
        case 'k':
            kflag = 1;
            kvalue = optarg;
            break;
        case 'o':
            oflag = 1;
            ovalue = optarg;
            break;
        case 'p':
            pflag = 1;
            pvalue = optarg;
            break;
        case 's':
            sflag = 1;
            svalue = optarg;
            break;
        case 'v':
            vflag = 1;
            vvalue = optarg;
            break;
        case '?':
            switch (optopt)
            {
            case 'i':
            case 'k':
            case 'o':
            case 'p':
            case 's':
            case 'v':
                fprintf(stderr, "md5: option -%c requires an argument.\n", optopt);
                break;
            default:
                fprintf(stderr, "Unknown option `-%c'.\n", optopt);
            }
            return 1;
        default:
            fprintf(stderr, "usage: base64 [-ade] [-i input_file] [-k hex_key] [-o output_file] [-p password] [-s hex_salt] [-v hex_init_vector] \n");
            return 1;
        }
    }

    assert(!is_cbc || vflag);

    msg = iflag ? read_file_n(ivalue, &n) : read_fd_n(STDIN_FILENO, &n);

    if (!dflag)
        msg = pad_msg(msg, &n);
    base64_n = (n*4 / 3) + 2;

    if (!kflag)
    {
        password = pflag ? strdup(pvalue) : get_password();
        salt = sflag ? verify_hex(svalue) : random() << 32 | random(); 
    }
    key = kflag ? verify_hex(kvalue) : generate_key(password, salt);

    printf("key = %llx\n", key);

    if (vflag)
        initialisation_vector = verify_hex(vvalue);

    e = malloc(base64_n);
    memset(e, 0, base64_n);

    if (aflag && dflag)
    {
        n = base64_decrypt(msg, msg);
        n -= n % 8;
    }

    des_algorithm(n, msg, key, (uint8_t *)e);

    if (dflag)
    {
        if (e[n - 1] < 1 || e[n - 1] > 8)
        {
            printf("bad padding [%c] %d\n", e[n - 1], e[n - 1]);
            exit(1);
        }
        n -= e[n - 1];
    }

    if (aflag && !dflag)
        n = base64_encrypt(n, e, e);

    if (aflag && !dflag && oflag)
        write_file(ovalue, e);
    else if (oflag)
        write_buffer_to_file(ovalue, n, e);

    if (aflag && !dflag && !oflag)
    {
        for (int i = 0; e[i] != '\0'; ++i % 64 || printf("\n"))
            printf("%c", e[i]);
        printf("\n");
    }
    else if (!oflag)
        for (int i = 0; i < n; ++i)
            printf("%c", e[i]);

    // for (int i = 0; i < n / 8; printf("\n"), ++i)
    //     for (int j = 0; j < 8; ++j)
    //         printf("%.2x", e[8*i + j]);

    free(msg), free(e), free(password);
    return 0;
}

static char *pad_msg(char *msg, int *msglen)
{
    char *padded;
    int padding;

    padding = 8 - *msglen%8;
    padded = malloc(*msglen + padding + 1);

    memcpy(padded, msg, *msglen);
    memset(&padded[*msglen], padding, padding);
    padded[*msglen + padding] = 0;

    free(msg);
    *msglen += padding;
    return padded;
}

static void des_algorithm(int n, const char msg[n], uint64_t key, uint8_t e[n])
{
    assert(n % 8 == 0);

    uint8_t rotated_keys[16][7];
    uint8_t final_keys[16][6] = {0};
    uint8_t s_box_input[6];
    uint32_t left[17], right[17], s_box_output;
    uint64_t *xored;

    permute(8, (uint8_t *)&key, 7, rotated_keys[0], key_permutation_table);
    for (int i = 0; i < 16; ++i)
    {
        i && memcpy(rotated_keys[i], rotated_keys[i - 1], 7);
        rotate_key(rotated_keys[i], circular_rotate_permutation_table[i]);
    }

    for (int i = 0; i < 16; ++i)
        permute(7, rotated_keys[i], 6, final_keys[i], key_permutation_table2);

    for (int i = 0; i < n / 8; ++i)
    {
        xored = (uint64_t *)&msg[8 * i];
        if (is_cbc && !dflag)
            *xored ^= i ? *((uint64_t *)&e[8 * (i - 1)]) : initialisation_vector;
        
        permute(8, (uint8_t *)xored, 8, &e[8 * i], initial_block_permutation_table);

        memcpy(&left[0], &e[8 * i], 4);
        memcpy(&right[0], &e[8*i + 4], 4);
        for (int j = 0; j < 16; ++j)
        {
            j && memcpy(&left[j], &right[j - 1], 4);

            permute(4, (uint8_t *)&right[j], 6, s_box_input, e_permutation_table);

            for (int k = 0; k < 6; ++k)
                s_box_input[k] ^= final_keys[dflag ? 15 - j : j][k];

            s_box_output = s_box(s_box_input);
            permute(4, (uint8_t *)&s_box_output, 4, (uint8_t *)&s_box_output, s_box_permutation_table);
            right[j + 1] = left[j] ^ s_box_output;
        }
        left[16] = right[15];

        memcpy(&e[8 * i], &right[16], 4);
        memcpy(&e[8*i + 4], &left[16], 4);
        permute(8, &e[8 * i], 8, &e[8 * i], final_block_permutation_table);

        xored = (uint64_t *)&e[8 * i];
        if (is_cbc && dflag)
            *xored ^= i ? *((uint64_t *)&msg[8 * (i - 1)]) : initialisation_vector;
    }
}

static void permute(int n, uint8_t block[n], int m, uint8_t permutated_block[m], int permutation_table[m * 8])
{
    uint8_t tmp[m];

    memset(tmp, 0, m);
    for (int i = 0; i < m; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            int shift = permutation_table[8*i + j] - 1;
            int bit = (block[shift / 8] >> (7 - shift%8)) & 1;
            tmp[i] |= bit << (7 - j);
        }
    }
    memcpy(permutated_block, tmp, m);
}

static void rotate_key(uint8_t key[7], int n)
{
    uint8_t lkey[4], rkey[4];
    uint8_t tmp;

    memcpy(lkey, key, 4);
    memcpy(rkey, &key[3], 4);

    lkey[3] = ((uint8_t)(lkey[3] >> 4) << 4);
    rkey[0] = ((uint8_t)(rkey[0] << 4) >> 4);

    tmp = lkey[0];
    lkey[0] = (uint8_t)(lkey[0] << n) | (uint8_t)(lkey[1] >> (8 - n));
    lkey[1] = (uint8_t)(lkey[1] << n) | (uint8_t)(lkey[2] >> (8 - n));
    lkey[2] = (uint8_t)(lkey[2] << n) | (uint8_t)(lkey[3] >> (8 - n));
    lkey[3] = (uint8_t)(lkey[3] << n) | (uint8_t)((uint8_t)(tmp >> (8 - n)) << 4);

    tmp = rkey[0];
    rkey[0] = (uint8_t)(((uint8_t)(rkey[0] << n) | (uint8_t)(rkey[1] >> (8 - n))) << 4) >> 4;
    rkey[1] = (uint8_t)(rkey[1] << n) | (uint8_t)(rkey[2] >> (8 - n));
    rkey[2] = (uint8_t)(rkey[2] << n) | (uint8_t)(rkey[3] >> (8 - n));
    rkey[3] = (uint8_t)(rkey[3] << n) | (uint8_t)(tmp >> (4 - n));

    memcpy(key, lkey, 3);
    key[3] = lkey[3] | rkey[0];
    memcpy(&key[4], &rkey[1], 3);
}

static void combine(uint8_t *res, uint8_t working, int table[4][16])
{
    uint8_t x;
    uint8_t y;

    x = (working >> 1) & 15;
    y = (working & 33) == 0 ? 0 : (working & 33) == 1 ? 1 : (working & 33) == 32 ? 2 : 3;
    *res = table[y][x];
}

static uint32_t s_box(uint8_t block[6])
{
    uint32_t res;
    uint8_t tmp[8], *tmp2 =(uint8_t *)&res;
    uint8_t working;

    working = (block[0] >> 2);
    combine(&tmp[0], working, s_box_table[0]);

    working = ((block[0] << 6) >> 2) | (block[1] >> 4);
    combine(&tmp[1], working, s_box_table[1]);

    working = ((block[1] << 4) >> 2) | (block[2] >> 6);
    combine(&tmp[2], working, s_box_table[2]);

    working = ((block[2] << 2) >> 2);
    combine(&tmp[3], working, s_box_table[3]);

    working = (block[3] >> 2);
    combine(&tmp[4], working, s_box_table[4]);

    working = ((block[3] << 6) >> 2) | (block[4] >> 4);
    combine(&tmp[5], working, s_box_table[5]);

    working = ((block[4] << 4) >> 2) | (block[5] >> 6);
    combine(&tmp[6], working, s_box_table[6]);

    working = ((block[5] << 2) >> 2);
    combine(&tmp[7], working, s_box_table[7]);

    tmp2[0] = (tmp[0] << 4) | tmp[1];
    tmp2[1] = (tmp[2] << 4) | tmp[3];
    tmp2[2] = (tmp[4] << 4) | tmp[5];
    tmp2[3] = (tmp[6] << 4) | tmp[7];

    return res;
}
