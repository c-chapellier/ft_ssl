#include "ft_ssl.h"

int hub(int argc, char *argv[], int n, algorithm_t algorithms[n])
{    
    for (int i = 0; i < n; ++i)
    {
        if (strcmp(argv[0], algorithms[i].name) == 0)
        {
            return (algorithms[i].algorithm(argc - 1, &argv[1]));
        }
    }

    printf("ft_ssl: Error: '%s' is an invalid command.\n\n", argv[0]);
    printf("Standard commands:\n\n");
    printf("Message Digest commands:\n");
    for (int i = 0; i < n; ++i)
    {
        printf("%s\n", algorithms[i].name);
    }
    printf("\n");
    printf("Cipher commands:\n");
    return (EXIT_FAILURE);
}

void fill_algorithms(int n, algorithm_t algorithms[n])
{
    algorithms[0].name = "md5";
    algorithms[0].algorithm = &md5;
    algorithms[1].name = "sha256";
    algorithms[1].algorithm = &sha256;
}

int main(int argc, char *argv[])
{
    int         n = 2;
    algorithm_t algorithms[n];

    if (argc == 1)
    {
        printf("usage: ft_ssl command [command opts] [command args]\n");
        return (EXIT_FAILURE);
    }
    fill_algorithms(n, algorithms);
    if (hub(argc - 1, &argv[1], n, algorithms) == EXIT_FAILURE)
    {
        return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}