#include <stdio.h>
#include <poll.h>

int main()
{
    struct pollfd fd_ev[1]; 
    fd_ev[0].fd = 0;
    fd_ev[0].events = POLLIN;
    fd_ev[0].revents = 0;

    int done = 0;
    while(!done)
    {
        int timeout = 500;
        switch(poll(fd_ev, 1, timeout))
        {
            case -1:
                perror("poll");
                break;
            case 0:
                printf("timeout...\n");
                break;
            default:
                {
                    if(fd_ev[0].revents & POLLIN)
                    {
                        char buf[1024];
                        ssize_t _s = read(fd_ev[0].fd, buf, sizeof(buf)-1);
                        if(_s > 0)
                        {
                            buf[_s - 1] = '\0';
                            printf("echo: %s\n", buf);
                        }
                    }
                }
                break;
        }
    }
    return 0;
}
