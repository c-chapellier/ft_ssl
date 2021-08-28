#include "../ft_ssl.h"

static char *table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static char padding = '=';

static int dflag = 0, eflag = 1, iflag = 0, oflag = 0;
static char *ivalue = NULL, *ovalue = NULL;

int base64_encrypt(int n, char msg[n], char e[]);
int base64_decrypt(char *msg, char *e);
static char invert_table(char c);

int base64(int argc, char *argv[])
{
    char *msg, *e;
    int c, n;

    opterr = 0;
    while ((c = getopt(argc, argv, "dei:o:")) != EOF)
    {
        switch (c)
        {
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
        case 'o':
            oflag = 1;
            ovalue = optarg;
            break;
        case '?':
            if (optopt == 'i' || optopt == 'o')
                fprintf(stderr, "md5: option -%c requires an argument.\n", optopt);
            else
                fprintf(stderr, "Unknown option `-%c'.\n", optopt);
            return 1;
        default:
            fprintf(stderr, "usage: base64 [-de] [-i input_file] [-o output_file]\n");
            return 1;
        }
    }

    msg = iflag ? read_file_n(ivalue, &n) : read_fd_n(STDIN_FILENO, &n);

    if (dflag)
    {
        if ((n-1) % 4 != 0)
        {
            printf("not valid\n");
            exit(1);
        }
        e = malloc((n*4 / 3) + 2);
        base64_decrypt(msg, e);
    }
    else
    {
        e = malloc((n*4 / 3) + 2);
        base64_encrypt(n, msg, e);
    }

    !dflag && sprintf(e, "%s\n", e);
    oflag ? write_file(ovalue, e) : printf("%s", e);

    free(msg), free(e);
    return 0;
}

int base64_encrypt(int n, char msg[n], char e[])
{
    char *overlap_free = malloc((strlen(msg)*4 / 3) + 2);

    for (int i = 0; i <= (n-1) / 3; ++i)
    {
        overlap_free[4*i] = table[(uint8_t)msg[3*i] / 4];
        overlap_free[4*i + 1] = table[((uint8_t)msg[3*i] % 4)*16 + (uint8_t)msg[3*i + 1]/16];
        overlap_free[4*i + 2] = table[((uint8_t)msg[3*i + 1] % 16)*4 + (uint8_t)msg[3*i + 2]/64];
        overlap_free[4*i + 3] = table[(uint8_t)msg[3*i + 2] % 64];
    }

    if (n % 3 == 1)
        overlap_free[4*((n-1)/3) + 2] = padding;
    if (n % 3 == 1 || n % 3 == 2)
        overlap_free[4*((n-1)/3) + 3] = padding;
    overlap_free[4*((n-1)/3) + 4] = '\0';

    memcpy(e, overlap_free, 4*((n-1)/3) + 5);
    return 4*((n-1)/3) + 4;
}

int base64_decrypt(char *msg, char *e)
{
    int n = strlen(msg);

    // assert(n % 4 == 0);

    for (int i = 0; i < n / 4; ++i)
    {
        e[3*i] = invert_table(msg[4*i])*4 + invert_table(msg[4*i + 1])/16;
        e[3*i + 1] = (invert_table(msg[4*i + 1])%16)*16 + invert_table(msg[4*i + 2])/4;
        e[3*i + 2] = (invert_table(msg[4*i + 2])%4)*64 + invert_table(msg[4*i + 3]);
    }
    return 3*n / 4;
}

static char invert_table(char c)
{
    if (c == padding)
        return '\0';
    for (int i = 0; table[i] != '\0'; ++i)
    {
        if (table[i] == c)
            return i;
    }
    printf("Invalid character in input stream.\n");
    exit(1);
}
