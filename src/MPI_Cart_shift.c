/* 
MPI_Cart_shift

   Returns the shifted source and destination ranks, given a shift
   direction and amount
int MPI_Cart_shift(
  MPI_Comm comm,
  int direction,
  int displ,
  int *source,
  int *dest
);

Parameters

   comm
          [in] communicator with cartesian structure (handle)

   direction
          [in] coordinate dimension of shift (integer)

   displ
          [in] displacement (> 0: upwards shift, < 0: downwards shift)
          (integer)

   source
          [out] rank of source process (integer)

   dest
          [out] rank of destination process (integer)

Remarks

   If the process topology is a cartesian structure, a MPI_SENDRECV
   operation is likely to be used along a coordinate direction to perform
   a shift of data. As input, MPI_SENDRECV takes the rank of a source
   process for the receive, and the rank of a destination process for the
   send. If the function MPI_CART_SHIFT is called for a cartesian process
   group, it provides the calling process with the above identifiers,
   which then can be passed to MPI_SENDRECV. The user specifies the
   coordinate direction and the size of the step (positive or negative).
   The function is local.

   The direction argument indicates the dimension of the shift, i.e., the
   coordinate which value is modified by the shift. The coordinates are
   numbered from 0 to ndims-1, when ndims is the number of dimensions.

   Depending on the periodicity of the cartesian group in the specified
   coordinate direction, MPI_CART_SHIFT provides the identifiers for a
   circular or an end-off shift. In the case of an end-off shift, the
   value MPI_PROC_NULL may be returned in rank_source or rank_dest,
   indicating that the source or the destination for the shift is out of
   range.

   Advice to users.

   In Fortran, the dimension indicated by DIRECTION = i has DIMS(i+1)
   nodes, where DIMS is the array that was used to create the grid. In C,
   the dimension indicated by direction = i is the dimension specified by
   dims[i].

Thread and Interrupt Safety

   This routine is both thread- and interrupt-safe. This means that this
   routine may safely be used by multiple threads and from within a signal
   handler.

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

   MPI_SUCCESS
          No error; MPI routine completed successfully.

   MPI_ERR_TOPOLOGY
          Invalid topology. Either there is no topology associated with
          this communicator, or it is not the correct type (e.g., MPI_CART
          when expecting MPI_GRAPH).

   MPI_ERR_COMM
          Invalid communicator. A common error is to use a null
          communicator in a call (not even allowed in MPI_Comm_rank).

   MPI_ERR_ARG
          Invalid argument. Some argument is invalid and is not identified
          by a specific error class (e.g., MPI_ERR_RANK).

*/
 
// Copyright 2009 Deino Software. All rights reserved.
// Source: http://mpi.deino.net/mpi_functions/index.htm

#include "mpi.h"
#include <stdio.h>
int main( int argc, char *argv[] )
{
    int errs = 0;
    int size, rank;
    int source, dest;
    int dims[2], periods[2];
    MPI_Comm comm;
    MPI_Init( &argc, &argv );
    MPI_Comm_size( MPI_COMM_WORLD, &size );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    dims[0] = size;
    periods[0] = 1;
    MPI_Cart_create( MPI_COMM_WORLD, 1, dims, periods, 0, &comm );
    MPI_Cart_shift( comm, 0, 1, &source, &dest );
    if (source != ((rank - 1 + size) % size)) {
        errs++;
        printf( "source for shift 1 is %d\n", source );fflush(stdout);
    }
    if (dest != ((rank + 1) % size)) {
        errs++;
        printf( "dest for shift 1 is %d\n", dest );fflush(stdout);
    }
    MPI_Cart_shift( comm, 0, 0, &source, &dest );
    if (source != rank) {
        errs++;
        printf( "Source for shift 0 is %d\n", source );fflush(stdout);
    }
    if (dest != rank) {
        errs++;
        printf( "Dest for shift 0 is %d\n", dest );fflush(stdout);
    }
    MPI_Cart_shift( comm, 0, -1, &source, &dest );
    if (source != ((rank + 1) % size)) {
        errs++;
        printf( "source for shift -1 is %d\n", source );fflush(stdout);
    }
    if (dest != ((rank - 1 + size) % size)) {
        errs++;
        printf( "dest for shift -1 is %d\n", dest );fflush(stdout);
    }
    /* Now, with non-periodic */
    MPI_Comm_free( &comm );
    periods[0] = 0;
    MPI_Cart_create( MPI_COMM_WORLD, 1, dims, periods, 0, &comm );
    MPI_Cart_shift( comm, 0, 1, &source, &dest );
    if ((rank > 0 && source != (rank - 1)) || (rank == 0 && source !=
MPI_PROC_NULL)) {
        errs++;
        printf( "source for non-periodic shift 1 is %d\n", source
);fflush(stdout);
    }
    if ((rank < size-1 && dest != rank + 1) || ((rank == size-1) &&
dest != MPI_PROC_NULL)) {
        errs++;
        printf( "dest for non-periodic shift 1 is %d\n", dest
);fflush(stdout);
    }
    MPI_Cart_shift( comm, 0, 0, &source, &dest );
    if (source != rank) {
        errs++;
        printf( "Source for non-periodic shift 0 is %d\n", source
);fflush(stdout);
    }
    if (dest != rank) {
        errs++;
        printf( "Dest for non-periodic shift 0 is %d\n", dest
);fflush(stdout);
    }
    MPI_Cart_shift( comm, 0, -1, &source, &dest );
    if ((rank < size - 1 && source != rank + 1) || (rank == size - 1 &&
source != MPI_PROC_NULL)) {
        errs++;
        printf( "source for non-periodic shift -1 is %d\n", source
);fflush(stdout);
    }
    if ((rank > 0 && dest != rank - 1) || (rank == 0 && dest !=
MPI_PROC_NULL)) {
        errs++;
        printf( "dest for non-periodic shift -1 is %d\n", dest
);fflush(stdout);
    }
    MPI_Comm_free( &comm );
    MPI_Finalize();
    return 0;
}

