#include <limits.h>
#include <sys/time.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <stack>
#include <vector>

using namespace std;

#define GRAPH_SIZE 11
#define NO_CITY -1

typedef int city;

#define GET_TIME( now )                         \
    {                                           \
        struct timeval t;                       \
        gettimeofday( &t, NULL );               \
        now = t.tv_sec + t.tv_usec / 1000000.0; \
    }

int graph[ GRAPH_SIZE ][ GRAPH_SIZE ];

struct Tour {
    int cost;
    vector<int> route;

    void Add_city( city );
    void Remove_last_city();
    bool Feasible( Tour &, city & );
};

inline void Tour::Add_city( city current ) {
    int added = graph[ route.back() ][ current ];
    cost += added;
    route.push_back( current );
    //cout << curr_tour.route.size() - 1 << endl;
    //cout << "added: " << added << endl;
    //cout << "Added: " << current << " " << curr_tour.cost << endl;
    //cout << cost << endl;
}

inline void Tour::Remove_last_city() {
    cost -= graph[ route.back() ][ route[ route.size() - 2 ] ];
    route.pop_back();
}

inline bool Tour::Feasible( Tour & best, city & current ){
    return find( route.begin(), route.end(), current ) == route.end() 
    && cost + graph[ route.back() ][ current ] < best.cost;
}

void read_graph() {
    ifstream is( "tsp2.txt" );
    for ( int i = 0; i < GRAPH_SIZE; i++ ) {
        for ( int j = 0; j < GRAPH_SIZE; j++ ) {
            is >> graph[ i ][ j ];
        }
    }
    is.close();
}

void WriteTour( Tour &tour ) {
    ofstream os( "tsp_result.txt" );
    for ( int i = 0; i < GRAPH_SIZE + 1; i++ ) {
        os << tour.route[ i ] << " ";
    }
}

Tour tsp() {
    stack<Tour> s;
    s.empty();
    Tour tour, best, curr_tour;
    tour.cost = 0;
    tour.route.clear();
    tour.route.push_back( 0 );
    best.cost = INT_MAX;
    best.route.clear();
    s.push( tour );
    //cout << s.size() << endl;
    while ( !s.empty() ) {
        curr_tour = s.top();
        s.pop();
        if ( curr_tour.route.size() == GRAPH_SIZE ){
            curr_tour.Add_city( 0 );
            // for ( int i = 0; i < curr_tour.route.size(); i++ ) {
            //     cout << curr_tour.route[ i ] << " ";
            // }
            // cout << endl
            //      << curr_tour.cost << endl;
            //int cost = curr_tour.cost;
            if ( best.cost > curr_tour.cost ) {
                best = curr_tour;
            }           
        }
        else{
            for ( city i = GRAPH_SIZE - 1; i >= 1; i-- ) {
                if ( curr_tour.Feasible(best, i)) {
                    curr_tour.Add_city( i );
                    s.push( curr_tour );
                    curr_tour.Remove_last_city();
                }
            }
        }
        // if Feasible
    }
    return best;
    // while !Empty
}

int main() {
    double start, finish;
    read_graph();
    // for ( int i = 0; i < GRAPH_SIZE; i++ ) {
    //     for ( int j = 0; j < GRAPH_SIZE; j++ ) {
    //         cout << graph[ i ][ j ] << " ";
    //     }
    //     cout << endl;
    // }
    GET_TIME( start );
    Tour best = tsp();
    GET_TIME( finish );
    WriteTour( best );
    for ( int i = 0; i < best.route.size(); i++ ) {
        cout << best.route[ i ] << " ";
    }
    cout << endl
         << best.cost << endl;
    printf( "Cost = %d\n", best.cost );
    printf( "Elapsed time = %e seconds\n", finish - start );
}