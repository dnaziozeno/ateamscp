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
/* DefServerMD() :  Esta funcao inicia o servidor MD da memoria de  solucoes */
/* duais e retorna o ponteiro para a classe criada nessa funcao ou anterior- */
/* mente por outro agente. Deve ser usada nos agentes.                       */
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

DPSK_CLASS *DefMDServer(char   *ID_DPSK,
			char   *path,
			char    init_serv,
			u_long *ServerNb)
{
  char   FileExecServer[200] = "serverMD ";
  u_long NbServMD            = 0;

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

  DPSK_SLOT    m0     ("Method", "TALK");

  DPSK_OBJECT  ServMD ("Server",&file,&clones,&m0,NULL);
  DPSK_CLASS  *ServMDClass;

  /* Verifica se o servidor ja' foi instalado */
 if (!(ServMDClass = LookupClass("ServMD")))
  { if (init_serv)
     { printf("\n\n* Criando e iniciando o servidor de solucoes duais. *\n");
       ServMDClass = DefineClass("ServMD",&ServMD,NULL);
       NbServMD    = ServMDClass -> InstServer("TALK",NULL);
     }
  }
 else
   printf("\n\n* O servidor da memoria de solucoes duais esta ativo. *\n");
 if (ServerNb)
   *ServerNb = NbServMD;
 return(ServMDClass);
}

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */
