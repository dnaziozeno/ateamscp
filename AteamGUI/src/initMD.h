#include <stdio.h>
#include <stdlib.h>

#ifndef __INIT_MD__
#define __INIT_MD__

void ExecAgInitMD(MPI_Comm comm);
MPI_Comm InitMD(int argc,char *argv[]);

#endif /* __INIT_MD__ */
