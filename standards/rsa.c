#include "../ft_ssl.h"

static char *inform = NULL, *outform = NULL, *in = NULL, *out = NULL;
static char *passin = NULL, *passout = NULL;
static int p_des = 0, p_text = 0, p_noout = 0, p_modulus = 0;
static int p_check = 0, p_pubin = 0, p_pubout = 0;

// static char *public_key_start = "-----BEGIN PUBLIC KEY-----";
// static char *public_key_end = "-----END PUBLIC KEY-----";
// static char *private_key_start = "-----BEGIN RSA PRIVATE KEY-----";
// static char *private_key_end = "-----END RSA PRIVATE KEY-----";

static struct option long_options[] =
{
    /* These options set a flag. */
    {"inform",  required_argument,  0, 'a'}, // DER | NET | PEM
    {"outform", required_argument,  0, 'b'}, // DER | NET | PEM
    {"in",      required_argument,  0, 'c'}, // file name
    {"passin",  required_argument,  0, 'd'}, // param
    {"out",     required_argument,  0, 'e'}, // file name
    {"passout", required_argument,  0, 'f'}, // password
    {"des",     no_argument,        0, 'h'}, // ???
    {"text",    no_argument,        0, 'i'}, // output components of the key in text format
    {"noout",   no_argument,        0, 'j'}, // do not output the key
    {"modulus", no_argument,        0, 'k'}, // output the modulo value of the key
    {"check",   no_argument,        0, 'l'}, // check the coherence of the key
    {"pubin",   no_argument,        0, 'm'}, // take a public key in input
    {"pubout",  no_argument,        0, 'n'}, // generate and output a public key
    {0, 0, 0, 0}
};

static void print_opt();
static void print_usage();
static void check_error();

int rsa(int argc, char *argv[])
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
            inform = optarg;
            break;
        case 'b':
            outform = optarg;
            break;
        case 'c':
            in = optarg;
            break;
        case 'd':
            passin = optarg;
            break;
        case 'e':
            out = optarg;
            break;
        case 'f':
            passout = optarg;
            break;
        case 'h':
            p_des = 1;
            break;
        case 'i':
            p_text = 1;
            break;
        case 'j':
            p_noout = 1;
            break;
        case 'k':
            p_modulus = 1;
            break;
        case 'l':
            p_check = 1;
            break;
        case 'm':
            p_pubin = 1;
            break;
        case 'n':
            p_pubout = 1;
            break;
        case '?':
            if (optopt == 's')
                fprintf(stderr, "rsa: option -%c requires an argument.\n", optopt);
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
    check_error();

    uint8_t nb1[4] = {0, 0, 0, 255};
    uint8_t nb2[4] = {0, 0, 0, 255};
    adition(nb1, nb2, 4);
    return 0;
}

static void print_opt(){
    // printf("inform  : %s\n", inform);
    // printf("outform : %s\n", outform);
    // printf("in      : %s\n", in);
    // printf("passin  : %s\n", passin);
    // printf("out     : %s\n", out);
    // printf("passout : %s\n", passout);
    // printf("des     : %d\n", p_des);
    // printf("text    : %d\n", p_text);
    // printf("noout   : %d\n", p_noout);
    // printf("modulus : %d\n", p_modulus);
    // printf("check   : %d\n", p_check);
    // printf("pubin   : %d\n", p_pubin);
    // printf("pubout  : %d\n", p_pubout);
}

static void print_usage(){
    fprintf(stderr, "usage: rsa\n");
    fprintf(stderr, "\t[-inform <DER | NET | PEM>] [-outform <DER | NET | PEM>]\n");
    fprintf(stderr, "\t[-in <file>] [-passin <arg>] [-out <file>] [-passout <arg>]\n");
    fprintf(stderr, "\t[-des] [-text] [-noout] [-modulus] [-check] [-pubin] [-pubout]\n");
}

static void check_error() {
    if(inform && strcmp(inform, "DER") != 0 && strcmp(inform, "NET") != 0 && strcmp(inform, "PEM") != 0) {
        fprintf(stderr, "rsa: Invalid format \"%s\" for -inform\n", inform);
        exit(EXIT_FAILURE);
    }
    if(outform && strcmp(outform, "DER") != 0 && strcmp(outform, "NET") != 0 && strcmp(outform, "PEM") != 0) {
        fprintf(stderr, "rsa: Invalid format \"%s\" for -outform\n", inform);
        exit(EXIT_FAILURE);
    }
}