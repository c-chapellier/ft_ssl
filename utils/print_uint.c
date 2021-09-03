#include "../ft_ssl.h"

static int powint(int b, int e)
{
    int ret = b;
    if (e == 0)
    {
        return (1);
    }
    for (int i = 0; i < e - 1; ++i)
    {
        ret *= b;
    }
    return (ret);
}

void print_uint8(uint8_t *n)
{
    int t = *n;
    for (int i = 0; i < 8; ++i)
    {
        // printf("n = %u [%u]\n", n, pow(2, 7 - i));
        if (t / powint(2, 7 - i) >= 1) // power for integers -> https://stackoverflow.com/questions/29787310/does-pow-work-for-int-data-type-in-c
        {
            putchar('1');
            t -= powint(2, 7 - i);
        }
        else
        {
            putchar('0');
        }
    }
}
