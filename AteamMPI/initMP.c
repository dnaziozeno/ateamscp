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
/* Este arquivo contem o codigo necessario para o agente InitMP. Este agente */
/* inicia a memoria de solucoes primais com solucoes geradas geradas por uma */
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

int    *U_x      = NULL,
       *unsat    = NULL,
       *cardin   = NULL,
       *sets     = NULL;
double *aux_cost = NULL;

void ExecAgInitMP(MPI_Comm communicatorMP, MPI_Comm communicatorMD);

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
      errcodes[numSpawns],              
      size,
      rank;	
	
  MPI_Init(&argc, &argv);               
  MPI_Comm_size(MPI_COMM_WORLD, &size);   					                     
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);                                         
  MPI_Comm interComSpawn, commServerMP, commServerMD;             
  
  char        *look        = NULL,
               path[200];
  u_long       NbServerMP  = 0;


  MaxLenPrimalMem = atoi(argv[3]);
  RandomInitMP    = (char) atoi(argv[4]);;
  RestrValue      = (float) (atoi(argv[5]) / 100.0);

  if (!(finput = fopen(argv[1],"r")))
   { printf("\n\n* Erro na abertura do arquivo %s. *",argv[1]);
     exit(1);
   }
  ReadSource();
  fclose(finput);
  srand48(time(NULL));

  if (argc == 6)
    strcpy(path,argv[2]);
  else
    strcpy(path,"./");
  look = argv[1];
  while (look)
    if (look = (char *) strstr(argv[1],"/"))
      argv[1] = (look + 1);
   strcat(path,argv[1]);

  
  char *param = malloc(200 * sizeof(char));
  strcpy(param, path);

  int flagConnect = 1;
  char portMD[MPI_MAX_PORT_NAME];
  
  flagConnect = MPI_Lookup_name("ServerMD", MPI_INFO_NULL, portMD);
  if(flagConnect) 
  {  printf("\n\n* Termino do Agente InitMP *");
     printf("\n* ERRO : A memoria de solucoes duais nao foi iniciada. *");
  }
  
  else
   {  
   	  MPI_Comm_connect(portMD, MPI_INFO_NULL, 0, MPI_COMM_SELF, &commServerMD); 
   	  /* ------------------------------------------------------------------------- */
      /* Iniciando o serverMP */
  
        MPI_Comm_spawn
        ( "/home/naziozeno/Documents/projeto-final/AteamSCP/AteamMPI/serverMP", 
          &param, 1, MPI_INFO_NULL, 0,  MPI_COMM_SELF, &interComSpawn, errcodes
        );		                  
      /* ------------------------------------------------------------------------- */
      /* Espera bloqueante necessária para não dar crash */
  	
  	    char messageBlock[1];
  	    MPI_Status status;
  	    MPI_Recv(messageBlock, 1, MPI_CHAR, 0, MPI_ANY_TAG, interComSpawn, &status); 
      /* ------------------------------------------------------------------------- */
  
      /* Inicio da conexao com serverMP */
      char portMP[MPI_MAX_PORT_NAME];
      flagConnect = 1;
      flagConnect = MPI_Lookup_name("ServerMP", MPI_INFO_NULL, portMP);
      if(flagConnect) 
      {  printf("\n\n* Termino do Agente InitMP *");
         printf("\n* ERRO : Nao foi possivel iniciar ServerMP. *");
      }
      else 	
   	  { /* Conecta como serverMD */	
        MPI_Comm_connect(portMP, MPI_INFO_NULL, 0, MPI_COMM_SELF, &commServerMP);
	    
	    char * buffer;
	    int  method = INIT_GLOBAL_VARS, 
             position = 0,              
             sizeOfMessage = 0;                   
	    
	    sizeOfMessage = 3 * sizeof(int) + sizeof(float) + sizeof(unsigned long);
	    buffer = malloc(sizeOfMessage);
	    
        /* Sequência de empacotamento da primeira mensagem */
	    MPI_Pack(&method, 1, MPI_INT, buffer, sizeOfMessage, &position, commServerMP);
	    MPI_Pack(&nb_lin, 1, MPI_INT, buffer, sizeOfMessage, &position, commServerMP);
	    MPI_Pack(&nb_col, 1, MPI_INT, buffer, sizeOfMessage, &position, commServerMP);
	    MPI_Pack(&density, 1, MPI_FLOAT, buffer, sizeOfMessage, &position, 
	                                                                    commServerMP);
	    MPI_Pack(&NbServerMP, 1, MPI_UNSIGNED_LONG, buffer, sizeOfMessage, &position, 
	                                                                    commServerMP);	 
	  
	    MPI_Send(buffer, position, MPI_PACKED, 0, 1, commServerMP);
  	    free(buffer);  
        
        /* A heuristica primal gulosa sera chamada <MaxLenPrimalMem>  vezes para
	       gerar solucoes que permitam preencher total ou parcialmente a memoria
	       de solucoes primais. */
        ExecAgInitMP(commServerMP, commServerMD);
        //FinishInitServer(ServMPClass);
        FreeSource();

        int i = 0;
        int message = 1224;
  
        MPI_Send(&message, 1, MPI_INT, 0, 1, commServerMP);
        MPI_Send(&message, 1, MPI_INT, 0, 1, commServerMD);
        
        MPI_Comm_disconnect(&commServerMP);
        MPI_Comm_disconnect(&commServerMD);
        printf("\n\n* Termino do Agente InitMP *");
		printf("\n* A memoria de solucoes primais foi iniciada. *\n");
    } 
  }
  MPI_Finalize();
}

/* ------------------------------------------------------------------------- */
void ExecAgInitMP(MPI_Comm communicatorMP, MPI_Comm communicatorMD)
{
  char        stop       = FALSE,
              sleep_ag   = FALSE;
  int         count      = 0,
             *var_x      = NULL;
  double      Z_aux      = 0,
             *var_u      = NULL,
             *red_cost   = NULL;
  u_long      NbServerMP = 0;
  DualType    DualSol;
  PrimalType  PrimalSol;

  var_u           = (double *) malloc(max_lin * sizeof(double));
  red_cost        = (double *) malloc(nb_col * sizeof(double));
  aux_cost        = (double *) malloc(nb_col * sizeof(double));
  var_x           = (int *) malloc(nb_col * sizeof(int));
  PrimalSol.var_x = (int *) malloc(nb_col * sizeof(int));
  U_x             = (int *) malloc((nb_lin+1) * sizeof(int));
  unsat           = (int *) malloc(nb_lin * sizeof(int));
  cardin          = (int *) malloc(nb_col * sizeof(int));
  sets            = (int *) malloc((nb_col+1) * sizeof(int));
  Reduction(var_x,&Z_aux);

/*  while ((count++ < MaxLenPrimalMem ) && (!stop)) */
  while ((count++ < (MaxLenPrimalMem / 2)) && (!stop))
   { if (!unicost)
      RequestDualSol(FALSE, &stop, LinearIncr, NULL, var_u, red_cost,
	 	              &DualSol, agent_free, communicatorMD);
     if (!stop)
      { PrimalSol.value = Z_aux;
	    memcpy(PrimalSol.var_x,var_x,nb_col * sizeof(int));
	    if (unicost)
	     FeoResendeGreedy(RestrValue, &PrimalSol);
	    else
	      PrimalGreedy(RandomInitMP,&DualSol,&PrimalSol);
	    PrimalSol.proc_time = 0;
	    SendPrimalSolutiontoServer(&stop, &sleep_ag, &PrimalSol, communicatorMP);
      }
   }

  free(var_x);
  free(var_u);
  free(red_cost);
  free(aux_cost);
  free(PrimalSol.var_x);
  free(U_x);
  free(unsat);
  free(cardin);
  free(sets);
  
  
}

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */
