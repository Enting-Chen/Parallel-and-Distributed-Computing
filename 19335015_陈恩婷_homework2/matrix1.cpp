#include<iostream>
using namespace std;

int a[ 300 ][ 300 ], b[ 300 ][ 300 ], c[300][300];

void multiply( int** A, int** B, int** C, int N ) {
    for ( int i = 0; i < N; i++ ) {
        for ( int j = 0; j < N; j++ ) {
            C[ i ][ j ] = 0;
            for ( int k = 0; k < N; k++ ) {
                C[ i ][ j ] = C[ i ][ j ] + A[ i ][ k ] * B[ k ][ j ];
            }
        }
    }
}

int main(){
    int** a = new int*[ 300 ];
    int** b = new int*[ 300 ];
    int** c = new int*[ 300 ];  
    for ( int i = 0; i < 300; i++ )    //分配连续内存
    {
        a[ i ] = new int[ 300 ];
        b[ i ] = new int[ 300 ];
        c[ i ] = new int[ 300 ];
    }
    for ( int i = 0; i < 300;  i++){
        for ( int j = 0; j < 300;  j++){
            a[ i ][ j ] = b[ i ][ j ] = i + j;
        }
    }
    multiply( a, b, c, 300 );
    delete[] a;
    delete[] b;
    delete[] c;
    cout << "Okay" << endl;
}