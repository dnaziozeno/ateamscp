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
/* Este arquivo contem o codigo necessario para a interface do agente gencut */
/* com o servidor de memoria de solucoes primais e com o servidor de memoria */
/* de cortes do ATeam distribuido para o Set Covering Problem.               */
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */

#include "globais.h"    /* Inclusao das definicoes de tipos, macros e  cons- */
			/* tantes utilizadas em todos os arquivos  do A-Team */
                        /* distribuido para o Set Covering Problem.          */
#include "agentes.h"    /* Inclusao das definicoes de variaveis usadas em to-*/
                        /* os agentes do ATeam distribuido.                  */

char    *exclude      = NULL,
        *satur        = NULL;
int     *S            = NULL,
        *Q            = NULL,
        *cost_col     = NULL,
        *identity_lin = NULL,
        *identity_col = NULL,
        *primes       = NULL;
RandInd *set_ind      = NULL;

void ExecAgGenCut(DPSK_CLASS *ServMDClass,
		  DPSK_CLASS *ServMPClass);
void GenTabPrimes();

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
   O primeiro parametro a ser passado para esse agente e' o numero do proce-
   ssador em que esta executando o 'ns'  e o segundo parametro e' o nome  do
   arquivo que contem a instancia do SCP.
   ------------------------------------------------------------------------- */
main(int argc, char *argv[])
{
  DPSK_CLASS *ServMPClass = NULL,
             *ServMDClass = NULL;

  ServMPClass = DefMPServer(argv[1],"\0",FALSE,NULL);
  if (!ServMPClass)
   { printf("\n\n* Agente GenCuts finalizado *");
     printf("\n* ERRO : Servidor de solucoes primais nao iniciado *\n");
   }
  else
   { ServMDClass = DefMDServer(argv[1],"\0",FALSE,NULL);
     if (!ServMDClass)
      { printf("\n\n* Agente GenCuts finalizado *");
	printf("\n* ERRO : Servidor de solucoes duais nao iniciado *\n");
      }
     else
      { TimeSleeping = (int)  ReadAteamParam(1);
	MaxCutGen    = (int)  ReadAteamParam(7);
	/*  BalasNgCut   = (char) ReadAteamParam(20); */
	if (!(finput = fopen(argv[2],"r")))
	 { printf("\n\n* Erro na abertura do arquivo %s *\n",argv[2]);
	   exit(1);
         }
	ReadSource();
	Reduction(NULL,NULL);
	fclose(finput);
	srand48(time(NULL));

	ExecAgGenCut(ServMDClass,ServMPClass);

	printf("\n\n* Agente GenCut finalizado *");
	printf("\n* Servidor finalizou processamento *\n");
      }
   }

  CloseDPSK();
}

/* ------------------------------------------------------------------------- */

void ExecAgGenCut(DPSK_CLASS *ServMDClass,
		  DPSK_CLASS *ServMPClass)
{
  char        stop     = FALSE,
              sleep_ag = FALSE;
  int         i        = 0,
              count    = 0,
              nb_cuts  = 0,
             *var_x    = NULL;
  CutType    *cut      = NULL;
  PrimalType  PrimalSol;

/*  if (BalasNgCut) */
   { exclude = (char *) malloc(nb_lin * sizeof(char));
     satur   = (char *) malloc(nb_lin * sizeof(char));
   }
/*  else */
   { Q            = (int *) malloc((nb_col+1) * sizeof(int));
     cost_col     = (int *) malloc(nb_lin * sizeof(int));
     identity_lin = (int *) malloc(nb_lin * sizeof(int));
     identity_col = (int *) malloc((nb_lin+1) * sizeof(int));
   }
  set_ind = (RandInd *) malloc((nb_lin + 1) * sizeof(RandInd));
  S       = (int *) malloc((nb_lin+1) * sizeof(int));
  primes  = (int *) malloc(nb_col * sizeof(int));
  var_x   = (int *) malloc(nb_col * sizeof(int));
  cut     = (CutType *) malloc(MaxCutGen * sizeof(CutType));
  for (i = 0; i < MaxCutGen; i++)
    cut[i].coef = (short *) malloc(nb_col * sizeof(short));
  GenTabPrimes();

  do
   { if (sleep_ag)
       sleep(TimeSleeping);
/*     RequestPrimalSol(FALSE,&stop,Constant,var_x,&PrimalSol,ServMPClass); */
     RequestPrimalSol(FALSE,&stop,LinearDecr,var_x,&PrimalSol,ServMPClass);
     if (!stop)
/*    { if (BalasNgCut = !BalasNgCut) */
/*    { if (!(++count % 5))
          GenBNCuts(&nb_cuts,&PrimalSol,cut);
        else */
      {   GenBRCuts(&nb_cuts,&PrimalSol,cut);
	if (nb_cuts > 0)
	  SendCutstoServer(&stop,&sleep_ag,nb_cuts,cut,ServMDClass);
      }
   } while(!stop);

/*  if (BalasNgCut) */
   { free(exclude);
     free(satur);
     free(S);
     free(set_ind);
   }
/*  else */
   { free(Q);
     free(cost_col);
     free(identity_lin);
     free(identity_col);
   }
  free(var_x);
  free(cut);
  free(primes);
  for (i = 0; i < MaxCutGen; i++)
    free(cut[i].coef);
}

/* ------------------------------------------------------------------------- */

void GenTabPrimes()
{
  int i = 0,
      j = 0,
      k = 0;

  k = (nb_col < nb_lin) ? nb_lin : nb_col;
  while (j < nb_col)
   { for (i = 2; i < k; i++)
       if ((k % i) == 0)
	 break;
     if (i == k)
      { primes[j] = k;
	j++;
      }
     k++;
   }
}




/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */
