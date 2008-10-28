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
/*                                                                           */
/*    Implementacao de um procedimento para gerar facetas para o Problema de */
/* Recobrimento de Conjuntos. Este procedimento gera facetas do seguinte ti- */
/* po: sum Xi = 2.  O procedimento original gera facetas com coeficienes  no */
/* conjunto {0,1,2} e esta descrito na referencia abaixo.                    */
/*                                                                           */
/* Referencia-1  : On the Set Covering Polytope: I. All the Facets with      */
/*                 coefficients in {0,1,2}                                   */
/*                 Egon Balas and Shu Ming Ng                                */
/*                 Mathematical Programming 43, pp 57-69, 1989               */
/*                                                                           */
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */

#include "globais.h"   /* Inclusao das definicoes de tipos, macros e constan-*/
                       /* tes utilizadas em todos os arquivos do A-Team dis- */
                       /* tribuido para o Set Covering Problem.              */

extern char    *exclude;
extern char    *satur;
extern int      nb_lin;
extern int      nb_col;
extern int      MaxCutGen;
extern int     *S;
extern int     *primes;
extern Lin     *line;
extern Col     *column;
extern RandInd *set_ind;

void LinesSatur(PrimalType *PrimalSol);
void GenRandomInds(int        *nb_ind,
		   PrimalType *PrimalSol);
void RandomizeInds(int nb_ind);
void GenSet_S(int         nb_ind,
	      PrimalType *PrimalSol);
void AddCoefs(int        *nb_cuts,
	      CutType    *cut,
	      PrimalType *PrimalSol);
int  TestSort(const void *a,
              const void *b);
void GenNewSet_S(int     *nb_cuts,
		 CutType *cut);

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */
/*
void GenBNCuts(int        *nb_cuts,
               PrimalType *PrimalSol,
               CutType    *cut)
{
  int i      = 0,
      nb_ind = 0;

  *nb_cuts = 0;
  LinesSatur(PrimalSol);
  GenRandomInds(&nb_ind,PrimalSol);
  GenSet_S(nb_ind,PrimalSol);
  AddCoefs(nb_cuts,cut,PrimalSol);
  (*nb_cuts)++;
  for (i = 1; i < MaxCutGen; i++)
   { GenNewSet_S(nb_cuts,cut);
     AddCoefs(nb_cuts,cut,PrimalSol);
     (*nb_cuts)++;
   }
}
*/

void GenBNCuts(int        *nb_cuts,
	       PrimalType *PrimalSol,
	       CutType    *cut)
{
  int i      = 0,
      nb_ind = 0;

  *nb_cuts = 0;
  LinesSatur(PrimalSol);
  GenRandomInds(&nb_ind,PrimalSol);
  for (i = 0; i < MaxCutGen; i++)
   { GenSet_S(nb_ind,PrimalSol);
     if (S[nb_lin] > 1)
      { AddCoefs(nb_cuts,cut,PrimalSol);
	GenNewSet_S(nb_cuts,cut);
	if (S[nb_lin] > 1)
	 { AddCoefs(nb_cuts,cut,PrimalSol);
	   (*nb_cuts)++;
         }
      }
     RandomizeInds(nb_ind);
   }
}

/* ------------------------------------------------------------------------- */
/* Percorre todas as restricoes do problema, verificando quais estao satura-
   das (soma das variaveis igual a 1), gerando o conjunto S com tais restri-
   coes. */

void LinesSatur(PrimalType *PrimalSol)
{
  int    i   = 0,
         sum = 0;
  DList *Ji  = NULL;

  memset(satur,FALSE,nb_lin * sizeof(char));
  for (i = 0; i < nb_lin; i++)
   { sum = 0;
     if (!line[i].del)
      { Ji = line[i].first;
	while ((Ji) && (sum < 2))
	 { sum += PrimalSol->var_x[column[Ji->col].var];
	   Ji   = Ji->right;
         }
      }
     satur[i] = (sum == 1);
   }
}

/* ------------------------------------------------------------------------- */
/* Percorre as variaveis primais x de uma solucao factivel qualquer, gerando
   um conjunto com os indices de tais variaveis, que serao posteriormente a-
   cessadas de forma randomica para garantir que diferentes cortes sejam ge-
   rados. */

void GenRandomInds(int        *nb_ind,
		   PrimalType *PrimalSol)
{
  int j = 0;

  *nb_ind = 0;
  while ((j < nb_col) && (*nb_ind < nb_lin))
   { if (PrimalSol->var_x[column[j].var] == 1)
      { set_ind[*nb_ind].j    = j;
        set_ind[*nb_ind].sort = lrand48();
        (*nb_ind)++;
      }
     j++;
   }
  qsort((void *)set_ind,*nb_ind,sizeof(RandInd),TestSort);
}

/* ------------------------------------------------------------------------- */
/* Gera uma nova ordenacao para o conjunto com os indices das variaveis pri-
   mais que sao iguais a 1. */

void RandomizeInds(int nb_ind)
{
  int i = 0;

  for (i = 0; i < nb_ind; i++)
    set_ind[i].sort = lrand48();
  qsort((void *)set_ind,nb_ind,sizeof(RandInd),TestSort);
}

/* ------------------------------------------------------------------------- */
/* Percorre as variaveis primais x de uma solucao factivel qualquer,  o con-
   junto S e' entao construido a partir das variaveis que  sao  iguais  a  1. 
   Dentre as linhas cobertas por uma tal variavel, e' escolhida uma para com-
   por o conjunto S e as outras sao excluidas do conjunto de linhas que podem
   vir a ser inseridas em S. */

void GenSet_S(int         nb_ind,
	      PrimalType *PrimalSol)
{
  int    i        = 0,
         j        = 0,
         line_S   = 0,
         rand_qtd = 0;
  DList *Ij       = NULL,
        *Ji       = NULL;

  S[nb_lin] = 0;
  memset(exclude,FALSE,nb_lin * sizeof(char));
  rand_qtd = 2 + (lrand48() % 4);
/*  rand_qtd = 15; */

  while ((i < nb_ind) && (S[nb_lin] < rand_qtd))
   { j = set_ind[i].j;
     line_S = -1;
     Ij = column[j].first;
     while ((Ij) && (line_S == -1))
      { if ((!exclude[Ij->lin]) && (satur[Ij->lin]))
	 { exclude[Ij->lin] = TRUE;
	   line_S = Ij->lin;
         }
	Ij = Ij->down;
      }
     if (line_S != -1)
      { S[S[nb_lin]] = line_S;
	S[nb_lin]++;
      }
     i++;
   }
}

/* ------------------------------------------------------------------------- */
/* Este procedimento constroi um novo conjunto S = M(J_zero(beta)), onde be-
   ta e' a ultima restricao gerada. Para maiores detalhes ver o  Teorema 2.1
   na Referencia-1. */
void GenNewSet_S(int     *nb_cuts,
		 CutType *cut)
{
  char   beta = TRUE;
  int    i    = 0,
         j    = 0;
  DList *Ji   = NULL;

  S[nb_lin] = 0;
  for (i = 0; i < nb_lin; i++)
   { beta = TRUE;
     Ji = line[i].first;
     while (Ji && beta)
      { if (cut[*nb_cuts - 1].coef[Ji->col] == 0)
	 { beta = FALSE;
	   break;
	 }
	Ji = Ji->right;
      }
     if (beta)
      { S[S[nb_lin]] = i;
	S[nb_lin]++;
      }
   }
}

/* ------------------------------------------------------------------------- */
/* Percorre as restricoes do conjunto S, somando os coeficientes de cada co-
   luna para gerar a nova restricao R, com a seguinte  lei de  formacao:  se 
   todos os coeficientes de uma mesma coluna sao iguais a 0,  entao  o  novo
   coeficiente tambem sera' 0; se todos forem iguais a 1, entao o  novo coe-
   ficiente sera' 2 e sera' 1 em caso contrario. */

void AddCoefs(int        *nb_cuts,
	      CutType    *cut,
	      PrimalType *PrimalSol)
{
  short *aux = NULL;
  int    i   = 0,
         j   = 0;
  DList *Ji  = NULL;

  i   = *nb_cuts;
  aux = cut[i].coef;
  cut[i] = (CutType) {FALSE,FALSE,2,aux,0,0,0,-1,0};
  memset(cut[i].coef,0,nb_col * sizeof(short));
  for (j = 0; j < S[nb_lin]; j++)
   { Ji = line[S[j]].first;
     while (Ji)
      { (cut[i].coef[Ji->col])++;
	Ji = Ji->right;
      }
   }
  for (j = 0; j < nb_col; j++)
   { cut[i].coef[j] = (short) ceil(((float) cut[i].coef[j]) /
				   (((float) S[nb_lin]) - .8));
     if (cut[i].coef[j] > 0)
/*      { cut[i].key += (j * 557); */
      { cut[i].key += (j * primes[j]);
	cut[i].col_cut++;
      }
   }
}

/* ------------------------------------------------------------------------- */

int TestSort(const void *a,
             const void *b)
{
  return(((RandInd *)a)->sort - ((RandInd *)b)->sort);
}

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */
