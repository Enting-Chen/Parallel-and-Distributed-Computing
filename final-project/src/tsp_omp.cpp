#include <limits.h>
#include <omp.h>
#include <sys/time.h>

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <queue>
#include <stack>
#include <vector>

#define GET_TIME( now )                         \
    {                                           \
        struct timeval t;                       \
        gettimeofday( &t, NULL );               \
        now = t.tv_sec + t.tv_usec / 1000000.0; \
    }

using namespace std;

#define GRAPH_SIZE 11
#define NO_CITY -1

typedef int city;

//global variables: accessible to all threads
int graph[ GRAPH_SIZE ][ GRAPH_SIZE ];

int thread_count;

int init_tour_count;

int bar_count;

struct Tour {
    int cost;
    vector<int> route;

    void Add_city( city );
    void Remove_last_city();
    bool Feasible( Tour&, city& );
} best;

deque<Tour> q;

inline void Tour::Add_city( city current ) {
    int added = graph[ route.back() ][ current ];
    cost += added;
    route.push_back( current );
    //cout << curr_tour.route.size() - 1 << endl;
    //cout << "added: " << added << endl;
    //cout << "Added: " << current << " " << curr_tour.cost << endl;
}

inline void Tour::Remove_last_city() {
    cost -= graph[ route.back() ][ route[ route.size() - 2 ] ];
    route.pop_back();
}

inline bool Tour::Feasible( Tour& best, city& current ) {
    return find( route.begin(), route.end(), current ) == route.end() && cost + graph[ route.back() ][ current ] < best.cost;
}

void Read_graph() {
    ifstream is( "tsp2.txt" );
    for ( int i = 0; i < GRAPH_SIZE; i++ ) {
        for ( int j = 0; j < GRAPH_SIZE; j++ ) {
            is >> graph[ i ][ j ];
        }
    }
    is.close();
}

void WriteTour( Tour& tour ) {
    ofstream os( "tsp_result.txt" );
    for ( int i = 0; i < GRAPH_SIZE + 1; i++ ) {
        os << tour.route[ i ] << " ";
    }
}

void Update_best_tour( Tour& tour ) {
    if ( best.cost > tour.cost ) {
        best = tour;
    }
}

void Build_initial_queue( void ) {
    int curren_size = 0;
    Tour tour;

    tour.cost = 0;
    tour.route.push_back( 0 );

    //bfs
    q.push_back( tour );

    curren_size++;

    while ( curren_size < thread_count ) {
        tour = q.front();
        q.pop_front();
        curren_size--;
        for ( city i = 1; i < GRAPH_SIZE; i++ )
            if ( find( tour.route.begin(), tour.route.end(), i ) == tour.route.end() ) {
                tour.Add_city( i );
                q.push_back( tour );
                curren_size++;
                tour.Remove_last_city();
            }
    }
    init_tour_count = curren_size;
}

void Set_init_tours( long my_rank, int* my_first_tour_p,
                     int* my_last_tour_p ) {
    int quotient = init_tour_count / thread_count;
    int remainder = init_tour_count % thread_count;
    int my_count;

    if ( my_rank < remainder ) {
        my_count = quotient + 1;
        *my_first_tour_p = my_rank * my_count;
    }
    else {
        my_count = quotient;
        *my_first_tour_p = my_rank * my_count + remainder;
    }
    *my_last_tour_p = *my_first_tour_p + my_count - 1;
}

void Partition_tree( int & my_rank, stack<Tour>& s ) {
    int my_first_tour, my_last_tour;

    #pragma omp master
    Build_initial_queue();

    #pragma omp barrier
    Set_init_tours( my_rank, &my_first_tour, &my_last_tour );
    for ( int i = my_last_tour; i >= my_first_tour; i-- ) {
        s.push( q[ i ] );
    }
}

void Tsp() {
    int my_rank = omp_get_thread_num();
    stack<Tour> s;
    stack<Tour> available;
    Tour curr_tour;
    curr_tour.cost = 0;
    curr_tour.route.clear();
    best.cost = INT_MAX;

    Partition_tree( my_rank, s );

    while ( !s.empty() ) {
        curr_tour = s.top();
        s.pop();
        if ( curr_tour.route.size() == GRAPH_SIZE ) {
            if ( curr_tour.cost + graph[ curr_tour.route.back() ][ 0 ] > best.cost ) {
                continue;
            }
            curr_tour.Add_city( 0 );
            // for ( int i = 0; i < curr_tour.route.size(); i++ ) {
            //     cout << curr_tour.route[ i ] << " ";
            // }
            // cout << endl
            //      << curr_tour.cost << endl;
            //int cost = curr_tour.cost;
            #pragma omp critical
            Update_best_tour( curr_tour );
        }
        else {
            for ( city i = GRAPH_SIZE - 1; i >= 1; i-- ) {
                if ( curr_tour.Feasible( best, i ) ) {
                    curr_tour.Add_city( i );
                    s.push( curr_tour );
                    curr_tour.Remove_last_city();
                }
            }
        }
    }
    // if Feasible
}

int main( int argc, char* argv[] ) {
    Read_graph();

    double start, finish;

    //get number of threads from command line
    thread_count = strtol( argv[ 1 ], NULL, 10 );

    best.cost = INT_MAX;
    best.route.clear();

    start = omp_get_wtime();

    #pragma omp parallel num_threads( thread_count ) default( none )
    Tsp();
    finish = omp_get_wtime();

    WriteTour( best );
    for ( int i = 0; i < best.route.size(); i++ ) {
        cout << best.route[ i ] << " ";
    }
    cout << endl
         << best.cost << endl;

    printf( "Cost = %d\n", best.cost );
    printf( "Elapsed time = %e seconds\n", finish - start );

    return 0;
}
