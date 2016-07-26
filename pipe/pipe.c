#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    int pipe_fd[2];
    if(pipe(pipe_fd) < 0)
    {
        perror("pipe");
        return 1;
    }

    // danxiang tongxin
    pid_t id = fork();
    if(id == 0)   // child
    {
        // child write
        close(pipe_fd[0]);
        const char* msg = "i am a child\n";

        // one write more
        int count = 5;
        while(count -- > 0)
        {
            write(pipe_fd[1], msg, strlen(msg));
            printf("child write: %s\n", msg);
            sleep(1);
        }
       
    }
    else  // father read
    {
        close(pipe_fd[1]);

        char buf[1024];
        memset(buf, '\0', sizeof(buf));
   
        // read more
        int count = 5;
        while(count-- > 0)
        {
            sleep(2);
            ssize_t _size = read(pipe_fd[0], buf, sizeof(buf));
            if(_size > 0)
            {
                printf("father read: %s\n", buf);
            }else if(_size == 0){
                printf("father read EOF.........\n");
                break;
            }else{
                perror("read");
                break;
            }
        }
        
        int status = 0;
        pid_t ret = waitpid(id, &status, 0);
        if(id == ret)
        {
            printf("wait sucsess\n");
        }
        else
        {
            printf("wait failed\n");
            printf("get log: %d\n", status&0xff);            
            printf("get code: %d\n", status&0xff);            
        }
    }
    return 0;
}
