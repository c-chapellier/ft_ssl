#include "ft_ssl.h"

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

    int n_hashes = sizeof hashes / sizeof hashes[0];
    int n_ciphers = sizeof ciphers / sizeof ciphers[0];

    for (int i = 0; i < n_hashes + n_ciphers; ++i)
        if (strcmp(argv[1], i < n_hashes ? hashes[i] : ciphers[i - n_hashes]) == 0)
            return mains[i](argc - 1, &argv[1]);

    printf("ft_ssl: Error: '%s' is an invalid command.\n\n", argv[1]);
    printf("Standard commands:\n\n");
    printf("Message Digest commands:\n");
    for (int i = 0; i < n_hashes; ++i)
        printf("%s\n", hashes[i]);
    printf("\n");
    printf("Cipher commands:\n");
    for (int i = 0; i < n_ciphers; ++i)
        printf("%s\n", ciphers[i]);
    return (EXIT_FAILURE);
}