#include "../ft_ssl.h"

#define BUF_SIZE 10000

static char *strjoin(char *s1, const char *s2)
{
    if (!s1)
        return strdup(s2);

    char *s = malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(s, s1);
    strcat(s, s2);
    return s;
}

char *read_fd(int fd)
{
    char *content = NULL;
    char buf[BUF_SIZE];
    int rc;

    do
    {
        rc = read(fd, buf, BUF_SIZE - 1);
        buf[rc] = '\0';
        content = strjoin(content, buf);

    } while (rc == BUF_SIZE - 1);
    
    return content;
}

char    *read_file(const char *filename)
{
    struct stat path_stat;
    char    *content;
    int     fd;

    stat(filename, &path_stat);
    if (S_ISDIR(path_stat.st_mode))
    {
        printf("md5: %s: Is a directory\n", filename);
        return NULL;
    }
    
    fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        printf("%s: No such file or directory\n", filename);
        return NULL;
    }
    content = read_fd(fd);
    close(fd);
    return content;
}
