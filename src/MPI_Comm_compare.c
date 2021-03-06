/* 
MPI_Comm_compare

   Compares two communicators
int MPI_Comm_compare(
  MPI_Comm comm1,
  MPI_Comm comm2,
  int *result
);

Parameters

   comm1
          [in] comm1 (handle)

   comm2
          [in] comm2 (handle)

   result
          [out] integer which is MPI_IDENT if the contexts and groups are
          the same, MPI_CONGRUENT if different contexts but identical
          groups, MPI_SIMILAR if different contexts but similar groups,
          and MPI_UNEQUAL otherwise

Remarks

   MPI_IDENT results if and only if comm1 and comm2 are handles for the
   same object (identical groups and same contexts). MPI_CONGRUENT results
   if the underlying groups are identical in constituents and rank order;
   these communicators differ only by context. MPI_SIMILAR results if the
   group members of both communicators are the same but the rank order
   differs. MPI_UNEQUAL results otherwise.
   Using 'MPI_COMM_NULL' with 'MPI_Comm_compare'

   It is an error to use MPI_COMM_NULL as one of the arguments to
   MPI_Comm_compare. The relevant sections of the MPI standard are

   .(2.4.1 Opaque Objects) A null handle argument is an erroneous IN
   argument in MPI calls, unless an exception is explicitly stated in the
   text that defines the function.

   .(5.4.1. Communicator Accessors) where there is no text in
   MPI_COMM_COMPARE allowing a null handle.

Thread and Interrupt Safety

   This routine is thread-safe. This means that this routine may be safely
   used by multiple threads without the need for any user-provided thread
   locks. However, the routine is not interrupt safe. Typically, this is
   due to the use of memory allocation routines such as malloc or other
   non-MPICH runtime routines that are themselves not interrupt-safe. (To
   perform the communicator comparisions, this routine may need to
   allocate some memory. Memory allocation is not interrupt-safe, and
   hence this routine is only thread-safe.)

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
    int size, dims[2], periods[2], remain[2];
    int result;
    MPI_Comm comm, newcomm;
    MPI_Init( &argc, &argv );
    /* First, create a 1-dim cartesian communicator */
    periods[0] = 0;
    MPI_Comm_size( MPI_COMM_WORLD, &size );
    dims[0] = size;
    MPI_Cart_create( MPI_COMM_WORLD, 1, dims, periods, 0, &comm );
    /* Now, extract a communicator with no dimensions */
    remain[0] = 0;
    MPI_Cart_sub( comm, remain, &newcomm );
    /* This should be congruent to MPI_COMM_SELF */
    MPI_Comm_compare( MPI_COMM_SELF, newcomm, &result );
    if (result != MPI_CONGRUENT) {
        errs++;
        printf( "cart sub to size 0 did not give self\n"
);fflush(stdout);
    }
    /* Free the new communicator */
    MPI_Comm_free( &newcomm );
    MPI_Comm_free( &comm );
    MPI_Finalize();
    return 0;
}

