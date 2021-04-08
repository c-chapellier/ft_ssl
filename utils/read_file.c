#include "../ft_ssl.h"

char    *read_file(const char *filename)
{
    int     fd = open(filename, O_RDONLY);
    int     len;
    char    *content;

    if (fd == -1)
    {
        printf("%s: No such file or directory\n", filename);
        return (NULL);
    }
    len = lseek(fd, 0, SEEK_END);
    content = mmap(0, len, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);
    return (content);
}

void    read_fd(int fd, int n, char buffer[n])
{
    int ret;

    ret = read(fd, buffer, n - 1);
    if (ret == -1)
    {
        printf("read_fd: read return -1\n");
        buffer[0] = 0;
        return ;
    }
    buffer[ret] = '\0';
}

