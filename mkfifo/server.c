#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main()
{
    int fd = open("./myfifo", O_RDONLY);
    if(fd < 0)
    {
        perror("open");
        return 1;
    }

    char buf[1024];
    memset(buf, '\0', sizeof(buf));

    while(1)
    {                          
        ssize_t size = read(fd, buf, sizeof(buf));
        if(size > 0)
        {
            printf("client# %s\n", buf);
        }

        if(strncmp(buf, "quit", 4) == 0)
        {
            break;
        }
    }

    close(fd);
    return 0;
}
