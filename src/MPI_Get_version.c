/* 
MPI_Get_version

   Return the version number of MPI
int MPI_Get_version(
  int *version,
  int *subversion
);

Parameters

   version
          [out] Version of MPI

   subversion
          [out] Subversion of MPI

Remarks

   In order to cope with changes to the MPI Standard, there are both
   compile-time and run-time ways to determine which version of the
   standard is in use in the environment one is using.

   The "version" will be represented by two separate integers, for the
   version and subversion:

   In C and C++,

    #define MPI_VERSION    1
    #define MPI_SUBVERSION 2

   in Fortran,

    INTEGER MPI_VERSION, MPI_SUBVERSION
    PARAMETER (MPI_VERSION    = 1)
    PARAMETER (MPI_SUBVERSION = 2)

   For runtime determination, MPI_GET_VERSION returns these two numbers.

   MPI_GET_VERSION is one of the few functions that can be called before
   MPI_INIT and after MPI_FINALIZE.

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

*/
 
// Copyright 2009 Deino Software. All rights reserved.
// Source: http://mpi.deino.net/mpi_functions/index.htm

#include<mpi.h>
#include<stdio.h>

int main(int argc, char *argv[])
{
    int version, sub_version;

    MPI_Init(&argc, &argv);

    MPI_Get_version(&version, &sub_version);
    printf("MPI Version %d.%d\n", version, sub_version);fflush(stdout);

    MPI_Finalize();
    return 0;
}
