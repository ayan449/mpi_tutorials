/* 
MPI_Unpublish_name

   Unpublish a service name published with MPI_Publish_name
int MPI_Unpublish_name(
  char *service_name,
  MPI_Info info,
  char *port_name
);

int MPI_Unpublish_name(
  wchar_t *service_name,
  MPI_Info info,
  wchar_t *port_name
);

Parameters

   service_name
          [in] a service name (string)

   info
          [in] implementation-specific information (handle)

   port_name
          [in] a port name (string)

Remarks

   This routine unpublishes a service name that has been previously
   published. Attempting to unpublish a name that has not been published
   or has already been unpublished is erroneous and is indicated by the
   error class MPI_ERR_SERVICE.

   All published names must be unpublished before the corresponding port
   is closed and before the publishing process exits. The behavior of
   MPI_UNPUBLISH_NAME is implementation dependent when a process tries to
   unpublish a name that it did not publish.

   If the info argument was used with MPI_PUBLISH_NAME to tell the
   implementation how to publish names, the implementation may require
   that info passed to MPI_UNPUBLISH_NAME contain information to tell the
   implementation how to unpublish a name.

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

   MPI_ERR_INFO
          Invalid Info

   MPI_ERR_ARG
          Invalid argument. Some argument is invalid and is not identified
          by a specific error class (e.g., MPI_ERR_RANK).

   MPI_ERR_OTHER
          Other error; use MPI_Error_string to get more information about
          this error code.

*/
 
// Copyright 2009 Deino Software. All rights reserved.
// Source: http://mpi.deino.net/mpi_functions/index.htm

#include "mpi.h"
#include <stdio.h>
#include <string.h>

int main( int argc, char *argv[] ) {
  int errs = 0;
  char port_name[MPI_MAX_PORT_NAME], port_name_out[MPI_MAX_PORT_NAME];
  char serv_name[256];
  int merr, mclass;
  char errmsg[MPI_MAX_ERROR_STRING];
  int msglen;
  int rank;

  MPI_Init( &argc, &argv );
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  /* Note that according to the MPI standard, port_name must have
     been created by MPI_Open_port. For this example we'll use a fake name. */
  strcpy( port_name, "otherhost:122" );
  strcpy( serv_name, "MyTest" );
  MPI_Comm_set_errhandler( MPI_COMM_WORLD, MPI_ERRORS_RETURN );
  if (rank == 0) {
    merr = MPI_Publish_name( serv_name, MPI_INFO_NULL, port_name );
    if (merr) {
      errs++;
      MPI_Error_string( merr, errmsg, &msglen );
      printf( "Error in Publish_name: \"%s\"\n", errmsg);
      fflush(stdout);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    merr = MPI_Unpublish_name( serv_name, MPI_INFO_NULL, port_name);
    if (merr) {
      errs++;
      MPI_Error_string( merr, errmsg, &msglen );
      printf( "Error in Unpublish name: \"%s\"\n", errmsg);
      fflush(stdout);
    }
  }
  else {
    MPI_Barrier(MPI_COMM_WORLD);
    merr = MPI_Lookup_name( serv_name, MPI_INFO_NULL, port_name_out);
    if (merr) {
      errs++;
      MPI_Error_string( merr, errmsg, &msglen );
      printf( "Error in Lookup name: \"%s\"\n", errmsg);
      fflush(stdout);
    }
    else {
      if (strcmp( port_name, port_name_out )) {
        errs++;
        printf( "Lookup name returned the wrong value (%s)\n", port_name_out );
	fflush(stdout);
      }
    }

    MPI_Barrier(MPI_COMM_WORLD);
  }

  MPI_Finalize();
  return 0;
}

