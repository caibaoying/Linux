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

    close(1);
    int new_fd = dup(fd);
    
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
