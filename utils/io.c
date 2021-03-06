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

char *read_fd_n(int fd, int *bytes_read)
{
    char *content = NULL, *tmp;
    char buf[BUF_SIZE];
    int rc;

    *bytes_read = 0;
    do
    {
        rc = read(fd, buf, BUF_SIZE - 1);
        tmp = malloc(*bytes_read + rc);

        memcpy(tmp, content, *bytes_read);
        memcpy(&tmp[*bytes_read], buf, rc);
        *bytes_read += rc;

        free(content);
        content = tmp;

    } while (rc == BUF_SIZE - 1);

    return content;
}

char *read_file(const char *filename)
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

char *read_file_n(const char *filename, int *bytes_read)
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
    content = read_fd_n(fd, bytes_read);
    close(fd);
    return content;
}

int write_file(const char *filename, const char *content)
{
    struct stat path_stat;
    int     fd;

    stat(filename, &path_stat);
    if (S_ISDIR(path_stat.st_mode))
    {
        printf("md5: %s: Is a directory\n", filename);
        return 1;
    }
    
    fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0666);
    if (fd == -1)
    {
        printf("%s: No such file or directory\n", filename);
        return 1;
    }
    write(fd, content, strlen(content));
    close(fd);
    return 0;
}

int write_buffer_to_file(const char *filename, int n, const char content[n])
{
    struct stat path_stat;
    int     fd;

    stat(filename, &path_stat);
    if (S_ISDIR(path_stat.st_mode))
    {
        printf("md5: %s: Is a directory\n", filename);
        return 1;
    }
    
    fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0666);
    if (fd == -1)
    {
        printf("%s: No such file or directory\n", filename);
        return 1;
    }
    write(fd, content, n);
    close(fd);
    return 0;
}