/* 
MPI_Bcast

   Broadcasts a message from the process with rank "root" to all other
   processes of the communicator
int MPI_Bcast(
  void *buffer,
  int count,
  MPI_Datatype datatype,
  int root,
  MPI_Comm comm
);

Parameters

   buffer
          [in/out] starting address of buffer (choice)

   count
          [in] number of entries in buffer (integer)

   datatype
          [in] data type of buffer (handle)

   root
          [in] rank of broadcast root (integer)

   comm
          [in] communicator (handle)

Remarks

   MPI_BCAST broadcasts a message from the process with rank root to all
   processes of the group, itself included. It is called by all members of
   group using the same arguments for comm, root. On return, the contents
   of root's communication buffer has been copied to all processes.

   General, derived datatypes are allowed for datatype. The type signature
   of count, datatype on any process must be equal to the type signature
   of count, datatype at the root. This implies that the amount of data
   sent must be equal to the amount received, pairwise between each
   process and the root. MPI_BCAST and all other data-movement collective
   routines make this restriction. Distinct type maps between sender and
   receiver are still allowed.

   The "in place" option is not meaningful here.

   If comm is an intercommunicator, then the call involves all processes
   in the intercommunicator, but with one group (group A) defining the
   root process. All processes in the other group (group B) pass the same
   value in argument root, which is the rank of the root in group A. The
   root passes the value MPI_ROOT in root. All other processes in group A
   pass the value MPI_PROC_NULL in root. Data is broadcast from the root
   to all processes in group B. The receive buffer arguments of the
   processes in group B must be consistent with the send buffer argument
   of the root.

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

   MPI_ERR_COMM
          Invalid communicator. A common error is to use a null
          communicator in a call (not even allowed in MPI_Comm_rank).

   MPI_ERR_COUNT
          Invalid count argument. Count arguments must be non-negative; a
          count of zero is often valid.

   MPI_ERR_TYPE
          Invalid datatype argument. May be an uncommitted MPI_Datatype
          (see MPI_Type_commit).

   MPI_ERR_BUFFER
          Invalid buffer pointer. Usually a null buffer where one is not
          valid.

   MPI_ERR_ROOT
          Invalid root. The root must be specified as a rank in the
          communicator. Ranks must be between zero and the size of the
          communicator minus one.

*/
 
// Copyright 2009 Deino Software. All rights reserved.
// Source: http://mpi.deino.net/mpi_functions/index.htm

#include "mpi.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ROOT 0
#define NUM_REPS 5
#define NUM_SIZES 3

int main( int argc, char **argv)
{
    int *buf;
    int i, rank, reps, n;
    int bVerify = 1;
    int sizes[NUM_SIZES] = { 100, 64*1024, 128*1024 };
    int num_errors=0, tot_errors;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (argc > 1)
    {
        if (strcmp(argv[1], "-novalidate") == 0 || strcmp(argv[1],"-noverify") == 0) bVerify = 0;
    }
    
    buf = (int *) malloc(sizes[NUM_SIZES-1]*sizeof(int));
    memset(buf, 0, sizes[NUM_SIZES-1]*sizeof(int));

    for (n=0; n<NUM_SIZES; n++)
    {
        if (rank == ROOT)
        {
            printf("bcasting %d MPI_INTs %d times\n", sizes[n], NUM_REPS);
            fflush(stdout);
        }
	
        for (reps=0; reps < NUM_REPS; reps++)
        {
            if (bVerify)
            {
                if (rank == ROOT)
                {
                    for (i=0; i<sizes[n]; i++)
                    {
                        buf[i] = 1000000 * (n * NUM_REPS + reps) + i;
                    }
                }
                else
                {
                    for (i=0; i<sizes[n]; i++)
                    {
                        buf[i] = -1 - (n * NUM_REPS + reps);
                    }
                }
            }
            MPI_Bcast(buf, sizes[n], MPI_INT, ROOT, MPI_COMM_WORLD);
            if (bVerify)
            {
                num_errors = 0;
                for (i=0; i<sizes[n]; i++)
                {
                    if (buf[i] != 1000000 * (n * NUM_REPS + reps) + i)
                    {
                        num_errors++;
                        if (num_errors < 10)
                        {
                            printf("Error: Rank=%d, n=%d, reps=%d, i=%d, buf[i]=%d expected=%d\n",
				   rank, n, reps, i, buf[i],1000000 * (n * NUM_REPS + reps) +i);
                            fflush(stdout);
                        }
                    }
                }
                if (num_errors >= 10)
                {
                    printf("Error: Rank=%d, num_errors = %d\n", rank, num_errors);
                    fflush(stdout);
                }
            }
        }
    }
    
    MPI_Reduce( &num_errors, &tot_errors, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD );
    if (rank == 0 && tot_errors == 0)  printf(" No Errors\n");

    fflush(stdout);
    free(buf);

    MPI_Finalize();
    return 0;
}

