#include <stdio.h>
#include <string.h>
#include <unistd.h>

void func()
{
    char array[102]; 
    const char round[4] = {'/','-','\\','-'};
    memset(array, '\0', sizeof(array));
    int i = 0;
    for(i = 0; i < 101; ++i)
    {    
        printf("[%-101s][%d%][%c]\r", array, i, round[i%4]);
        array[i] = '=';  
        usleep(100000);
        fflush(stdout);
    }
    printf("\n");
}

int main()
{   
    func();
    return 0;
}

