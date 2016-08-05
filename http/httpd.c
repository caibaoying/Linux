#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>
#include <fcntl.h>


#define _SIZE_ 1024

int startup(const char* _ip, int _port)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        perror("sock");
        exit(1);
    }

    int opt = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_port = htons(_port);
    local.sin_addr.s_addr = inet_addr(_ip);

    if(bind(sock, (struct sockaddr*)&local, sizeof(local)) < 0)
    {
        perror("sock");
        exit(3);
    }

    if(listen(sock, 5) < 0)
    {
        perror("listen");
        exit(4);
    }

    return sock;
}

void usage(const char* proc)
{
    printf("proc:%s[ip][port]\n", proc);
}

int get_line(int sock, char buf[], int len)
{
    if(!buf || len <= 0)
    {
        return -1;
    }

    char c = '\0';
    
    int n = 0;
    int i = 0;
    while((i < len-1) && (c != '\n'))
    {
        n = recv(sock, &c, 1, 0);
        if(n > 0)
        {
            if(c == '\r')
            {
                n = recv(sock, &c, 1, MSG_PEEK);
                if(n > 0 && c == '\n')
                {
                    recv(sock, &c, 1, 0);
                }
                else
                {
                    c = '\n';
                }
            }
            buf[i++] = c;
        }
        else
        {
            c = '\n';
        }
    }
    buf[i] = '\0';
    return i;
}

static void echo_errno(int sock)
{}

static clear_header(int sock)
{
    int ret = -1;
    char buf[_SIZE_];
    do{
        ret = get_line(sock, buf, sizeof(buf));
    }while((ret > 0) && (strcmp(buf, "\n") != 0));
}

void echo_www(int sock, const char* path, int size)
{
    int fd = open(path, O_RDONLY);
    if(fd < 0)
    {
        echo_errno(sock);
        return;
    }

    printf("server path: %s\n", path);
    char buf[_SIZE_];
    sprintf(buf, "HTTP/1.0 200 OK\r\n\r\n");
    send(sock, buf, strlen(buf), 0);

    if(sendfile(sock, fd, NULL, size) < 0)
    {
        echo_errno(sock);
        return;
    }

    close(fd);
}

void exec_cgi(int sock, const char* path, const char* method, const char* query_string)
{
    char buf[_SIZE_];
    int ret = -1;
    int content_length = -1;
    if(strcmp(method, "GET") == 0)
    {
        clear_header(sock);
    }
    else
    {
        do{
            ret = get_line(sock, buf, sizeof(buf));
            if((ret > 0) && (strncasecmp(buf, "Content-Length: ", 16) == 0))
            {
                content_length = atoi(&buf[16]);
            }
        }while((ret > 0) && (strcmp(buf, "\n") != 0));

        if(content_length == -1)
        {
            echo_errno(sock);
            return;
        }
    }
}

void* accept_request(void* arg)
{
    pthread_detach(pthread_self());
    
    int sock = (int)arg;
    char buf[_SIZE_];
    char method[_SIZE_];
    char url[_SIZE_];
    char path[_SIZE_];

    memset(buf, '\0', sizeof(buf));
    memset(method, '\0', sizeof(method));
    memset(url, '\0', sizeof(url));
    memset(path, '\0', sizeof(path));
    
    int cgi = 0;
    int ret = -1;
    char* query_string = NULL;

#ifdef _DEBUG_
    do{
        ret = getline(sock, buf, sizeof(buf));
        printf("%s", buf);
        fflush(stdout);
    } while((ret > 0) && (strcmp(buf, "\n") != 0));
#endif

    ret = get_line(sock, buf, sizeof(buf));
    if(ret < 0)
    {
        echo_errno(sock);
        return (void *)1;
    }

    //GET          /HTTP/1.1
    int i = 0;     // method index
    int j = 0;     // buf index

    while( (i < sizeof(method)-1) && (j < sizeof(buf)) && (!isspace(buf[j])))
    {
        method[i] = buf[j];
        i++, j++;
    }

    method[i] = '\0';
    while( isspace(buf[j]) )
    {
        j++;
    }

    if(strcasecmp(method, "GET") != 0 && strcasecmp(method, "post") != 0)
    {
        echo_errno(sock);
        return (void*)2;
    }

    if(strcasecmp(method, "POST") == 0)
    {
        cgi = 1;
    }

    i = 0; // url index
    while( (i<sizeof(url)-1) && (j < sizeof(buf)) && (!isspace(buf[j])))
    {
        url[i] = buf[j];
        i++, j++;
    }

    if(strcasecmp(method, "GET") == 0)
    {
        query_string = url;
        while(*query_string != '\0' && *query_string != '?' )
        {
            query_string++;
        }

        if(*query_string == '?')
        {
            cgi = 1;
            *query_string = '\0';
            query_string++;
        }
    }

    // /     /aa/bb/cc
    sprintf(path, "htdoc%s", url);   // htdoc/
    if(path[strlen(path) - 1] == '/')
    {
        strcat(path, "index.html");
    }

    printf("method: %s\n", method);
    printf("path: %s\n", path);
    printf("query_string: %s\n", query_string);

    struct stat st;
    if(stat(path, &st) < 0)
    {
        echo_errno(sock);
        return (void*)3;
    }
    else
    {
        if(S_ISDIR(st.st_mode))
        {
            strcpy(path, "htdoc/index.html");
        }
        else if( (st.st_mode & S_IXUSR) || (st.st_mode & S_IXGRP) || (st.st_mode & S_IXOTH))
        {
            cgi = 1;
        }
        else
        {}

        if(cgi)
        {
            exec_cgi(sock, path, method, query_string);
        }
        else
        {
            clear_header(sock);
            echo_www(sock, path, st.st_size);
        }
    }

    close(sock);
    return (void*)0;
}

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        usage(argv[0]);
        exit(1);
    }

    int listen_sock = startup(argv[1], atoi(argv[2]));
    printf("listen_sock%d\n", listen_sock);

    struct sockaddr_in peer;
    int len = sizeof(peer);

    int done = 0;
    while(!done)
    {
        int new_sock = accept(listen_sock, (struct sockaddr*)&peer, &len);

        printf("new_sck:%d\n", new_sock);

        if(new_sock > 0)
        {
            printf("debug:client socket:%s:%d\n", inet_ntoa(peer.sin_addr), ntohs(peer.sin_port));
            
            pthread_t tid;
            pthread_create(&tid, NULL, accept_request, (void *)new_sock);
        }
    }

    return 0;
}
