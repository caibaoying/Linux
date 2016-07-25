#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define _PORT_ 8080
#define _IP_ "127.0.0.1"
#define _SIZE_ 1024

int main()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        perror("socket");
        return 1;
    }

    struct sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_port = htons(_PORT_);
    local.sin_addr.s_addr = inet_addr(_IP_);

    if(bind(sock, (struct sockaddr*)&local, sizeof(local)) < 0)
    { 
        perror("bind");
        return 2;
    }

    if(listen(sock, 5) < 0)
    {
        perror("listen");
        return 3;
    }

    printf("listen success\n");
    struct sockaddr_in peer;
    while(1)
    {
        int len = sizeof(peer);
        int fd = accept(sock, (struct sockaddr*)&peer, &len);
        char buf[_SIZE_];
        memset(buf, '\0', sizeof(buf));
        inet_ntop(AF_INET, &peer.sin_addr, buf, sizeof(buf));
        printf("get connect, ip is :%s, port is %d\n", buf, ntohs(peer.sin_port));

        while(1)
        {
            char buf[_SIZE_];
            memset(buf, '\0', sizeof(buf));
            int _size = read(fd, buf, sizeof(buf)-1);

            printf("client# %s\n", buf);
            if(_size < 0)
            {
                perror("read");
                return 4;
            }
            if(write(fd, buf, strlen(buf)) < 0)
            {
                perror("write");
                return 5;
            }
            printf("server# %s\n", buf);
        }
    }
    return 0;
}

