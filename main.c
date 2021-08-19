#include "ft_ssl.h"

char *names[] = {
    "md5",
    "sha256",
};

int (*mains[])(int argc, char *argv[]) = {
    &md5,
    &sha256,
};

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        printf("usage: ft_ssl command [command opts] [command args]\n");
        return (EXIT_FAILURE);
    }

    int n = sizeof names / sizeof names[0];

    for (int i = 0; i < n; ++i)
    {
        if (strcmp(argv[1], names[i]) == 0)
        {
            return mains[i](argc - 1, &argv[1]);
        }
    }

    printf("ft_ssl: Error: '%s' is an invalid command.\n\n", argv[1]);
    printf("Standard commands:\n\n");
    printf("Message Digest commands:\n");
    for (int i = 0; i < n; ++i)
    {
        printf("%s\n", names[i]);
    }
    printf("\n");
    printf("Cipher commands:\n");
    return (EXIT_FAILURE);
}