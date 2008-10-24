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
/* InitServerMP() : Esta funcao inicia o servidor MP da memoria de  solucoes */
/* primais e retorna o ponteiro para a classe criada nessa funcao ou anteri- */
/* ormente por outro agente. Deve ser usada nos agentes primais.             */
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */

#include "globais.h"   /* Inclusao das definicoes de tipos, macros e constan-*/
                       /* tes utilizadas em todos os arquivos do A-Team dis- */
                       /* tribuido para o Set Covering Problem.              */

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */

DPSK_CLASS *DefMPServer(char   *ID_DPSK,
			char   *path,
			char    init_serv,
			u_long *ServerNb)
{
  char   FileExecServer[200] = "serverMP ";
  u_long NbServMP            = 0;

  if (atoi(ID_DPSK) < 6665)
   { printf("\n\n ****** ERRO  *******\n");
     printf("\n Identificador invalido de sub-rede do DPSK");
     printf("\n ou identificador fornecido menor que 6665.");
     printf("\n Uso correto:  %% ns -p<ident> &");
     printf("\n               %% tman -p<ident> &");
     printf("\n               %% Cliente <ident> <arq. dados> &\n");
     exit(1);
   }
  strcat(FileExecServer,ID_DPSK);
  strcat(FileExecServer," ");
  strcat(FileExecServer,path);

  OpenDPSK(atoi(ID_DPSK));

  DPSK_SLOT    file   ("File",FileExecServer);

  /* Nr. maximo de servidores */
  DPSK_SLOT    clones ("MaxClones",1);

  DPSK_SLOT    m0     ("Method","TALK");

  DPSK_OBJECT  ServMP ("Server",&file,&clones,&m0,NULL);
  DPSK_CLASS  *ServMPClass;

  /* Verifica se o servidor ja' foi instalado */
 if (!(ServMPClass = LookupClass("ServMP")))
  { if (init_serv)
     { printf("\n\n* Criando e iniciando o servidor de solucoes primais. *\n");
       ServMPClass = DefineClass("ServMP",&ServMP,NULL);
       NbServMP    = ServMPClass -> InstServer("TALK",NULL);
     }
  }
 else
   printf("\n\n* O servidor da memoria de solucoes primais esta ativo. *\n");
 if (ServerNb)
   *ServerNb = NbServMP;

 return(ServMPClass);
}

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */
