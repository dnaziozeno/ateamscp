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
/* File: IOParam.cpp                                                                     */
/* Date: 16/08/2008                                                                      */
/* ------------------------------------------------------------------------------------- */
/* Essa classe tem como objetivo tratar as questoes referentes a entrada e saida de      */
/* parametros do A-Team.                                                                 */
/* ------------------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------------------- */

#include "IOParam.h"

/* ------------------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------------------- */
/* Retorna um ponteiro para um vetor de inteiros que representa os parametros do A-Team, */
/* os quais estão na mesma sequencia do arquivo de entrada. A posicao 0 do vetor contem  */
/* o status da operacao, que pode ser IOPARAMS_SUCCESS ou IOPARAMS_FAILURE.              */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* const char *file_name - Caminho do arquivo de parametros de configuracao.             */
/* ------------------------------------------------------------------------------------- */
int *IOParam::readParams(const char *file_name)
{
    FILE *file;
    char *strin = (char *) malloc(128 * sizeof(char));
    int *params = (int *) malloc(24 * sizeof(int));

    if (file_name == NULL)
    {
        params[0] = IOPARAMS_FAILURE;
        return params;
    }

    /* Abre o arquivo de parametros para leitura. */
    if ((file = fopen(file_name, "r")) == NULL)
    {
        params[0] = IOPARAMS_FAILURE;
        return params;
    }

    /* Descarta as 8 primeiras linhas de comentarios. */
    for (int count = 0; count < 8; count++) fgets(strin, 100, file);

    /* Recebe os valores do parametros e os atribui as posicoes do vetor. */
    for (int count = 1; count < 24; count++)
    {
        if (fscanf(file, "%d", &params[count]) == EOF)
        {
            params[0] = IOPARAMS_FAILURE;
            return params;
        }

        fgets(strin, 100, file);
    }

    /* Fecha o arquivo de parametros. */
    if (fclose(file))
    {
        params[0] = IOPARAMS_FAILURE;
        return params;
    }

    /* Libera a area de memoria ocupada pelas variaveis auxiliares. */
    free(strin);

    params[0] = IOPARAMS_SUCCESS;
    return params;
}

/* ------------------------------------------------------------------------------------- */
/* Retorna IOPARAMS_SUCCESS caso os parametros de configuracao sejam escritos no arquivo */
/* e IOPARAMS_FAILURE caso ocorra algum erro.                                            */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* const char *file_name - Caminho do arquivo de parametros de configuracao.             */
/* int *params - Ponteiro para o vetor de parametros do A-Team.                          */
/* ------------------------------------------------------------------------------------- */
int IOParam::writeParams(const char *file_name, int *params)
{
    FILE *model_file, *new_file;
    char *strin = (char *) malloc(128 * sizeof(char));

    if (file_name == NULL || params == NULL) return IOPARAMS_FAILURE;

    /* Obtem o caminho do arquivo de parametros modelo. */
    char *model_path = IOParam::getExecutablePath();
    strncat(model_path, "/etc/ateam_param.model", 22);

    /* Abre o arquivo de modelo de parametros para leitura e cria o novo. */
    if ((model_file = fopen(model_path, "r")) == NULL) return IOPARAMS_FAILURE;
    if ((new_file = fopen(file_name, "w")) == NULL) return IOPARAMS_FAILURE;

    /* Escreve as 8 primeiras linhas de comentarios. */
    for (int count = 0; count < 8; count++)
    {
        fgets(strin, 100, model_file);
        fputs(strin, new_file);
    }

    fprintf(new_file, "\n");

    /* Escreve os parametros e os comentarios correspondentes, os */
    /* quais estao definidos no modelo de arquivo de parametros.  */
    for (int count = 1; count < 24; count++)
    {
        fscanf(model_file, "%*d");
        fgets(strin, 100, model_file);
        fprintf(new_file, "%6d%s", params[count], strin);
    }

    /* Fecha o arquivo de modelo e o novo arquivo. */
    if (fclose(model_file)) return IOPARAMS_FAILURE;
    if (fclose(new_file)) return IOPARAMS_FAILURE;

    /* Libera a area de memoria ocupada pelas variaveis auxiliares. */
    free(strin);

    return IOPARAMS_SUCCESS;
}

/* ------------------------------------------------------------------------------------- */
/* Retorna uma string de caracteres que contem o caminho absoluto para o diretorio da    */
/* aplicacao.                                                                            */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
char *IOParam::getExecutablePath()
{
    /* Obtem o caminho relativo para o executavel. */
    wxString argv0 = wxTheApp->argv[0];

    wxPathList pathlist;
    pathlist.AddEnvList(wxT("PATH"));
    wxString path = pathlist.FindAbsoluteValidPath(argv0);

    /* Obtem o caminho absoluto do diretorio a partir */
    /* do qual o arquivo binario foi executado.       */
    wxFileName filename(path);
    filename.Normalize();
    path = filename.GetFullPath();

    /* Transforma o caminho absoluto do executavel no */
    /* caminho absoluto do diretorio da aplicacao.    */
    path = path + argv0;
    path.Replace(_T("/./"), _T("/"));
    path = path.BeforeLast('/');
    path.Replace(_T("/bin"), _T(""));

    char *path_c = (char *) malloc(512 * sizeof(char));
    strncpy(path_c, path.mb_str(wxConvUTF8), 450);

    return path_c;
}

/* ------------------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------------------- */
