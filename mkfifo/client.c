#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main()
{
    // success ==0    error:-1
    if(mkfifo("./myfifo", 0777) < 0)
    {
        perror("myfifo\n");
        return 1;
    }

    printf("AAAAAAAAAAAAAAAAAAAAAAAA\n");
    // success:fd   error:-1
    int fd = open("./myfifo", O_WRONLY);
    if(fd < 0)
    {
        perror("open");
        return 2;
    }
    
    char buf[1024];
    memset(buf, '\0', sizeof(buf));
    while(1)
    {
        printf("please enter# ");
        fflush(stdout);

        ssize_t size = read(1, buf, sizeof(buf)-1);
        if(size < 0)
        {
            perror("read");
            break;
        }
        buf[size-1] = '\0';
        
        int ret=write(fd, buf, strlen(buf)+1);

        if(strncmp(buf, "quit", 4) == 0)
            break;

        printf("client# %s\n", buf);
    }

    close(fd);
    return 0;
}
