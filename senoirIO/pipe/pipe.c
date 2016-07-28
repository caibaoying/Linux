#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

int main()
{
    int fd[2];
    int ret = socketpair(AF_LOCAL, SOCK_STREAM, 0, fd);
    if(ret < 0)
    {
        perror("socketpair");
        return 1;
    }

    pid_t id = fork();

    if(id < 0)
    {
        perror("fork");
        return 2;
    }
    else if(id == 0)  // child
    {
        close(fd[0]);
        const char* msg = "hello bit";
        char buf[1024];
        while(1)
        {
            memset(buf, '\0',sizeof(buf));
            write(fd[1], msg, strlen(msg));
            read(fd[1], buf, sizeof(buf));
            sleep(1);
            printf("father-->child: %s\n", buf);
        }
    }
    else   // father
    {
        close(fd[1]);
        char buf[1024];
        const char* msg = "ni hao";
        while(1)
        {
            memset(buf, '\0', sizeof(buf));
            read(fd[0], buf, sizeof(buf));
            printf("child-->father: %s\n", buf);
            write(fd[0], msg, strlen(msg));
        }
    }
    return 0;
}
