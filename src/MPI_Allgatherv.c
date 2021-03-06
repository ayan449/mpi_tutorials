/* 
MPI_Allgatherv

   Gathers data from all tasks and deliver the combined data to all tasks
int MPI_Allgatherv(
  void *sendbuf,
  int sendcount,
  MPI_Datatype sendtype,
  void *recvbuf,
  int *recvcounts,
  int *displs,
  MPI_Datatype recvtype,
  MPI_Comm comm
);

Parameters

   sendbuf
          [in] starting address of send buffer (choice)

   sendcount
          [in] number of elements in send buffer (integer)

   sendtype
          [in] data type of send buffer elements (handle)

   recvbuf

                [out] address of receive buffer (choice)

   recvcounts
          [in] integer array (of length group size) containing the number
          of elements that are received from each process

   displs
          [in] integer array (of length group size). Entry i specifies the
          displacement (relative to recvbuf ) at which to place the
          incoming data from process i

   recvtype
          [in] data type of receive buffer elements (handle)

   comm
          [in] communicator (handle)

Remarks

   The MPI standard (1.0 and 1.1) says that

       The jth block of data sent from each proess is received by every
   process and placed in the jth block of the buffer recvbuf.

   This is misleading; a better description is

       The block of data sent from the jth process is received by every
   process and placed in the jth block of the buffer recvbuf.

   MPI_ALLGATHERV can be thought of as MPI_GATHERV, but where all
   processes receive the result, instead of just the root. The jth block
   of data sent from each process is received by every process and placed
   in the jth block of the buffer recvbuf. These blocks need not all be
   the same size.

   The type signature associated with sendcount, sendtype, at process j
   must be equal to the type signature associated with recvcounts[j],
   recvtype at any other process.

   The outcome is as if all processes executed calls to

MPI_GATHERV(sendbuf,sendcount,sendtype,recvbuf,recvcounts,displs, recvtype,root,
comm),

   for root = 0 , ..., n-1. The rules for correct usage of MPI_ALLGATHERV
   are easily found from the corresponding rules for MPI_GATHERV.

   The "in place" option for intracommunicators is specified by passing
   the value MPI_IN_PLACE to the argument sendbuf at all processes.
   sendcount and sendtype are ignored. Then the input data of each process
   is assumed to be in the area where that process would receive its own
   contribution to the receive buffer. Specifically, the outcome of a call
   to MPI_ALLGATHER in the "in place" case is as if all processes executed
   n calls to

    MPI_GATHERV( MPI_IN_PLACE, 0, MPI_DATATYPE_NULL, recvbuf, recvcounts, displs
, recvtype, root, comm )

   for root = 0, ..., n - 1.

   If comm is an intercommunicator, then each process in group A
   contributes a data item; these items are concatenated and the result is
   stored at each process in group B. Conversely the concatenation of the
   contributions of the processes in group B is stored at each process in
   group A. The send buffer arguments in group A must be consistent with
   the receive buffer arguments in group B, and vice versa.

Thread and Interrupt Safety

   This routine is thread-safe. This means that this routine may be safely
   used by multiple threads without the need for any user-provided thread
   locks. However, the routine is not interrupt safe. Typically, this is
   due to the use of memory allocation routines such as malloc or other
   non-MPICH runtime routines that are themselves not interrupt-safe.

Notes for Fortran

   All MPI routines in Fortran (except for MPI_WTIME and MPI_WTICK) have
   an additional argument ierr at the end of the argument list. ierr is an
   integer and has the same meaning as the return value of the routine in
   C. In Fortran, MPI routines are subroutines, and are invoked with the
   call statement.

   All MPI objects (e.g., MPI_Datatype, MPI_Comm) are of type INTEGER in
   Fortran.

Errors

   All MPI routines (except MPI_Wtime and MPI_Wtick) return an error
   value; C routines as the value of the function and Fortran routines in
   the last argument. Before the value is returned, the current MPI error
   handler is called. By default, this error handler aborts the MPI job.
   The error handler may be changed with MPI_Comm_set_errhandler (for
   communicators), MPI_File_set_errhandler (for files), and
   MPI_Win_set_errhandler (for RMA windows). The MPI-1 routine
   MPI_Errhandler_set may be used but its use is deprecated. The
   predefined error handler MPI_ERRORS_RETURN may be used to cause error
   values to be returned. Note that MPI does not guarentee that an MPI
   program can continue past an error; however, MPI implementations will
   attempt to continue whenever possible.

   MPI_ERR_BUFFER
          Invalid buffer pointer. Usually a null buffer where one is not
          valid.

   MPI_ERR_COUNT
          Invalid count argument. Count arguments must be non-negative; a
          count of zero is often valid.

   MPI_ERR_TYPE
          Invalid datatype argument. May be an uncommitted MPI_Datatype
          (see MPI_Type_commit).

*/
 
// Copyright 2009 Deino Software. All rights reserved.
// Source: http://mpi.deino.net/mpi_functions/index.htm

#include "mpi.h"
#include <stdio.h>

#define MAX_PROCESSES 10

int main( int argc, char **argv )
{
    int rank, size, i,j;
    int table[MAX_PROCESSES][MAX_PROCESSES];
    int errors=0;
    int participants;
    int displs[MAX_PROCESSES];
    int recv_counts[MAX_PROCESSES];
    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );
    /* A maximum of MAX_PROCESSES processes can participate */
    if ( MAX_PROCESSES % size || size > MAX_PROCESSES )
    {
        fprintf( stderr, "Number of processors must divide into %d\n", MAX_PROCESSES );
	fflush(stderr);
        MPI_Abort( MPI_COMM_WORLD, 1 );
    }
    participants = size;
    if ( (rank < participants) )
    {
        /* Determine what rows are my responsibility */
        int block_size = MAX_PROCESSES / participants;
        int begin_row = rank * block_size;
        int end_row = (rank+1) * block_size;
        int send_count = block_size * MAX_PROCESSES;
        /* Fill in the displacements and recv_counts */
        for (i=0; i<participants; i++) {
            displs[i] = i * block_size * MAX_PROCESSES;
            recv_counts[i] = send_count;
        }
        /* Paint my rows my color */
        for (i=begin_row; i<end_row ;i++)
            for (j=0; j<MAX_PROCESSES; j++)
                table[i][j] = rank + 10;
        /* Everybody gets the gathered data */
        MPI_Allgatherv(&table[begin_row][0], send_count, MPI_INT,
                            &table[0][0], recv_counts, displs,
                            MPI_INT, MPI_COMM_WORLD);
        /* Everybody should have the same table now.
            The entries are:
            Table[i][j] = (i/block_size) + 10;
        */
        for (i=0; i<MAX_PROCESSES;i++)
            if ( (table[i][0] - table[i][MAX_PROCESSES-1] !=0) )
                errors++;
        for (i=0; i<MAX_PROCESSES;i++) {
            for (j=0; j<MAX_PROCESSES;j++) {
                if (table[i][j] != (i/block_size) + 10) errors++;
            }
        }
        if (errors) {
            /* Print out table if there are any errors */
            for (i=0; i<MAX_PROCESSES;i++) {
                printf("\n");
                for (j=0; j<MAX_PROCESSES; j++)
                    printf(" %d",table[i][j]);
            }
            printf("\n");
            fflush(stdout);
        }
    }
    
    MPI_Finalize();
    if (errors)
    {
        printf( "[%d] done with ERRORS(%d)!\n", rank, errors);
	fflush(stdout);
    }
    return errors;
}

