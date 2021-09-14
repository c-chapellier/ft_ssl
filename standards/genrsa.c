#include "../ft_ssl.h"

#define COMPOSITE 0
#define PROBABLY_PRIME 1

int     fd;

static int pflag = 0, qflag = 0, rflag = 0, sflag = 0;
static char *svalue = NULL;

static uint64_t modpow(__uint128_t base, uint64_t exp, uint64_t modulus);
static uint64_t randu64();
static uint64_t randu64_range(uint64_t min, uint64_t max);
static int miller_rabin_primalty_test(uint64_t n, int k);
static uint64_t generate_prime_candidate();
static uint64_t generate_prime_number();

static uint8_t *get_res(uint64_t nb1, uint64_t nb2);

int genrsa(int argc, char *argv[])
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
                fprintf(stderr, "md5: option -%c requires an argument.\n", optopt);
            else
                fprintf(stderr, "Unknown option `-%c'.\n", optopt);
            return 1;
        default:
            fprintf(stderr, "usage: md5 [-pqr] [-s string] [files ...]\n");
            return 1;
        }
    }
    fd = open("/dev/random", O_RDONLY);
    
    uint64_t nb1 = generate_prime_number();
    uint64_t nb2 = generate_prime_number();
    printf("prime = %llu %llx\nprime = %llu %llx\n", nb1, nb1, nb2, nb2);
    close(fd);

    get_res(nb1, nb2);
    return 0;
}

static uint64_t generate_prime_number()
{
    uint64_t p = 4;

    while (miller_rabin_primalty_test(p, 128) != PROBABLY_PRIME)
        p = generate_prime_candidate();
    return p;
}

static uint64_t generate_prime_candidate()
{
    return randu64() | ((uint64_t)1 << (64 - 1)) | 1;
}

static int miller_rabin_primalty_test(uint64_t n, int k)
{
    if (n == 2 || n == 3)
        return PROBABLY_PRIME;
    if (n <= 1 || n % 2 == 0)
        return COMPOSITE;

    uint64_t x, s = 0, r = n - 1;
    int j;

    while ((r & 1) == 0)
        s += 1, r /= 2;
    
    for (int i = 0; i < k; ++i)
    {
        x = modpow(randu64_range(2, n - 1), r, n);

        if (x != 1 && x != n - 1)
        {
            j = 1;
            while (j < s && x != n - 1)
            {
                x = modpow(x, 2, n);
                if (x == 1)
                    return COMPOSITE;
                ++j;
            }
            if (x != n - 1)
                return COMPOSITE;
        }
    }
    return PROBABLY_PRIME;
}

static uint64_t randu64()
{
    int     size = 8;
    char    buff[size];
    read(fd, buff, 8);
    uint64_t *res = (uint64_t*)buff;
    printf("%d: %llu\n", fd, *res);
    return *res;
    //return (uint64_t)random() << 32 | random();
}

static uint64_t randu64_range(uint64_t min, uint64_t max)
{
    return min + randu64() % (max - 1 - min);
}

static uint64_t modpow(__uint128_t base, uint64_t exp, uint64_t modulus)
{
    __uint128_t result = 1;

    base %= modulus;
    while (exp > 0)
    {
        if (exp & 1)
            result = (result * base) % modulus;
        assert(base < (base * base));
        base = (base * base) % modulus;
        exp >>= 1;
    }
    return result;
}

static uint8_t *get_res(uint64_t nb1, uint64_t nb2)
{
    uint32_t new_size = 0;
    uint8_t nb1t[8];
    uint8_t nb2t[8];
    memcpy(nb1t, &nb1, 8);
    memcpy(nb2t, &nb2, 8);

    for (int i = 0; i < 4; ++i) {
        uint8_t tmp = nb1t[i];
        nb1t[i] = nb1t[8 - 1 - i];
        nb1t[8 - 1 - i] = tmp;

        tmp = nb2t[i];
        nb2t[i] = nb2t[8 - 1 - i];
        nb2t[8 - 1 - i] = tmp;
    }

    uint8_t *res = multiplication(nb1t, nb2t, 8, &new_size);
    double_dabble(res, new_size, &new_size);
    return res;
}