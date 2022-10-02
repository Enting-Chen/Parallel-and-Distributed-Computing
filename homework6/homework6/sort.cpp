#include <cstdio>
#include <cstring>
#include <algorithm>
#include <mpi.h>
using namespace std;

#define GLOBAL_N 20

void Generate_list(int local_A[], int local_n, int my_rank) {
    srandom(my_rank+1);
    for (int i = 0; i < local_n; i++){
       local_A[i] = random() % 200;
    }   
} 

void Print_list(int local_A[], int local_n, int rank) {
   for (int i = 0; i < local_n; i++){
      printf("%d ", local_A[i]);
   }  
   printf("\n");
}  

void Print_global_list(int local_A[], int local_n, int my_rank, int p, 
      MPI_Comm comm) {
   int* A;

   if (my_rank == 0) {
      A = new int[p*local_n];
      MPI_Gather(local_A, local_n, MPI_INT, A, local_n, MPI_INT, 0,
            comm);
      printf("\nGlobal list:\n");
      Print_list(A, p*local_n, my_rank);
      delete[] A;
   } 
   else {
      MPI_Gather(local_A, local_n, MPI_INT, A, local_n, MPI_INT, 0,
            comm);
   }
} 

void Odd_even_iter(int local_A[], int temp_B[], int temp_C[],
        int local_n, int phase, int even_partner, int odd_partner,
        int my_rank, int p, MPI_Comm comm) {
   MPI_Status status;

   if (phase % 2 == 0) {
      if (even_partner >= 0) {
         MPI_Sendrecv(local_A, local_n, MPI_INT, even_partner, 0, 
            temp_B, local_n, MPI_INT, even_partner, 0, comm,
            &status);
         merge(local_A, local_A+local_n, temp_B, temp_B+local_n, temp_C);
         if (my_rank % 2 != 0){
            copy(temp_C+local_n, temp_C+local_n*2, local_A);
         }
         else{
            copy(temp_C, temp_C+local_n, local_A);
         }
      }
   } else { 
      if (odd_partner >= 0) {
         MPI_Sendrecv(local_A, local_n, MPI_INT, odd_partner, 0, 
            temp_B, local_n, MPI_INT, odd_partner, 0, comm,
            &status);
         merge(local_A, local_A+local_n, temp_B, temp_B+local_n, temp_C);
         if (my_rank % 2 != 0){
            copy(temp_C, temp_C+local_n, local_A);
         }
         else{
            copy(temp_C+local_n, temp_C+2*local_n, local_A);
         }
      }
   }
}  

void Sort(int local_A[], int local_n, int my_rank, 
         int p, MPI_Comm comm) {
   int phase;
   int *temp_B, *temp_C;
   int even_partner;  /* phase is even or left-looking */
   int odd_partner;   /* phase is odd or right-looking */

   /* Temporary storage used in merge-split */
   temp_B = new int[local_n];
   temp_C = new int[local_n*2];

   /* Find partners:  negative rank => do nothing during phase */
   if (my_rank % 2 != 0) {
      even_partner = my_rank - 1;
      odd_partner = my_rank + 1;
      if (odd_partner == p) odd_partner = MPI_PROC_NULL;  // Idle during odd phase
   } else {
      even_partner = my_rank + 1;
      if (even_partner == p) even_partner = MPI_PROC_NULL;  // Idle during even phase
      odd_partner = my_rank-1;  
   }

   /* Sort local list using built-in sort */
   sort(local_A, local_A+local_n);

   for (phase = 0; phase < p; phase++)
      Odd_even_iter(local_A, temp_B, temp_C, local_n, phase, 
             even_partner, odd_partner, my_rank, p, comm);

   delete [] temp_B;
   delete [] temp_C;
}  /* Sort */

int main(int argc, char* argv[]) {
   int my_rank, p;
   
   MPI_Comm comm;

   MPI_Init(&argc, &argv);
   comm = MPI_COMM_WORLD;
   MPI_Comm_size(comm, &p);
   MPI_Comm_rank(comm, &my_rank);
   int local_n = GLOBAL_N/p;

   int * local_A = new int[local_n];
   Generate_list(local_A, local_n, my_rank);
   for ( int i = 0; i < p; i++ ){
      if ( i == my_rank ){
         printf("%d: ", my_rank);
         Print_list(local_A, local_n, my_rank);
      }
      MPI_Barrier(comm);
   }

   Sort(local_A, local_n, my_rank, p, comm);

   Print_global_list(local_A, local_n, my_rank, p, comm);

   delete [] local_A;

   MPI_Finalize();

   return 0;
}  /* main */

