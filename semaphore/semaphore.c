#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#define _SIZE_ 64

sem_t space_nums;
sem_t data_nums;

int buf[_SIZE_];
static pthread_mutex_t lock_com;
static pthread_mutex_t lock_pro;

void init_all_sem()
{
    sem_init(&space_nums, 0, _SIZE_);
    sem_init(&data_nums, 0, 0);

    pthread_mutex_init(&lock_com, NULL);
    pthread_mutex_init(&lock_pro, NULL);
}

void* consum1(void* arg)
{
    int index = 0;
    while(1)
    {
        pthread_mutex_lock(&lock_com);
        sem_wait(&data_nums);
        int tmp = buf[index++];

        printf("consum1 success: %d\n", tmp);

        sem_post(&space_nums);
        index %= _SIZE_;
        sleep(2);
        pthread_mutex_unlock(&lock_com);
    }
}

void* product1(void* arg)
{
    int index = 0;
    int data = rand()%1234;
    while(1)
    {
        pthread_mutex_lock(&lock_pro);
        sem_wait(&space_nums);
        buf[index++] = data;

        printf("product1 success: %d\n", data);
    
        sem_post(&data_nums);
        index %= _SIZE_;
        sleep(1);
        pthread_mutex_unlock(&lock_pro);
    }
}

void* consum2(void* arg)
{
    int index = 0;
    while(1)
    {
        pthread_mutex_lock(&lock_com);
        sem_wait(&data_nums);
        int tmp = buf[index++];

        printf("consum2 success: %d\n", tmp);

        sem_post(&space_nums);
        index %= _SIZE_;
        sleep(4);
        pthread_mutex_unlock(&lock_com);
    }
}

void* product2(void* arg)
{
    int index = 0;
    int data = rand()%1234;
    while(1)
    {
        pthread_mutex_lock(&lock_pro);
        sem_wait(&space_nums);
        buf[index++] = data;

        printf("product2 success: %d\n", data);
    
        sem_post(&data_nums);
        index %= _SIZE_;
        sleep(3);
        pthread_mutex_unlock(&lock_pro);
    }
}

void destroy_all_sem()
{
    sem_destroy(&space_nums);
    sem_destroy(&data_nums);

    pthread_mutex_destroy(&lock_com);
    pthread_mutex_destroy(&lock_pro);
}

int main()
{
    init_all_sem();
    pthread_t consumer1;
    pthread_t producter1;
    pthread_t consumer2;
    pthread_t producter2;

    pthread_create(&consumer1, NULL, consum1, NULL);
    pthread_create(&producter1, NULL, product1, NULL);
    pthread_create(&consumer2, NULL, consum2, NULL);
    pthread_create(&producter2, NULL, product2, NULL);
    
    pthread_join(consumer1, NULL);
    pthread_join(producter1, NULL);
    pthread_join(consumer2, NULL);
    pthread_join(producter2, NULL);
    destroy_all_sem();
    return 0;
}
