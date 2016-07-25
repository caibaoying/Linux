#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>
#include <sys/types.h>

int main()
{
    while(1)
    {
        char hostname[32];
        memset(hostname, '\0', sizeof(hostname));
        gethostname(hostname, sizeof(hostname));
        
        struct passwd * pwd = getpwuid(getuid());
        printf("[%s@%s]$ ",pwd->pw_name, hostname);
        fflush(stdout);

        char buf[1024];
        memset(buf, '\0', sizeof(buf));
        ssize_t size = read(0, buf, sizeof(buf));
        if(size > 0){
            buf[size-1] = '\0';
        }
        //printf("%s", buf);

        char* my_argv[64];
        int index = 1;
        char* p = buf;
        my_argv[0] = p;
        
        while(*p != '\0') {
            if(*p == ' '){
                *p = '\0';
                p++;
                my_argv[index++] = p;
                continue;
            }
            p++;
        }
        my_argv[index] = NULL;


        pid_t id = fork();
        if(id == -1){
            printf("fork error\n");
        }else if(id == 0){
            //printf("child: pid = %d\n", getpid());
            //sleep(2);

           // char *const my_argv[] = 
           // {
           //     "ls","-l","-a","-i",NULL
           // };
            
           // char* const my_env[] = 
           // {
           //     "PATH=test/it/changed",NULL
           // };

           // execl("/bin/ls", "ls", "-l", "-i", "-a", NULL); 
           // execlp("ls", "ls", "-l", "-i", "-a", NULL);
           // execlp("ls", "ls", "-l", "-i", "-a", NULL);
           // execv("/bin/ls",my_argv);
           // execle("./test", "test", NULL, my_env);
              execvp(my_argv[0], my_argv);
        }else{
           // printf("father: pid = %d\n", getpid());
            pid_t ret = waitpid(id, NULL, 0);

            if(ret > 0){
            //    printf("father wait success\n");
            }else{
            //    printf("father wait failed\n");
            }
        }
    }
    return 0;
}
