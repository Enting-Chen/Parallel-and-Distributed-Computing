#include <limits.h>
#include <sys/time.h>

#include <algorithm>
#include <fstream>
#include <iostream>
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

int graph[ GRAPH_SIZE ][ GRAPH_SIZE ];

class Tour {
   public:
    int cost;
    vector<int> route;

    void Add_city( city current );
    void Remove_last_city();
};

inline void Tour::Add_city( city current ) {
    int added = graph[ route.back() ][ current ];
    cost += added;
    route.push_back( current );
    //cout << curr_tour.route.size() - 1 << endl;
    //cout << "added: " << added << endl;
    //cout << "Added: " << current << " " << cost << endl;
}

inline void Tour::Remove_last_city() {
    cost -= graph[ route.back() ][ route[ route.size() - 2 ] ];
    route.pop_back();
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

void WriteTour( Tour & tour ) {
    ofstream os( "tsp_result.txt" );
    for ( int i = 0; i < GRAPH_SIZE+1; i++ ) {
        os << tour.route[ i ] << " ";
    }
}

Tour tsp() {
    stack<city> s;
    s.empty();
    Tour curr_tour, best;
    curr_tour.cost = 0;
    curr_tour.route.clear();
    curr_tour.route.push_back( 0 );
    best.cost = INT_MAX;
    best.route.clear();
    for ( city i = GRAPH_SIZE - 1; i >= 1; i-- ) {
        s.push( i );
        //cout << i << " ";
    }
    //cout << s.size() << endl;
    while ( !s.empty() ) {
        city current = s.top();
        //cout << current << endl;
        s.pop();
        if ( current == NO_CITY ) {
            curr_tour.Remove_last_city();  //Remove last city( curr tour );
        }                                  // End of child list, back up
        else {
            curr_tour.Add_city( current );
            //cout << curr_tour.cost << endl;
            if ( curr_tour.route.size() == GRAPH_SIZE ) {
                int temp_cost = curr_tour.cost;
                curr_tour.Add_city( 0 );
                // for ( int i = 0; i < curr_tour.route.size(); i++ ) {
                //     cout << curr_tour.route[ i ] << " ";
                // }
                // cout << endl
                //      << curr_tour.cost << endl;
                //int cost = curr_tour.cost;
                if ( best.route.empty() || best.cost > curr_tour.cost ) {
                    best = curr_tour;
                }
                curr_tour.cost = temp_cost;
                curr_tour.route.pop_back();
                //cout << curr_tour.cost << endl;
                curr_tour.Remove_last_city();
                //cout << curr_tour.cost << endl;
            }
            else {
                s.push( NO_CITY );
                for ( city i = GRAPH_SIZE - 1; i >= 1; i-- ) {
                    if ( find( curr_tour.route.begin(), curr_tour.route.end(), i ) == curr_tour.route.end() && curr_tour.cost + graph[ curr_tour.route.back() ][ i ] < best.cost ) {
                        s.push( i );
                    }
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