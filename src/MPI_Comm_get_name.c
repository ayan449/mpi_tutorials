/* 
MPI_Comm_get_name

   Return the print name from the communicator
int MPI_Comm_get_name(
  MPI_Comm comm,
  char *comm_name,
  int *resultlen
);

int MPI_Comm_get_name(
  MPI_Comm comm,
  wchar_t *comm_name,
  int *resultlen
);

Parameters

   comm
          [in] Communicator to get name of (handle)

   comm_name
          [out] On output, contains the name of the communicator. It must
          be an array of size at least MPI_MAX_OBJECT_NAME.

   resultlen
          [out] Number of characters in name

Remarks

   MPI_COMM_GET_NAME returns the last name which has previously been
   associated with the given communicator. The name may be set and got
   from any language. The same name will be returned independent of the
   language used. name should be allocated so that it can hold a resulting
   string of length MPI_MAX_OBJECT_NAME characters. MPI_COMM_GET_NAME
   returns a copy of the set name in name.

   If the user has not associated a name with a communicator, or an error
   occurs, MPI_COMM_GET_NAME will return an empty string (all spaces in
   Fortran, "" in C and C++). The three predefined communicators will have
   predefined names associated with them. Thus, the names of
   MPI_COMM_WORLD, MPI_COMM_SELF, and MPI_COMM_PARENT will have the
   default of MPI_COMM_WORLD, MPI_COMM_SELF, and MPI_COMM_PARENT. The fact
   that the system may have chosen to give a default name to a
   communicator does not prevent the user from setting a name on the same
   communicator; doing this removes the old name and assigns the new one.

   Because MPI specifies that null objects (e.g., MPI_COMM_NULL) are
   invalid as input to MPI routines unless otherwise specified, using
   MPI_COMM_NULL as input to this routine is an error.

   Rationale..

   We provide separate functions for setting and getting the name of a
   communicator, rather than simply providing a predefined attribute key
   for the following reasons:
     * It is not, in general, possible to store a string as an attribute
       from Fortran.

     * It is not easy to set up the delete function for a string attribute
       unless it is known to have been allocated from the heap.

     * To make the attribute key useful additional code to call strdup is
       necessary. If this is not standardized then users have to write it.
       This is extra unneeded work which we can easily eliminate.

     * The Fortran binding is not trivial to write (it will depend on
       details of the Fortran compilation system), and will not be
       portable. Therefore it should be in the library rather than in user
       code.

   Advice to users.

   The above definition means that it is safe simply to print the string
   returned by MPI_COMM_GET_NAME, as it is always a valid string even if
   there was no name.

   Note that associating a name with a communicator has no effect on the
   semantics of an MPI program, and will (necessarily) increase the store
   requirement of the program, since the names must be saved. Therefore
   there is no requirement that users use these functions to associate
   names with communicators. However debugging and profiling MPI
   applications may be made easier if names are associated with
   communicators, since the debugger or profiler should then be able to
   present information in a less cryptic manner.

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
    MPI_Comm comm;
    int rlen;
    char name[MPI_MAX_OBJECT_NAME], nameout[MPI_MAX_OBJECT_NAME];
    MPI_Init( &argc, &argv );
    nameout[0] = 0;
    MPI_Comm_get_name( MPI_COMM_WORLD, nameout, &rlen );
    if (strcmp(nameout,"MPI_COMM_WORLD")) {
        printf( "Name of comm world is %s, should be MPI_COMM_WORLD\n",
nameout );fflush(stdout);
    }
    nameout[0] = 0;
    MPI_Comm_get_name( MPI_COMM_SELF, nameout, &rlen );
    if (strcmp(nameout,"MPI_COMM_SELF")) {
        printf( "Name of comm self is %s, should be MPI_COMM_SELF\n",
nameout );fflush(stdout);
    }
    MPI_Finalize();
    return 0;
}

