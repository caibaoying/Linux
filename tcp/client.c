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

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(_PORT_);
    server.sin_addr.s_addr = inet_addr(_IP_);

    int ret = connect(sock, (struct sockaddr*)&server, sizeof(server));

    if(ret < 0)
    {
        perror("connet");
        return 2;
    }

    printf("connet success\nu");

    char buf[_SIZE_];
    while(1)
    {
        memset(buf, '\0', sizeof(buf));
        printf("client# ");
        fflush(stdout);
        fgets(buf, sizeof(buf), stdin);
        buf[strlen(buf)-1] = '\0';
        write(sock, buf, sizeof(buf));
        read(sock, buf, sizeof(buf));
        printf("server# %s\n", buf);
    }
    return 0;
}
