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

static int hash_from_string(uint8_t message[], uint8_t args, algorithm_t algorithm)
{
    uint8_t digest[algorithm.digest_size_in_bytes]; 
    
    if (message == NULL)
    {
        printf("%s: option requires an argument -- s\n", algorithm.name);
        printf("usage: %s [-pqr] [-s string] [files ...]\n", algorithm.name);
        return (EXIT_FAILURE);
    }
    algorithm.hash(message, digest);

    if ((args & ARG_Q) == 0 && (args & ARG_R) == 0)
        printf("%s (\"%s\") = ", algorithm.uppercase_name, (char *)message);
    for (int i = 0; i < algorithm.digest_size_in_bytes; ++i)
        printf("%02x", digest[i]);
    if ((args & ARG_Q) == 0 && args & ARG_R)
        printf(" \"%s\"", (char *)message);
    printf("\n");
    return (EXIT_SUCCESS);
}

static int hash_from_stdin(uint8_t args, algorithm_t algorithm)
{
    uint8_t message[256];
    uint8_t digest[algorithm.digest_size_in_bytes]; 
    
    read_fd(STDIN_FILENO, 256, (char *)message);
    algorithm.hash(message, digest);

    // printf("\n");
    if (args & ARG_P)
        printf("%s", message);
    for (int i = 0; i < algorithm.digest_size_in_bytes; ++i)
        printf("%02x", digest[i]);
    printf("\n");
    return (EXIT_SUCCESS);
}

static int hash_from_files(char *filenames[], uint8_t args, algorithm_t algorithm)
{
    int     failure = 0;
    uint8_t *message;
    uint8_t digest[algorithm.digest_size_in_bytes]; 
    
    for (int i = 0; filenames[i] != NULL; ++i)
    {
        message = ((uint8_t *)read_file(filenames[i]));
        if (message == NULL)
        {
            failure = 1;
            continue ;
        }
        algorithm.hash(message, digest);

        if ((args & ARG_Q) == 0 && (args & ARG_R) == 0)
            printf("%s (%s) = ", algorithm.uppercase_name, filenames[i]);
        for (int i = 0; i < algorithm.digest_size_in_bytes; ++i)
            printf("%02x", digest[i]);
        if ((args & ARG_Q) == 0 && args & ARG_R)
            printf(" %s", filenames[i]);
        printf("\n");
    }
    return (failure ? EXIT_FAILURE : EXIT_SUCCESS);
}

int algorithm_cli(int argc, char *argv[], algorithm_t algorithm)
{
    uint8_t args;
    uint8_t *message = NULL;
    int     first_filename;

    (void)argc;
    args = fill_args(argv, &message, &first_filename);

    if (args & ARG_S)
    {
        if (hash_from_string(message, args, algorithm) == EXIT_FAILURE)
            return (EXIT_FAILURE);
    }
    else if (argv[first_filename] == NULL)
        hash_from_stdin(args, algorithm);
    else
    {
        if (hash_from_files(&argv[first_filename], args, algorithm) == EXIT_FAILURE)
            return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}