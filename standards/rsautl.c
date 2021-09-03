#include "../ft_ssl.h"

static char *in = NULL, *out = NULL, *passin = NULL;
static int p_pubin = 0, p_encrypt = 0, p_decrypt = 0, p_hexdump = 0;

// static char *public_key_start = "-----BEGIN PUBLIC KEY-----";
// static char *public_key_end = "-----END PUBLIC KEY-----";
// static char *private_key_start = "-----BEGIN RSA PRIVATE KEY-----";
// static char *private_key_end = "-----END RSA PRIVATE KEY-----";

static struct option long_options[] =
{
    {"in",      required_argument,  0, 'a'}, // file name
    {"out",     required_argument,  0, 'b'}, // file name
    {"inkey",   required_argument,  0, 'c'}, // 
    {"pubin",   no_argument,        0, 'd'}, // 
    {"encrypt", no_argument,        0, 'e'}, // 
    {"decrypt", no_argument,        0, 'f'}, // 
    {"hexdump", no_argument,        0, 'g'}, // 
    {0, 0, 0, 0}
};

static void print_opt();
static void print_usage();

int rsautl(int argc, char *argv[])
{
    int c;
    int option_index = 0;
    while ((c = getopt_long_only(argc, argv, "", long_options, &option_index)) != EOF)
    {
        switch (c)
        {
        case 0:
            break;
        case 'a':
            in = optarg;
            break;
        case 'b':
            out = optarg;
            break;
        case 'c':
            passin = optarg;
            break;
        case 'd':
            p_pubin = 1;
            break;
        case 'e':
            p_encrypt = 1;
            break;
        case 'f':
            p_decrypt = 1;
            break;
        case 'g':
            p_hexdump = 1;
            break;
        case '?':
            if (optopt == 's')
                fprintf(stderr, "rsautl: option -%c requires an argument.\n", optopt);
            else
                fprintf(stderr, "Unknown option `-%c'.\n", optopt);
            print_usage();
            return 1;
        default:
            print_usage();
            return 1;
        }
    }

    print_opt();

    return 0;
}

static void print_opt(){
    printf("in      : %s\n", in);
    printf("passin  : %s\n", passin);
    printf("out     : %s\n", out);
    printf("pubin   : %d\n", p_pubin);
    printf("encrypt : %d\n", p_encrypt);
    printf("decrypt : %d\n", p_decrypt);
    printf("hexdump : %d\n", p_hexdump);
}

static void print_usage(){
    fprintf(stderr, "usage: rsautl\n");
    fprintf(stderr, "\t[-in file] [-out file] [-inkey file]\n");
    fprintf(stderr, "\t[-pubin] [-encrypt] [-decrypt] [-hexdump]\n");
}
