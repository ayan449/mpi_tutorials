/* 
MPI_Comm_spawn

   Spawn up to maxprocs instances of a single MPI application
int MPI_Comm_spawn(
  char *command,
  char *argv[],
  int maxprocs,
  MPI_Info info,
  int root,
  MPI_Comm comm,
  MPI_Comm *intercomm,
  int array_of_errcodes[]
);

int MPI_Comm_spawn(
  wchar_t *command,
  wchar_t *argv[],
  int maxprocs,
  MPI_Info info,
  int root,
  MPI_Comm comm,
  MPI_Comm *intercomm,
  int array_of_errcodes[]
);

Parameters

   command
          [in] name of program to be spawned (string, significant only at
          root)

   argv
          [in] arguments to command (array of strings, significant only at
          root)

   maxprocs
          [in] maximum number of processes to start (integer, significant
          only at root)

   info
          [in] a set of key-value pairs telling the runtime system where
          and how to start the processes (handle, significant only at
          root)

   root
          [in] rank of process in which previous arguments are examined
          (integer)

   comm
          [in] intracommunicator containing group of spawning processes
          (handle)

   intercomm
          [out] intercommunicator between original group and the newly
          spawned group (handle)

   array_of_errcodes
          [out] one code per process (array of integer)

Info argument

   DeinoMPI uses the info argument to specify additional control
   parameters to the spawn command.  Here are the supported keys:
     * path - search path to locate the executable. Use semicolons (;) to
       separate paths.  Example: c:\temp;c:\bin
     * host - host name to launch the processes on.  All processes will be
       launched on this host.
     * wdir - working directory
     * env - list of environment variables in the form: env=var env2=var2
       ...  Variables with spaces or = characters in them should be
       quoted.  The quote and escape characters need to be escaped within
       quoted strings.  For example: name="John Doe" random="He said,
       \"put it in c:\\temp\""
     * hosts - list of hosts where to deposit processes.  The form is:
       hostA[:n] hostB[:m] where :n is an optional way to deposit more
       than one process per host.  Example: hostA hostB:2 hostC.  If count
       = 5 the processes will be deposited as follows: hostA hostB hostB
       hostC hostA
     * machinefile - file to be read to create a host list.  The format of
       the file is one host per line with blank lines and lines beginning
       with # ignored.  Multiple processes per host can be specified by
       specifying the host name as follows: hostA:n.  Hosts are selected
       round robin from the list until count number of processes are
       launched.
     * map - list of network drives to map before launching the
       processes.  The format is: drive:\\share;drive2:\\share2.  Example:
       z:\\myserver\home\userA;y:\\myserver\data
     * localonly - launch the processes only on the host that performs the
       spawn.  This is the host that executes mpiexec, not the host of the
       root process that called spawn.
     * priority - set the priority of the launched processes.  Format:
       priority class[:thread priority].  You can specify the process
       priority and optionally the thread priority.  The classes are:
       0,1,2,3,4 (idle, below, normal, above, high).  The thread
       priorities are: 0,1,2,3,4,5 (idle, lowest, below, normal, above,
       highest).  The default is 2:3
     * exitcodes - print the exit codes of the spawned processes when the
       group exits.
     * log - shortcut to enable MPE logging of the spawned group.

Remarks

   MPI_COMM_SPAWN tries to start maxprocs identical copies of the MPI
   program specified by command, establishing communication with them and
   returning an intercommunicator. The spawned processes are referred to
   as children. The children have their own MPI_COMM_WORLD, which is
   separate from that of the parents. MPI_COMM_SPAWN is collective over
   comm, and also may not return until MPI_INIT has been called in the
   children. Similarly, MPI_INIT in the children may not return until all
   parents have called MPI_COMM_SPAWN. In this sense, MPI_COMM_SPAWN in
   the parents and MPI_INIT in the children form a collective operation
   over the union of parent and child processes. The intercommunicator
   returned by MPI_COMM_SPAWN contains the parent processes in the local
   group and the child processes in the remote group. The ordering of
   processes in the local and remote groups is the same as the as the
   ordering of the group of the comm in the parents and of MPI_COMM_WORLD
   of the children, respectively. This intercommunicator can be obtained
   in the children through the function MPI_COMM_GET_PARENT.

   Advice to users.

   An implementation may automatically establish communication before
   MPI_INIT is called by the children. Thus, completion of MPI_COMM_SPAWN
   in the parent does not necessarily mean that MPI_INIT has been called
   in the children (although the returned intercommunicator can be used
   immediately). ( End of advice to users.)

   The command argument The command argument is a string containing the
   name of a program to be spawned. The string is null-terminated in C. In
   Fortran, leading and trailing spaces are stripped. MPI does not specify
   how to find the executable or how the working directory is determined.
   These rules are implementation-dependent and should be appropriate for
   the runtime environment.

   Advice to users.

   MPI does not say what happens if the program you start is a shell
   script and that shell script starts a program that calls MPI_INIT.
   Though some implementations may allow you to do this, they may also
   have restrictions, such as requiring that arguments supplied to the
   shell script be supplied to the program, or requiring that certain
   parts of the environment not be changed. ( End of advice to users.)

   The argv argument argv is an array of strings containing arguments that
   are passed to the program. The first element of argv is the first
   argument passed to command, not, as is conventional in some contexts,
   the command itself. The argument list is terminated by NULL in C and
   C++ and an empty string in Fortran. In Fortran, leading and trailing
   spaces are always stripped, so that a string consisting of all spaces
   is considered an empty string. The constant MPI_ARGV_NULL may be used
   in C, C++ and Fortran to indicate an empty argument list. In C and C++,
   this constant is the same as NULL.

   Examples of argv in C and Fortran

   To run the program "ocean" with arguments "-gridfile" and "ocean1.grd"
   in C:

       char command[] = "ocean";
       char *argv[] = {"-gridfile", "ocean1.grd", NULL};
       MPI_Comm_spawn(command, argv, ...);

   or, if not everything is known at compile time:

       char *command;
       char **argv;
       command = "ocean";
       argv=(char **)malloc(3 * sizeof(char *));
       argv[0] = "-gridfile";
       argv[1] = "ocean1.grd";
       argv[2] = NULL;
       MPI_Comm_spawn(command, argv, ...);

   In Fortran:

       CHARACTER*25 command, argv(3)
       command = ' ocean '
       argv(1) = ' -gridfile '
       argv(2) = ' ocean1.grd'
       argv(3) = ' '
       call MPI_COMM_SPAWN(command, argv, ...)

   Arguments are supplied to the program if this is allowed by the
   operating system. In C, the MPI_COMM_SPAWN argument argv differs from
   the argv argument of main in two respects. First, it is shifted by one
   element. Specifically, argv[0] of main is provided by the
   implementation and conventionally contains the name of the program
   (given by command). argv[1] of main corresponds to argv[0] in
   MPI_COMM_SPAWN, argv[2] of main to argv[1] of MPI_COMM_SPAWN, etc.
   Second, argv of MPI_COMM_SPAWN must be null-terminated, so that its
   length can be determined. Passing an argv of MPI_ARGV_NULL to
   MPI_COMM_SPAWN results in main receiving argc of 1 and an argv whose
   element 0 is (conventionally) the name of the program.

   If a Fortran implementation supplies routines that allow a program to
   obtain its arguments, the arguments may be available through that
   mechanism. In C, if the operating system does not support arguments
   appearing in argv of main(), the MPI implementation may add the
   arguments to the argv that is passed to MPI_INIT.


   The maxprocs argument MPI tries to spawn maxprocs processes. If it is
   unable to spawn maxprocs processes, it raises an error of class
   MPI_ERR_SPAWN.

   An implementation may allow the info argument to change the default
   behavior, such that if the implementation is unable to spawn all
   maxprocs processes, it may spawn a smaller number of processes instead
   of raising an error. In principle, the info argument may specify an
   arbitrary set [img7.gif] of allowed values for the number of processes
   spawned. The set {m[i]} does not necessarily include the value
   maxprocs. If an implementation is able to spawn one of these allowed
   numbers of processes, MPI_COMM_SPAWN returns successfully and the
   number of spawned processes, m, is given by the size of the remote
   group of intercomm. If m is less than maxproc, reasons why the other
   processes were not spawned are given in array_of_errcodes as described
   below. If it is not possible to spawn one of the allowed numbers of
   processes, MPI_COMM_SPAWN raises an error of class MPI_ERR_SPAWN.

   A spawn call with the default behavior is called hard. A spawn call for
   which fewer than maxprocs processes may be returned is called soft.

   Advice to users.

   By default, requests are hard and MPI errors are fatal. This means that
   by default there will be a fatal error if MPI cannot spawn all the
   requested processes. If you want the behavior "spawn as many processes
   as possible, up to N," you should do a soft spawn, where the set of
   allowed values {m[i]} is {0 ... N}. However, this is not completely
   portable, as implementations are not required to support soft spawning.
   ( End of advice to users.)

   The info argument The info argument to all of the routines in this
   chapter is an opaque handle of type MPI_Info in C, MPI::Info in C++ and
   INTEGER in Fortran. It is a container for a number of user-specified (
   key, value) pairs. key and value are strings (null-terminated char* in
   C, character*(*) in Fortran).

   For the SPAWN calls, info provides additional (and possibly
   implementation-dependent) instructions to MPI and the runtime system on
   how to start processes. An application may pass MPI_INFO_NULL in C or
   Fortran. Portable programs not requiring detailed control over process
   locations should use MPI_INFO_NULL.

   MPI does not specify the content of the info argument, except to
   reserve a number of special key values. The info argument is quite
   flexible and could even be used, for example, to specify the executable
   and its command-line arguments. In this case the command argument to
   MPI_COMM_SPAWN could be empty. The ability to do this follows from the
   fact that MPI does not specify how an executable is found, and the info
   argument can tell the runtime system where to "find" the executable ""
   (empty string). Of course a program that does this will not be portable
   across MPI implementations.

   The root argument All arguments before the root argument are examined
   only on the process whose rank in comm is equal to root. The value of
   these arguments on other processes is ignored.

   The array_of_errcodes argument The array_of_errcodes is an array of
   length maxprocs in which MPI reports the status of each process that
   MPI was requested to start. If all maxprocs processes were spawned,
   array_of_errcodes is filled in with the value MPI_SUCCESS. If only m (
   [img8.gif] ) processes are spawned, m of the entries will contain
   MPI_SUCCESS and the rest will contain an implementation-specific error
   code indicating the reason MPI could not start the process. MPI does
   not specify which entries correspond to failed processes. An
   implementation may, for instance, fill in error codes in one-to-one
   correspondence with a detailed specification in the info argument.
   These error codes all belong to the error class MPI_ERR_SPAWN if there
   was no error in the argument list. In C or Fortran, an application may
   pass MPI_ERRCODES_IGNORE if it is not interested in the error codes. In
   C++ this constant does not exist, and the array_of_errcodes argument
   may be omitted from the argument list.

   Advice to users.

   It is possible in MPI to start a static SPMD or MPMD application by
   starting first one process and having that process start its siblings
   with MPI_COMM_SPAWN. This practice is discouraged primarily for reasons
   of performance. If possible, it is preferable to start all processes at
   once, as a single MPI-1 application.

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

   MPI_ERR_ARG
          Invalid argument. Some argument is invalid and is not identified
          by a specific error class (e.g., MPI_ERR_RANK).

   MPI_ERR_INFO
          Invalid Info

*/
 
// Copyright 2009 Deino Software. All rights reserved.
// Source: http://mpi.deino.net/mpi_functions/index.htm

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#define NUM_SPAWNS 2
int main( int argc, char *argv[] )
{
    int np = NUM_SPAWNS;
    int errcodes[NUM_SPAWNS];
    MPI_Comm parentcomm, intercomm;
    MPI_Init( &argc, &argv );
    MPI_Comm_get_parent( &parentcomm );
    if (parentcomm == MPI_COMM_NULL)
    {
        /* Create 2 more processes - this example must be called
spawn_example.exe for this to work. */
        MPI_Comm_spawn( "spawn_example", MPI_ARGV_NULL, np,
MPI_INFO_NULL, 0, MPI_COMM_WORLD, &intercomm, errcodes );
        printf("I'm the parent.\n");
    }
    else
    {
        printf("I'm the spawned.\n");
    }
    fflush(stdout);
    MPI_Finalize();
    return 0;
}

