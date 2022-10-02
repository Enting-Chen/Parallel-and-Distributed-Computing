#include <stdio.h>
#include <queue>
#include <omp.h>
#include <unistd.h>
using namespace std;

int producer_num = 2;
int consumer_num = 2;

omp_lock_t pro_lock, con_lock;

void producer(queue<int> &q, int id){
    int i = 0;
    while ( i < 5 ){
        omp_set_lock(&pro_lock);
        q.push(i);
        printf("producer %d: %d\n", id, i);
        i++;
        omp_unset_lock(&pro_lock);
    }
}

void consumer(queue<int> &q, int id)
{
    int i = 0;
    while ( i < 5 ){
        omp_set_lock(&con_lock);
        while ( q.empty() );
        printf("consumer %d: %d\n", id, q.front());
        q.pop();
        omp_unset_lock(&con_lock);
        i++;
    }
}

int main(){
    queue<int> q;
    omp_init_lock(&pro_lock);
    omp_init_lock(&con_lock);
    omp_set_num_threads(producer_num + consumer_num);
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            sleep(1);
            printf("producer %d\n", omp_get_thread_num());
            producer(q, omp_get_thread_num());
        }
        #pragma omp section
        {
            sleep(1);
            printf("producer %d\n", omp_get_thread_num());
            producer(q, omp_get_thread_num());
        }
        #pragma omp section
        {
            sleep(1);
            printf("consumer %d\n", omp_get_thread_num());
            consumer(q, omp_get_thread_num());
        }
        #pragma omp section
        {
            sleep(1);
            printf("consumer %d\n", omp_get_thread_num());
            consumer(q, omp_get_thread_num());
        }
    }
    omp_destroy_lock(&pro_lock);
    omp_destroy_lock(&con_lock);
    return 0;
}
