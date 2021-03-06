/* ------------------------------------------------------------------------------------- */
/* Projeto Final de Curso, Universidade Federal de Goias, Instituto de Informatica       */
/* Titulo: Console Grafico para Controle de um A-Team Dinamico                           */
/*                                                                                       */
/* Orientador: Humberto Jose Longo                                                       */
/* Orientando: Diocleciano Pereira Naziozeno Neto                                        */
/*                                                                                       */
/* Homepage: http://www.inf.ufg.br/~diocleciano                                          */
/* E-mail: dioclecianoneto@hotmail.com                                                   */
/*                                                                                       */
/* File: IOParam.h                                                                       */
/* Date: 16/08/2008                                                                      */
/* ------------------------------------------------------------------------------------- */
/* Essa classe tem como objetivo tratar as questoes referentes a entrada e saida de      */
/* parametros do A-Team.                                                                 */
/* ------------------------------------------------------------------------------------- */

#ifndef IOParam_H
#define IOParam_H

/* ------------------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------------------- */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <wx/wx.h>
#include <wx/filename.h>
#include <wx/file.h>

#define IOPARAMS_SUCCESS 1
#define IOPARAMS_FAILURE 0

/* ------------------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------------------- */

class IOParam {

public:
    static int *readParams(const char *file_name, int nparams);
    static int writeParams(const char *file_name, int *params, int nparams);
    static char *getExecutablePath();
};

#endif /* IOParam_H */

/* ------------------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------------------- */
