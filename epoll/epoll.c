#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>

#define MAX_EVENTS 128

void usage(char* proc)
{
    printf("Usage: %s, [ip][port]\n", proc);
}

int startup(const char* _ip, int _port)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        perror("socket");
        exit(2);
    }

    struct sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_port = htons(_port);
    local.sin_addr.s_addr = inet_addr(_ip);

    int yes = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, 0);

    if(bind(sock, (struct sockaddr*)&local, sizeof(local)) < 0)
    {
        perror("bind");
        exit(3);
    }

    if(listen(sock, 5) < 0)
    {
        perror("listen");
        exit(4);
    }

    printf("listen success\n");
    return sock;
}

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        usage(argv[0]);
        exit(1);
    }

    int listen_sock = startup(argv[1], atoi(argv[2])); 
     
    int epoll_fd = epoll_create(256);

    printf("epoll_fd:%d \n", epoll_fd);
    if(epoll_fd < 0)
    {
        perror("epoll_create");
        close(epoll_fd);    
        exit(5);
    }

    struct epoll_event _ev, _evs[MAX_EVENTS];
    _ev.data.fd = listen_sock;
    _ev.events = EPOLLIN;
    
    if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_sock, &_ev) != 0)
    {
        perror("epoll_ctl");
        exit(6);
    }

    int timeout = -1;
    int done = 0;
    while(!done)
    {

        int epoll_n = epoll_wait(epoll_fd, _evs, MAX_EVENTS, timeout);   
        printf("epoll_n:%d \n", epoll_n);
        switch(epoll_n)
        {
            case 0:
                printf("timeout...\n");
                break;
            case -1:
                perror("epoll_wait");
                exit(7);
                break;
            default:
                {
                    struct sockaddr_in peer;
                    int len = sizeof(peer);
                    int i = 0;
                    int new_fd = 0;
                    for(; i < epoll_n; i++)
                    {
                        int fd = _evs[i].data.fd;
                        printf("fd:%d, listen_sock:%d\n", fd, listen_sock);
                        if(fd == listen_sock && (_evs[i].events & EPOLLIN)) // listen
                        {
                            if(new_fd = accept(fd, (struct sockaddr*)&peer, &len))
                            {
                                printf("get a new client: %s, %d\n", inet_ntoa(peer.sin_addr), ntohs(peer.sin_port));
                                _ev.data.fd = new_fd;
                                _ev.events = EPOLLIN;
                                _ev.data.fd = fd;
                                if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_fd, &_ev) != 0)
                                {
                                    perror("epoll_ctl");
                                    exit(8);
                                }
                            }
                        }
                        else if(_evs[i].events & EPOLLIN) // ordinary
                        {
                            char buf[1024];
                            memset(buf, '\0', sizeof(buf));

                            /////////////////////////////////////////////
                            ssize_t _s = recv(fd, buf, sizeof(buf)-1, 0);
                            if(_s > 0)
                            {
                                printf("client# %s\n", buf);
                                _ev.events = EPOLLOUT;
                                _ev.data.fd = fd;
                                epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &_ev);
                            }
                            else if(_s == 0)
                            {
                                printf("client close\n");
                                epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
                                close(fd);
                            }
                            else
                            {
                                perror("recv");
                            }
                        }
                        else if(_evs[i].events & EPOLLOUT)
                        {
                            const char* msg = "HTTP/1.1 200 OK\r\n\r\n<h1>hey man,what's up?</h1>\r\n";
                            send(fd, msg, strlen(msg), 0);
                            epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
                            close(fd);
                        }
                        else
                        {}

                     }
                }
                break;
        }
    }
    return 0;
}
