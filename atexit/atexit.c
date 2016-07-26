#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
void func1()
{
    printf("this is func1, %d\n",getpid());
}

void func2()
{
    printf("this is func2, %d\n",getpid());
}

void func3()
{
    printf("this is func3, %d\n",getpid());
}

int main()
{
    atexit(func1);
    atexit(func2);
    atexit(func3);

    _exit(1);
    //return 0; 
}

