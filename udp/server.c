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

    struct sockaddr_in remote;
    remote.sin_family = AF_INET;
    remote.sin_port = htons(_PORT_);
    remote.sin_addr.s_addr = inet_addr(_IP_);

    if(bind(sock, (struct sockaddr*)&remote, sizeof(remote)) < 0)
    {
        perror("bind");
        return 2;
    }
    char buf[_SIZE_];
    struct sockaddr_in client;
    int len = sizeof(client);
    while(1)
    {
        memset(buf, '\0', sizeof(buf));
        int _ret = recvfrom(sock, buf, sizeof(buf), 0 , (struct sockaddr*)&client, &len);
        if(_ret < 0)
        {
            perror("recvfrom");
            return 3;
        }
        printf("client# %s\n", buf);
        sleep(1); 
       // sendto(sock, buf, strlen(buf), 0, (struct sockaddr*)&remote, sizeof(remote));
       // printf("121323234324\n");

    }
    return 0;
}
