#include<iostream>
#include<unistd.h>
#include<omp.h>
#include<chrono>
using namespace std;
using namespace std::chrono;

#define size 20
#define ThreadNumber 4

void dummy(){
    sleep(1);
}

int main(){
    auto t1 = system_clock::now();
    #pragma omp parallel for num_threads(ThreadNumber) \
    schedule(dynamic, 2)
    for( int i = 0; i < size; i++ ){
        dummy();
    }
    auto t2 = system_clock::now();
    cout << "Time taken: " << duration_cast<milliseconds>(t2-t1).count() << " ms" << endl;
}