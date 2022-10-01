#include <immintrin.h>
#include <stdio.h>

int main( int argc, char const* argv[] ) {
    __m256 float_vec_0 = _mm256_set_ps( 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0 );
    float* flo1 = (float*)&float_vec_0;
    printf( "float1:\t\t%f, %f, %f, %f, %f, %f, %f, %f\n", flo1[ 0 ], flo1[ 1 ], flo1[ 2 ], flo1[ 3 ], flo1[ 4 ], flo1[ 5 ], flo1[ 6 ], flo1[ 7 ] );

    __m256 float_vec_1 = _mm256_set_ps( 17.0, 18.0, 19.0, 20.0, 21.0, 22.0, 23.0, 24.0 );
    float* flo2 = (float*)&float_vec_1;
    printf( "float2:\t\t%f, %f, %f, %f, %f, %f, %f, %f\n", flo2[ 0 ], flo2[ 1 ], flo2[ 2 ], flo2[ 3 ], flo2[ 4 ], flo2[ 5 ], flo2[ 6 ], flo2[ 7 ] );

    __m256 float_result = _mm256_add_ps( float_vec_0, float_vec_1 );

    float* flo = (float*)&float_result;
    printf( "float:\t\t%f, %f, %f, %f, %f, %f, %f, %f\n", flo[ 0 ], flo[ 1 ], flo[ 2 ], flo[ 3 ], flo[ 4 ], flo[ 5 ], flo[ 6 ], flo[ 7 ] );

    return 0;
}