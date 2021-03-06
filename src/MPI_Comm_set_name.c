/* 
MPI_Comm_set_name

   Sets the print name for a communicator
int MPI_Comm_set_name(
  MPI_Comm comm,
  char *comm_name
);

int MPI_Comm_set_name(
  MPI_Comm comm,
  wchar_t *comm_name
);

Parameters

   comm
          [in] communicator to name (handle)

   comm_name
          [in] Name for communicator (string)

Remarks

   MPI_COMM_SET_NAME allows a user to associate a name string with a
   communicator. The character string which is passed to MPI_COMM_SET_NAME
   will be saved inside the MPI library (so it can be freed by the caller
   immediately after the call, or allocated on the stack). Leading spaces
   in name are significant but trailing ones are not.

   MPI_COMM_SET_NAME is a local (non-collective) operation, which only
   affects the name of the communicator as seen in the process which made
   the MPI_COMM_SET_NAME call. There is no requirement that the same (or
   any) name be assigned to a communicator in every process where it
   exists.

   Advice to users.

   Since MPI_COMM_SET_NAME is provided to help debug code, it is sensible
   to give the same name to a communicator in all of the processes where
   it exists, to avoid confusion.

   The length of the name which can be stored is limited to the value of
   MPI_MAX_OBJECT_NAME in Fortran and MPI_MAX_OBJECT_NAME-1 in C and C++
   to allow for the null terminator. Attempts to put names longer than
   this will result in truncation of the name. MPI_MAX_OBJECT_NAME must
   have a value of at least 64.

   Advice to users.

   Under circumstances of store exhaustion an attempt to put a name of any
   length could fail, therefore the value of MPI_MAX_OBJECT_NAME should be
   viewed only as a strict upper bound on the name length, not a guarantee
   that setting names of less than this length will always succeed.

Thread and Interrupt Safety

   This routine is thread and interrupt safe only if no MPI routine that
   updates or frees the same MPI object may be called concurrently with
   this routine.

   The MPI standard defined a thread-safe interface but this does not mean
   that all routines may be called without any thread locks. For example,
   two threads must not attempt to change the contents of the same
   MPI_Info object concurrently. The user is responsible in this case for
   using some mechanism, such as thread locks, to ensure that only one
   thread at a time makes use of this routine.

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

   MPI_ERR_COMM
          Invalid communicator. A common error is to use a null
          communicator in a call (not even allowed in MPI_Comm_rank).

*/
 
// Copyright 2009 Deino Software. All rights reserved.
// Source: http://mpi.deino.net/mpi_functions/index.htm

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
int main( int argc, char *argv[] )
{
    int errs = 0;
    MPI_Comm comm;
    int cnt, rlen;
    char name[MPI_MAX_OBJECT_NAME], nameout[MPI_MAX_OBJECT_NAME];
    MPI_Init( &argc, &argv );
    nameout[0] = 0;
    MPI_Comm_get_name( MPI_COMM_WORLD, nameout, &rlen );
    printf( "Name of comm world is: %s\n", nameout );
    fflush(stdout);
    strcpy( name, "comm-0" );
    MPI_Comm_set_name( MPI_COMM_WORLD, name );
    nameout[0] = 0;
    MPI_Comm_get_name( MPI_COMM_WORLD, nameout, &rlen );
    printf( "Now the name of comm world is: %s\n", nameout );
    fflush(stdout);
    MPI_Finalize();
    return 0;
}

