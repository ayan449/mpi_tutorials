/* 
MPI_Finalize

   Terminates MPI execution environment
int MPI_Finalize( void );

Remarks

   This routines cleans up all MPI state. Once this routine is called, no
   MPI routine (even MPI_INIT) may be called. The user must ensure that
   all pending communications involving a process completes before the
   process calls MPI_FINALIZE.
   All processes must call this routine before exiting. The number of
   processes running after this routine is called is undefined; it is best
   not to perform much more than a return rc after calling MPI_Finalize.

Thread and Signal Safety

   The MPI standard requires that MPI_Finalize be called only by the same
   thread that initialized MPI with either MPI_Init or MPI_Init_thread.

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

*/
 
// Copyright 2009 Deino Software. All rights reserved.
// Source: http://mpi.deino.net/mpi_functions/index.htm

#include "mpi.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    int rank, nprocs;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    printf("Hello, world.  I am %d of %d\n", rank, nprocs);
    fflush(stdout);
    MPI_Finalize();
    return 0;
}

