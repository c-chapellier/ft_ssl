#include "ft_ssl.h"

char *standards[] = {
    "genrsa",
    "rsa",
    "rsautl",
};

char *hashes[] = {
    "md5",
    "sha256",
};

char *ciphers[] = {
    "base64",
    "des",
    "des-ecb",
    "des-cbc",
};

int (*mains[])(int argc, char *argv[]) = {
    &genrsa,
    &rsa,
    &rsautl,
    &md5,
    &sha256,
    &base64,
    &des,
    &des_ecb,
    &des,
};

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        printf("usage: ft_ssl command [command opts] [command args]\n");
        return (EXIT_FAILURE);
    }

    int n_standards = sizeof standards / sizeof standards[0];
    int n_hashes = sizeof hashes / sizeof hashes[0];
    int n_ciphers = sizeof ciphers / sizeof ciphers[0];

    for (int i = 0; i < n_standards + n_hashes + n_ciphers; ++i)
    {
        if (i < n_standards && strcmp(argv[1], standards[i]) == 0)
            return mains[i](argc - 1, &argv[1]);
        if (i < n_hashes && strcmp(argv[1], hashes[i]) == 0)
            return mains[n_standards + i](argc - 1, &argv[1]);
        if (i < n_ciphers && strcmp(argv[1], ciphers[i]) == 0)
            return mains[n_standards + n_hashes + i](argc - 1, &argv[1]);
    }

    printf("ft_ssl: Error: '%s' is an invalid command.\n\n", argv[1]);
    printf("Standard commands:\n");
    for (int i = 0; i < n_standards; ++i)
        printf("%s\n", standards[i]);
    printf("\n");
    printf("Message Digest commands:\n");
    for (int i = 0; i < n_hashes; ++i)
        printf("%s\n", hashes[i]);
    printf("\n");
    printf("Cipher commands:\n");
    for (int i = 0; i < n_ciphers; ++i)
        printf("%s\n", ciphers[i]);
    return (EXIT_FAILURE);
}