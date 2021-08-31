#include "../ft_ssl.h"

static int pflag = 0, qflag = 0, rflag = 0, sflag = 0;
static char *svalue = NULL;

int rsa(int argc, char *argv[])
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

    return 0;
}
