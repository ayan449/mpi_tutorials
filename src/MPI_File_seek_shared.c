/* 
MPI_File_seek_shared

   Updates the shared file pointer
int MPI_File_seek_shared(
  MPI_File mpi_fh,
  MPI_Offset offset,
  int whence
);

Parameters

   mpi_fh
          [in] file handle (handle)

   offset
          [in] file offset (integer)

   whence
          [in] update mode (state)

Remarks

   MPI_FILE_SEEK_SHARED updates the shared file pointer according to
   whence, which has the following possible values:
     * MPI_SEEK_SET: the pointer is set to offset
     * MPI_SEEK_CUR: the pointer is set to the current pointer position
       plus offset
     * MPI_SEEK_END: the pointer is set to the end of file plus offset

   MPI_FILE_SEEK_SHARED is collective; all the processes in the
   communicator group associated with the file handle mpi_fh must call
   MPI_FILE_SEEK_SHARED with the same values for offset and whence.

   The offset can be negative, which allows seeking backwards. It is
   erroneous to seek to a negative position in the view.

*/
 
// Copyright 2009 Deino Software. All rights reserved.
// Source: http://mpi.deino.net/mpi_functions/index.htm

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
* access style is explicitly described as modifiable. values include
* read_once, read_mostly, write_once, write_mostlye, random
*/
int main( int argc, char *argv[] )
{
    int errs = 0, err;
    int buf[10];
    int rank;
    MPI_Comm comm;
    MPI_Status status;
    MPI_File fh;
    MPI_Info infoin, infoout;
    char value[1024];
    int flag, count;
    MPI_Init( &argc, &argv );
    comm = MPI_COMM_WORLD;
    MPI_Comm_rank( comm, &rank );
    MPI_Info_create( &infoin );
    MPI_Info_set( infoin, "access_style", "write_once,random" );
    err = MPI_File_open( comm, "testfile", MPI_MODE_RDWR |
MPI_MODE_CREATE, infoin, &fh );
    if (err) {
        errs ++;
        MPI_Abort( MPI_COMM_WORLD, 911 );
    }
    buf[0] = rank;
    err = MPI_File_write_ordered( fh, buf, 1, MPI_INT, &status );
    if (err) { errs ++; }
    MPI_Info_set( infoin, "access_style", "read_once" );
    err = MPI_File_seek_shared( fh, 0, MPI_SEEK_SET );
    if (err) { errs ++; }
    err = MPI_File_set_info( fh, infoin );
    if (err) { errs ++; }
    MPI_Info_free( &infoin );
    buf[0] = -1;
    err = MPI_File_read_ordered( fh, buf, 1, MPI_INT, &status );
    if (err) { errs ++; }
    MPI_Get_count( &status, MPI_INT, &count );
    if (count != 1) {
        errs++;
        printf( "Expected to read one int, read %d\n", count
);fflush(stdout);
    }
    if (buf[0] != rank) {
        errs++;
        printf( "Did not read expected value (%d)\n", buf[0]
);fflush(stdout);
    }
    err = MPI_File_get_info( fh, &infoout );
    if (err) { errs ++; }
    MPI_Info_get( infoout, "access_style", 1024, value, &flag );
    /* Note that an implementation is allowed to ignore the set_info,
so we'll accept either the original or the updated version */
    if (!flag) {
    }
    else {
        if (strcmp( value, "read_once" ) != 0 && strcmp( value,
"write_once,random" ) != 0) {
            errs++;
            printf( "value for access_style unexpected; is %s\n", value
);fflush(stdout);
        }
    }
    MPI_Info_free( &infoout );
    err = MPI_File_close( &fh );
    if (err) { errs ++; }
    MPI_Barrier( comm );
    if (rank == 0) {
        err = MPI_File_delete( "testfile", MPI_INFO_NULL );
        if (err) { errs ++; }
    }
    MPI_Finalize( );
    return errs;
}

