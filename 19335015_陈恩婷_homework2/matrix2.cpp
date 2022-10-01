#include <iostream>
using namespace std;

void add( int** A, int** B, int** C, int N1, int N2 ) {
    for ( int i = 0; i < N1; i++ ) {
        for ( int j = 0; j < N2; j++ ) {
            C[ i ][ j ] = A[ i ][ j ] + B[ i ][ j ];
        }
    }
}

void sub( int** A, int** B, int** C, int N1, int N2 ) {
    for ( int i = 0; i < N1; i++ ) {
        for ( int j = 0; j < N2; j++ ) {
            C[ i ][ j ] = A[ i ][ j ] - B[ i ][ j ];
        }
    }
}

void multiply( int** A, int** B, int** C, int N1, int N2, int N3 ) {
    for ( int i = 0; i < N1; i++ ) {
        for ( int j = 0; j < N3; j++ ) {
            C[ i ][ j ] = 0;
            for ( int k = 0; k < N2; k++ ) {
                C[ i ][ j ] = C[ i ][ j ] + A[ i ][ k ] * B[ k ][ j ];
            }
        }
    }
}

void divide( int** p, int n1, int n2, int number, int** result ) {
    int rows = n1 / 2;  //行数减半
    int cols = n2 / 2;  //列数减半
    switch ( number ) {
    case 1:
        // result = new int[rows][cols];
        //result = new int* [rows];
        for ( int i = 0; i < rows; i++ ) {
            result[ i ] = new int[ cols ]();
            for ( int j = 0; j < cols; j++ ) {
                result[ i ][ j ] = p[ i ][ j ];
            }
        }
        return;

    case 2:
        // result = new int[rows][n-cols];
        //result = new int* [rows];
        for ( int i = 0; i < rows; i++ ) {
            result[ i ] = new int[ n2 - cols ]();
            for ( int j = 0; j < n2 - cols; j++ ) {
                result[ i ][ j ] = p[ i ][ j + cols ];
            }
        }
        return;

    case 3:
        // result = new int[n-rows][cols];
        //result = new int* [n1 - rows];
        for ( int i = 0; i < n1 - rows; i++ ) {
            result[ i ] = new int[ cols ]();
            for ( int j = 0; j < cols; j++ ) {
                result[ i ][ j ] = p[ i + rows ][ j ];
            }
        }
        return;

    case 4:
        // result = new int[n-rows][n-cols];
        //result = new int* [n1 - rows];
        for ( int i = 0; i < n1 - rows; i++ ) {
            result[ i ] = new int[ n2 - cols ]();
            for ( int j = 0; j < n2 - cols; j++ ) {
                result[ i ][ j ] = p[ i + rows ][ j + cols ];
            }
        }
        return;
    }
}

void divide_and_conquer( int** A, int** B, int** C, int N1, int N2, int N3 ) {
    if ( N1 <= 3 || N2 <= 3 || N3 <= 3 ) {
        multiply( A, B, C, N1, N2, N3 );
        return;
    }
    int** A11 = new int*[ N1 / 2 ];
    int** A12 = new int*[ N1 / 2 ];
    int** A21 = new int*[ N1 - N1 / 2 ];
    int** A22 = new int*[ N1 - N1 / 2 ];

    int** B11 = new int*[ N2 / 2 ];
    int** B12 = new int*[ N2 / 2 ];
    int** B21 = new int*[ N2 - N2 / 2 ];
    int** B22 = new int*[ N2 - N2 / 2 ];

    int** C11 = new int*[ N1 / 2 ];
    int** C12 = new int*[ N1 / 2 ];
    int** C21 = new int*[ N1 - N1 / 2 ];
    int** C22 = new int*[ N1 - N1 / 2 ];
    int** D11 = new int*[ N1 / 2 ];
    int** D12 = new int*[ N1 / 2 ];
    int** D21 = new int*[ N1 - N1 / 2 ];
    int** D22 = new int*[ N1 - N1 / 2 ];

    divide( A, N1, N2, 1, A11 );
    divide( A, N1, N2, 2, A12 );
    divide( A, N1, N2, 3, A21 );
    divide( A, N1, N2, 4, A22 );

    divide( B, N2, N3, 1, B11 );
    divide( B, N2, N3, 2, B12 );
    divide( B, N2, N3, 3, B21 );
    divide( B, N2, N3, 4, B22 );

    divide( C, N1, N3, 1, C11 );
    divide( C, N1, N3, 2, C12 );
    divide( C, N1, N3, 3, C21 );
    divide( C, N1, N3, 4, C22 );
    divide( C, N1, N3, 1, D11 );
    divide( C, N1, N3, 2, D12 );
    divide( C, N1, N3, 3, D21 );
    divide( C, N1, N3, 4, D22 );

    divide_and_conquer( A11, B11, C11, N1 / 2, N2 / 2, N3 / 2 );
    divide_and_conquer( A12, B21, D11, N1 / 2, N2 - N2 / 2, N3 / 2 );
    divide_and_conquer( A11, B12, C12, N1 / 2, N2 / 2, N3 - N3 / 2 );
    divide_and_conquer( A12, B22, D12, N1 / 2, N2 - N2 / 2, N3 - N3 / 2 );
    divide_and_conquer( A21, B11, C21, N1 - N1 / 2, N2 / 2, N3 / 2 );
    divide_and_conquer( A22, B21, D21, N1 - N1 / 2, N2 - N2 / 2, N3 / 2 );
    divide_and_conquer( A21, B12, C22, N1 - N1 / 2, N2 / 2, N3 - N3 / 2 );
    divide_and_conquer( A22, B22, D22, N1 - N1 / 2, N2 - N2 / 2, N3 - N3 / 2 );

    add( C11, D11, C11, N1 / 2, N3 / 2 );
    add( C12, D12, C12, N1 / 2, N3 - N3 / 2 );
    add( C21, D21, C21, N1 - N1 / 2, N3 / 2 );
    add( C22, D22, C22, N1 - N1 / 2, N3 - N3 / 2 );
    //将C11，C12,C21,C21合并为C矩阵
    for ( int i = 0; i < N1 / 2; i++ ) {
        delete[] D11[ i ];
        delete[] D12[ i ];
        for ( int j = 0; j < N3 / 2; j++ ) {
            C[ i ][ j ] = C11[ i ][ j ];
        }
        for ( int j = 0; j < N3 - N3 / 2; j++ ) {
            C[ i ][ j + N3 / 2 ] = C12[ i ][ j ];
        }
    }
    for ( int i = 0; i < N1 - N1 / 2; i++ ) {
        delete[] D21[ i ];
        delete[] D22[ i ];
        for ( int j = 0; j < N3 / 2; j++ ) {
            C[ i + N1 / 2 ][ j ] = C21[ i ][ j ];
        }
        for ( int j = 0; j < N3 - N3 / 2; j++ ) {
            C[ i + N1 / 2 ][ j + N3 / 2 ] = C22[ i ][ j ];
        }
    }
    delete[] D11;
    delete[] D12;
    delete[] D21;
    delete[] D22;
}

int main() {
    int** a = new int*[ 300 ];
    int** b = new int*[ 300 ];
    int** c = new int*[ 300 ];
    //int** c2 = new int*[ 300 ];

    for ( int i = 0; i < 300; i++ )  //分配连续内存
    {
        a[ i ] = new int[ 300 ]();
        b[ i ] = new int[ 300 ]();
        c[ i ] = new int[ 300 ]();
        //c2[ i ] = new int[ 300 ]();
    }
    for ( int i = 0; i < 300; i++ ) {
        for ( int j = 0; j < 300; j++ ) {
            a[ i ][ j ] = b[ i ][ j ] = i + j;
        }
    }
    divide_and_conquer( a, b, c, 300, 300, 300 );
    //multiply( a, b, c2, 300, 300, 300 );
    cout << "Okay" << endl;
}