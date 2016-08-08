#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct list
{
    struct list* _next;
    int _data;
}ListNode;

ListNode* head = NULL;
static pthread_mutex_t lock;
static pthread_mutex_t need_product;

void init_list(ListNode* phead)
{
    if(phead != NULL)
    {
        phead->_next = NULL;
        phead->_data = 0;
    }
}
void alloc_list(ListNode** node)
{
    *node = (ListNode*)malloc(sizeof(ListNode));
}

void delete_list(ListNode* node)
{
    free(node);
}
void* consum1(void* arg)
{
    ListNode* tmp = NULL;
    while(1)
    {
        pthread_mutex_lock(&lock);
        while(NULL == head)
        {
            pthread_cond_wait(&need_product, &lock);
        }

        tmp = head;
        head = head->_next;
        tmp->_next = NULL;
        pthread_mutex_unlock(&lock);

        printf("consumer1 success:  %d\n", tmp->_data);
        delete_list(tmp);
        tmp = NULL;
    }
}
void* product1(void* arg)
{
    while(1)
    {
        ListNode* tmp;
        alloc_list(&tmp);
        pthread_mutex_lock(&lock);
        
        tmp->_next = head;
        tmp->_data = rand()%1234;
        head = tmp;

        pthread_mutex_unlock(&lock);
        printf("product1 success,call consumer :%d\n", head->_data);
        sleep(2);

        pthread_cond_signal(&need_product);
     }
}
void* consum2(void* arg)
{
    ListNode* tmp = NULL;
    while(1)
    {
        pthread_mutex_lock(&lock);
        while(NULL == head)
        {
            pthread_cond_wait(&need_product, &lock);
        }

        tmp = head;
        head = head->_next;
        tmp->_next = NULL;
        pthread_mutex_unlock(&lock);

        printf("consumer2 success:  %d\n", tmp->_data);
        delete_list(tmp);
        tmp = NULL;
    }
}
void* product2(void* arg)
{
    while(1)
    {
        ListNode* tmp;
        alloc_list(&tmp);
        pthread_mutex_lock(&lock);
        
        tmp->_next = head;
        tmp->_data = rand()%1234;
        head = tmp;

        pthread_mutex_unlock(&lock);
        printf("product2 success,call consumer :%d\n", head->_data);
        sleep(3);
        pthread_cond_signal(&need_product);
     }
}
int main()
{
//    ListNode* tmp = head;
//    while(tmp)
//    {
//        tmp = tmp->_next;
//        printf("%d,", tmp->_data);
//    }
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
    return 0;
}
