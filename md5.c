#include "ft_ssl.h"

static void fill_args_d(int n, args_t args[n])
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

static int fill_args(char *argv[], uint8_t *message[], int *first_filename)
{
    int     n = 4;
    args_t  args_d[n];
    uint8_t args = 0;

    *first_filename = 0;
    fill_args_d(n, args_d);
    for (int i = 0; argv[i] != NULL; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            if (strcmp(argv[i], args_d[j].name) == 0)
            {
                *first_filename = i + 1;
                args |= args_d[j].value;
                if (args_d[j].value == ARG_S)
                {
                    *message = (uint8_t *)argv[i + 1];
                }
            }
        }
    }
    return (args);
}

static int md5_hash_from_string(uint8_t message[], uint8_t args)
{
    uint8_t digest[16]; 
    
    if (message == NULL)
    {
        printf("md5: option requires an argument -- s\n");
        printf("usage: md5 [-pqr] [-s string] [files ...]\n");
        return (EXIT_FAILURE);
    }
    md5_hash(message, digest);

    if ((args & ARG_Q) == 0 && (args & ARG_R) == 0)
        printf("MD5 (\"%s\") = ", (char *)message);
    for (int i = 0; i < 16; ++i)
        printf("%02x", digest[i]);
    if ((args & ARG_Q) == 0 && args & ARG_R)
        printf(" \"%s\"", (char *)message);
    printf("\n");
    return (EXIT_SUCCESS);
}

static int md5_hash_from_stdin()
{
    uint8_t message[256];
    uint8_t digest[16]; 
    
    read_fd(STDIN_FILENO, 256, (char *)message);
    md5_hash(message, digest);

    printf("\n");
    for (int i = 0; i < 16; ++i)
        printf("%02x", digest[i]);
    printf("\n");
    return (EXIT_SUCCESS);
}

static int md5_hash_from_files(char *filenames[], uint8_t args)
{
    int     failure = 0;
    uint8_t *message;
    uint8_t digest[16]; 
    
    for (int i = 0; filenames[i] != NULL; ++i)
    {
        message = ((uint8_t *)read_file(filenames[i]));
        if (message == NULL)
        {
            failure = 1;
            continue ;
        }
        md5_hash(message, digest);

        if ((args & ARG_Q) == 0 && (args & ARG_R) == 0)
            printf("MD5 (%s) = ", filenames[i]);
        for (int i = 0; i < 16; ++i)
            printf("%02x", digest[i]);
        if ((args & ARG_Q) == 0 && args & ARG_R)
            printf(" %s", filenames[i]);
        printf("\n");
    }
    return (failure ? EXIT_FAILURE : EXIT_SUCCESS);
}

int md5(int argc, char *argv[])
{
    uint8_t args;
    uint8_t *message = NULL;
    int     first_filename;

    args = fill_args(argv, &message, &first_filename);

    if (args & ARG_S)
    {
        if (md5_hash_from_string(message, args) == EXIT_FAILURE)
            return (EXIT_FAILURE);
    }
    else if (argc == 0)
        md5_hash_from_stdin();
    else
    {
        if (md5_hash_from_files(&argv[first_filename], args) == EXIT_FAILURE)
            return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}