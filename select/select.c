#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int fds[64];
const int fds_nums = sizeof(fds)/sizeof(fds[0]);

int startup(const char* _ip, int _port)
{
    int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_sock < 0)
    {
        perror("socket");
        exit(1);
    }
    
    int opt = 1;
    setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_port = htons(_port);
    local.sin_addr.s_addr = inet_addr(_ip);

    if(bind(listen_sock, (struct sockaddr*)&local, sizeof(local)) < 0)
    {
        perror("bind");
        exit(2);
    }

    if(listen(listen_sock, 5) < 0) 
    {
        perror("listen");
        exit(3);
    }

    return listen_sock;
}

void usage(const char* proc)
{
    printf("Usage : %s, [ip][port]", proc);
}

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        usage(argv[0]);
        exit(4);
    }

    int i = 0;
    for( ; i < fds_nums; i++)
    {
        fds[i++] = -1;
    }

    int listen_sock =  startup(argv[1], atoi(argv[2]));
    
    fd_set rset;
    FD_ZERO(&rset);
    FD_SET(listen_sock, &rset);

    fds[0] = listen_sock;

    int done = 0;
    while(!done)
    {
        int max_fd = -1;
        for(i = 0; i<fds_nums; i++)
        {
            if(fds[i] > 0) // ok-->fd
            {
                FD_SET(fds[i], &rset);
                max_fd = max_fd < fds[i]?fds[i]:max_fd;
            }

        }
        struct timeval timeout = {0, 0};
        switch(select(max_fd+1, &rset, NULL, NULL, NULL))
        {
            case 0:
                printf("timeout...\n");
                break;
            case -1:
                perror("select");
                break;
            default:
                {
                    for(i = 0; i<fds_nums; i++)
                    {
                        if(i == 0 && FD_ISSET(listen_sock, &rset))
                        {
                            struct sockaddr_in peer;
                            int len = sizeof(peer);
                            int new_fd = accept(listen_sock, (struct sockaddr*)&peer, &len);
                            if(new_fd > 0)
                            {
                                printf("get a new client:%s:%d\n", inet_ntoa(peer.sin_addr), ntohs(peer.sin_port));
                                int j = 0;
                                for(j = 0; j<fds_nums; j++)
                                {
                                    if(fds[i] == -1)
                                    {
                                        fds[i] = new_fd;
                                        break;
                                    }
                                }

                                if(j == fds_nums)
                                {
                                    close(new_fd);
                                }
                            }
                        }
                        else
                        {
                            if(FD_ISSET(fds[i], &rset))
                            {
                                char buf[1024];
                                memset(buf, '\0', sizeof(buf));
                                ssize_t _s = read(fds[i], buf, sizeof(buf)-1);
                                if(_s > 0)
                                {
                                    printf("client# %s\n", buf);
                                }
                                else if(_s == 0)
                                {
                                    printf("client close...\n");
                                    close(fds[i]);
                                    fds[i] = -1;
                                }
                                else
                                {
                                    perror("read");
                                }
                            }
                        }
                    }
                }
                break;
        }
    }

    return 0;
}
