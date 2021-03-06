/* ------------------------------------------------------------------------- */
/*                                                                           */
/* DISSERTACAO DE MESTRADO -> DCC-UNICAMP                                    */
/*                                                                           */
/* Titulo    : Resolucao de problemas de Recobrimento  e  Particionamento de */
/*             Conjuntos com o uso de Times Assincronos.                     */
/*                                                                           */
/* Orientador: Marcus Vinicius S. Poggi de Aragao                            */
/* Orientando: Humberto Jose Longo                                           */
/*                                                                           */
/* Alteracoes: 0 -> (10/09/94)                                               */
/*                                                                           */
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
/* Este arquivo contem o codigo necessario para o agente InitMD. Este agente */
/* inicia a memoria de solucoes duais com solucoes geradas geradas  por  uma */
/* heuristica gulosa.                                                        */
/* ------------------------------------------------------------------------- */
 
/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */

#include "globais.h"    /* Inclusao das definicoes de tipos, macros e  cons- */
			            /* tantes utilizadas em todos os arquivos  do A-Team */
                        /* distribuido para o Set Covering Problem.          */
#include "agentes.h"    /* Inclusao das definicoes de variaveis usadas em to-*/
                        /* os agentes do ATeam distribuido.                  */


int    *I_u     = NULL,
       *inact   = NULL;
double *delta_u = NULL;

void ExecAgInitMD(MPI_Comm comm);

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
   O primeiro parametro a ser passado para esse agente e' o numero do proce-
   ssador em que esta executando o 'ns',  o segundo parametro e' o  nome  do
   arquivo que contem a instancia do SCP e o terceiro e' o nome do diretorio
   em que serao gerados os arquivos de estatisticas.
   ------------------------------------------------------------------------- */
main(int argc,char *argv[])
{

  int numSpawns = 1,                    /* Qtd de processos de serverMD      */ 
      errcodes[numSpawns],              /* Array de erros para o spawn. Um   */
  					                    /* código por processo               */										
      
      /* Variáveis de controle de um grupo de processos da MPI*/
      size,
      rank;	
	

  MPI_Init(&argc, &argv);               
  MPI_Comm_size(MPI_COMM_WORLD, &size);   					                     
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);                                         
  MPI_Comm interComSpawn; 		        
                                        
  char        	 *look        = NULL,
                 path[200];
  unsigned long  NbServerMD   = 0;
               
/*
  MaxLenDualMem = (int)  ReadAteamParam(2);
  ReducPerc     = (int)  ReadAteamParam(12);
  RandomInitMD  = (char) ReadAteamParam(16);
*/

  MaxLenDualMem = atoi(argv[3]);
  ReducPerc     = atoi(argv[4]);
  RandomInitMD  = (char) atoi(argv[5]);

  /* OBS: Esse arquivo de ser passado por argv[2] */
  if (!(finput = fopen(argv[1],"r")))
   { printf("\n\n* Erro na abertura do arquivo %s. *",argv[1]);
     exit(1);
   }
  
   
  ReadSource();          /* Localizada em readsc.c */
  fclose(finput);
  Reduction(NULL,NULL); /* Localizada em reduction.c */
  srand48(time(NULL));

  if (argc == 10)
    strcpy(path,argv[2]);
  else
    strcpy(path,"./");
  look = argv[1];
  while (look)
    if (look = (char *) strstr(argv[1],"/"))
      argv[1] = (look + 1);
  strcat(path,argv[1]);
  
//  char *param = malloc(200 * sizeof(char));
//  strcpy(param, path);
 
    char *param[7];
    param[0] = (char *) malloc (512 * sizeof(char));
    param[1] = (char *) malloc (16 * sizeof(char));
    param[2] = (char *) malloc (16 * sizeof(char));
    param[3] = (char *) malloc (16 * sizeof(char));
    param[4] = (char *) malloc (16 * sizeof(char));
    param[5] = (char *) malloc (16 * sizeof(char));
    param[6] = NULL;

    strcpy(param[0], path);
    strcpy(param[1], argv[3]);
    strcpy(param[2], argv[6]);
    strcpy(param[3], argv[7]);
    strcpy(param[4], argv[8]);
    strcpy(param[5], argv[9]);

  /* ------------------------------------------------------------------------- */
  /* Iniciando o serverMD */
  
   MPI_Comm_spawn("../../AteamMPI/bin/serverMD", 
      param, 1, MPI_INFO_NULL, 0,  MPI_COMM_SELF, &interComSpawn, errcodes);		                  
  /* ------------------------------------------------------------------------- */
  /* Espera bloqueante necessária para não dar crash */
  	
  	char messageBlock[1];
  	MPI_Status status;
  	MPI_Recv(messageBlock, 1, MPI_CHAR, 0, MPI_ANY_TAG, interComSpawn, &status); 
  /* ------------------------------------------------------------------------- */
  
  
  int  method = INIT_GLOBAL_VARS, /* Primeira função a ser chamada pelo      */
         			              /* serverMD (INIT_GLOBAL_VARS)             */
       position = 0,              /* Marca as posicoes de cada dado 
                                                                  empacotado */
       flagConnect = 1;		      /* Flag para verificar se conectou com o 
       				                 servidor                                */
       
  char * buffer,                  /* Buffer contendo a mensagem a ser enviada*/
       port[MPI_MAX_PORT_NAME];   /* Porta para conectar com o serverMD      */
       
  MPI_Comm commServerMD;          /* Comunicador retornado apos a conexao com
  						                                          serverMD   */

  
 
  /* Verificando a existencia do serverMD - se sim recebe a porta */
  /* para conectar                                                */
  flagConnect = MPI_Lookup_name("ServerMD", MPI_INFO_NULL, port);

  if(flagConnect) 
  {
      printf("\n\n Erro: Porta não concebida \n\n");fflush(stdout);
  
  }else{	  
  	  
  	  /* Conecta como serverMD */	
      MPI_Comm_connect(port, MPI_INFO_NULL, 0, MPI_COMM_SELF, &commServerMD);
	  
	  buffer = malloc(3 * sizeof(int) + sizeof(float) + sizeof(unsigned long));
      
      /* Sequência de empacotamento da primeira mensagem */
	  MPI_Pack(&method, 1, MPI_INT, buffer, 3 * sizeof(int) + sizeof(float) + 
	  	sizeof(unsigned long), &position, commServerMD);
	  
	  /*
	   * Position foi incrementada: passa a referenciar a primeira
	   * posição livre no buffer e assim sucessivamente.   
	   */
	  MPI_Pack(&nb_lin, 1, MPI_INT, buffer, 3 * sizeof(int) + sizeof(float) + 
	  	sizeof(unsigned long), &position, commServerMD);
	  MPI_Pack(&nb_col, 1, MPI_INT, buffer, 3 * sizeof(int) + sizeof(float) + 
	  	sizeof(unsigned long), &position, commServerMD);
	  MPI_Pack(&density, 1, MPI_FLOAT, buffer, 3 * sizeof(int) + sizeof(float) + 
	  	sizeof(unsigned long), &position, commServerMD);
	  MPI_Pack(&NbServerMD, 1, MPI_UNSIGNED_LONG, buffer, 3 * sizeof(int) + 
	  	sizeof(float) + sizeof(unsigned long), &position, commServerMD);	 
	  /*
	   * Envia a mensagem empacotada para o serverMD, fazendo uso do comu-
	   * nicador interComSpawn resultante da chamada a rotina spawn
       */
  	  MPI_Send(buffer, position, MPI_PACKED, 0, 1, commServerMD);
  	  free(buffer);
  }
  
 
   /* A heuristica dual gulosa sera chamada <MaxLenDualMem> vezes para  gerar
	  solucoes que permitam preencher total ou parcialmente a memoria de solu-
	  coes duais. */
   ExecAgInitMD(commServerMD);
   FreeSource();
	
   int i = 0;
   int message = 1224;
   //printf("\n \n Messagen 1224 %d: \n", message);
   
   MPI_Send(&message, 1, MPI_INT, 0, 1, commServerMD);
 
   MPI_Comm_disconnect(&commServerMD);
   //printf("\n\n* Termino do Agente InitMD *");
   //printf("\n* A memoria de solucoes duais foi inicializada. *\n");
   
   MPI_Finalize();
}
     
/* ------------------------------------------------------------------------- */
void ExecAgInitMD(MPI_Comm communicator)
{
  char     stop        = FALSE,
           sleep_ag    = FALSE,
           new_problem = FALSE;
  int      count       = 0;
  
  DualType DualSol;

  MPI_Datatype	newtype;

  srand48(time(NULL));
  
  DualSol.var_u    = (double *) malloc(max_lin * sizeof(double));
  DualSol.red_cost = (double *) malloc(nb_col * sizeof(double));
  delta_u          = (double *) malloc(nb_lin * sizeof(double));
  //I_u              = (int *) malloc((2 * nb_lin+1) * sizeof(int));
  I_u              = (int *) malloc((nb_lin + 1) * sizeof(int));
  inact            = (int *) malloc(nb_lin * sizeof(int));
  
  //I_u = I_u + nb_lin;
 /*   while ((count++ < MaxLenDualMem) && (!stop)) */
 
 while ((count++ < (MaxLenDualMem / 2)) && (!stop))
 {
     DualSol.nb_cuts   = 0;
     DualSol.proc_time = 0;
     DualSol.cut       = NULL;
     DualGreedy(RandomInitMD,((float) ReducPerc / 100.0),&DualSol);
     SendDualSolutiontoServer(&stop,&sleep_ag,&new_problem,
			                               &DualSol, communicator);
 }
  
 free(DualSol.var_u);
 free(DualSol.red_cost);
 free(delta_u);
 //I_u = I_u - nb_lin;
 free(I_u);
 free(inact);
 
  
  printf("\n\n Termino do exec \n\n");
}
     
/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */

