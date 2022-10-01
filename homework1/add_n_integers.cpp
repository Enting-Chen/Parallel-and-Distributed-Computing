#include<omp.h>
#include<iostream>
using namespace std;

int main(){
    int sum = 0;
    #pragma omp parallel for num_threads(10) reduction(+: sum)
    for ( int i = 1; i <= 100; i++ ){
        sum += i;
    }
    cout << "sum = " << sum << endl;
}