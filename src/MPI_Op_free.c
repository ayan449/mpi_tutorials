/* 
MPI_Op_free

   Frees a user-defined combination function handle
int MPI_Op_free(
  MPI_Op *op
);

Parameters

   op
          [in] operation (handle)

Remarks

   Marks a user-defined reduction operation for deallocation and sets op
   to MPI_OP_NULL on exit.

   Null Handles The MPI 1.1 specification, in the section on opaque
   objects, explicitly disallows freeing a null MPI_Op. The text from the
   standard is:
 A null handle argument is an erroneous IN argument in MPI calls, unless an
 exception is explicitly stated in the text that defines the function. Such
 exception is allowed for handles to request objects in Wait and Test calls
 (sections Communication Completion and Multiple Completions ). Otherwise, a
 null handle can only be passed to a function that allocates a new object and
 returns a reference to it in the handle.

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

   MPI_SUCCESS
          No error; MPI routine completed successfully.

   MPI_ERR_ARG
          Invalid argument. Some argument is invalid and is not identified
          by a specific error class (e.g., MPI_ERR_RANK).

   MPI_ERR_ARG
          Invalid argument; the error code associated with this error
          indicates an attempt to free an MPI permanent operation (e.g.,
          MPI_SUM).

See Also

   MPI_Op_create

*/
 
// Copyright 2009 Deino Software. All rights reserved.
// Source: http://mpi.deino.net/mpi_functions/index.htm


#include "mpi.h"
#include <stdio.h>

void addem ( int *, int *, int *, MPI_Datatype * );

void addem(int *invec, int *inoutvec, int *len, MPI_Datatype *dtype)
{
    int i;
    for ( i=0; i<*len; i++ )
        inoutvec[i] += invec[i];
}

int main( int argc, char **argv )
{
    int rank, size, i;
    int data;
    int errors=0;
    int result = -100;
    int correct_result;
    MPI_Op op;

    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );

    data = rank;
    MPI_Op_create( (MPI_User_function *)addem, 1, &op );
    MPI_Reduce ( &data, &result, 1, MPI_INT, op, 0, MPI_COMM_WORLD );
    MPI_Bcast ( &result, 1, MPI_INT, 0, MPI_COMM_WORLD );
    MPI_Op_free( &op );
    correct_result = 0;
    for(i=0;i<size;i++)
        correct_result += i;
    if (result != correct_result) errors++;

    MPI_Finalize();
    return errors;
}

