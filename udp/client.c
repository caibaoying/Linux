#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define _PORT_ 8080
#define _IP_ "127.0.0.1"
#define _SIZE_ 1024

int main()
{
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0)
    {
        perror("socket");
        return 1;
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(_PORT_);
    server.sin_addr.s_addr = inet_addr(_IP_);

    char buf[_SIZE_];
    struct sockaddr_in client;
    int len = sizeof(client);
    while(1)
    {
        printf("please enter$ ");
        fflush(stdout);
        memset(buf, '\0', sizeof(buf));

        ssize_t _size = read(0, buf, sizeof(buf)-1);
        buf[_size-1] = '\0';
        printf("client# %s\n", buf);

        sendto(sock, buf, strlen(buf), 0, (struct sockaddr*)&server, sizeof(server));
        memset(buf, '\0', sizeof(buf));

       // int _ret = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr*)&client, &len); 
       // if(_ret < 0)
       // {
       //     perror("recvfrom");
       //     return 2;
       // }
       // printf("server# %s\n", buf);

    }
    return 0;
}
