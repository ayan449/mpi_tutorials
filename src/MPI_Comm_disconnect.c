/* 
MPI_Comm_disconnect

   Disconnect from a communicator
int MPI_Comm_disconnect(
  MPI_Comm * comm
);

Parameters

   comm
          [in] communicator (handle)

Remarks

   This routine waits for all pending communication to complete, then
   frees the communicator and sets comm to MPI_COMM_NULL. It may not be
   called with MPI_COMM_WORLD or MPI_COMM_SELF.

   MPI_COMM_DISCONNECT may be called only if all communication is complete
   and matched, so that buffered data can be delivered to its destination.
   This requirement is the same as for MPI_FINALIZE.

   MPI_COMM_DISCONNECT has the same action as MPI_COMM_FREE, except that
   it waits for pending communication to finish internally and enables the
   guarantee about the behavior of disconnected processes.

   Advice to users.

   To disconnect two processes you may need to call MPI_COMM_DISCONNECT,
   MPI_WIN_FREE and MPI_FILE_CLOSE to remove all communication paths
   between the two processes. Notes that it may be necessary to disconnect
   several communicators (or to free several windows or files) before two
   processes are completely independent.

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

See Also

   MPI_Comm_connect, MPI_Comm_join

*/
 
// Copyright 2009 Deino Software. All rights reserved.
// Source: http://mpi.deino.net/mpi_functions/index.htm

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <windows.h> /* Sleep */
#include <unistd.h>

/* This test checks to make sure that two MPI_Comm_connects to two different MPI ports
* match their corresponding MPI_Comm_accepts. The root process opens two MPI ports and
* sends the first port to process 1 and the second to process 2. Then the root process
* accepts a connection from the second port followed by the first port.
* Processes 1 and 2 both connect back to the root but process 2 first sleeps for three
* seconds to give process 1 time to attempt to connect to the root. The root should wait
* until process 2 connects before accepting the connection from process 1.
*/

int main( int argc, char *argv[] )
{
    int num_errors = 0;
    int rank, size;
    char port1[MPI_MAX_PORT_NAME];
    char port2[MPI_MAX_PORT_NAME];
    MPI_Status status;
    MPI_Comm comm1, comm2;
    int data = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (size < 3)
    {
        printf("Three processes needed to run this test.\n");
	fflush(stdout);
        MPI_Finalize();
        return 0;
    }
    if (rank == 0)
    {
        printf("0: opening ports.\n");
	fflush(stdout);
        MPI_Open_port(MPI_INFO_NULL, port1);
        MPI_Open_port(MPI_INFO_NULL, port2);
        printf("opened port1: <%s>\n", port1);
        printf("opened port2: <%s>\n", port2);
	fflush(stdout);
        MPI_Send(port1, MPI_MAX_PORT_NAME, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
        MPI_Send(port2, MPI_MAX_PORT_NAME, MPI_CHAR, 2, 0, MPI_COMM_WORLD);
        printf("accepting port2.\n");
	fflush(stdout);
        MPI_Comm_accept(port2, MPI_INFO_NULL, 0, MPI_COMM_SELF, &comm2);
        printf("accepting port1.\n");
	fflush(stdout);
        MPI_Comm_accept(port1, MPI_INFO_NULL, 0, MPI_COMM_SELF, &comm1);
        MPI_Close_port(port1);
        MPI_Close_port(port2);
        printf("sending 1 to process 1.\n");
	fflush(stdout);
        data = 1;
        MPI_Send(&data, 1, MPI_INT, 0, 0, comm1);
        printf("sending 2 to process 2.\n");
	fflush(stdout);
        data = 2;
        MPI_Send(&data, 1, MPI_INT, 0, 0, comm2);
        MPI_Comm_disconnect(&comm1);
        MPI_Comm_disconnect(&comm2);
    }
    else if (rank == 1)
    {
        MPI_Recv(port1, MPI_MAX_PORT_NAME, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Comm_connect(port1, MPI_INFO_NULL, 0, MPI_COMM_SELF, &comm1);
        MPI_Recv(&data, 1, MPI_INT, 0, 0, comm1, &status);
        if (data != 1)
        {
            printf("Received %d from root when expecting 1\n", data);
            fflush(stdout);
            num_errors++;
        }
        MPI_Comm_disconnect(&comm1);
    }
    else if (rank == 2)
    {
        MPI_Recv(port2, MPI_MAX_PORT_NAME, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
        /* make sure process 1 has time to do the connect before this process attempts to connect */
        //Sleep(3000);
	sleep(1);
        MPI_Comm_connect(port2, MPI_INFO_NULL, 0, MPI_COMM_SELF, &comm2);
        MPI_Recv(&data, 1, MPI_INT, 0, 0, comm2, &status);
        if (data != 2)
        {
            printf("Received %d from root when expecting 2\n", data);
            fflush(stdout);
            num_errors++;
        }
        MPI_Comm_disconnect(&comm2);
    }
    
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    return 0;
}
