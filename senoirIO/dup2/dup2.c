#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
int main()
{
    int fd = open("./log", O_CREAT | O_WRONLY , 0666);
    if(fd < 0)
    {
        perror("open");
        return 1;
    }

    printf("old: %d\n", fd);
    int new_fd = dup2(fd, 1);
    printf("new: %d\n", new_fd);
    
    char buf[1024];
    while(1)
    {
        memset(buf, '\0', sizeof(buf));
        ssize_t _size = read(0, buf, sizeof(buf)-1);
        if(_size > 0)
        {
           // buf[_size - 1] = '\0';
            if(strncmp("quit", buf, 4) == 0)
                break;
        }
        printf("%s", buf);
        fflush(stdout); 
    }
    return 0;
}
