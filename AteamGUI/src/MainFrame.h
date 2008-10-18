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
/* Alteracoes: 0 -> (10/09/94)                                                           */
/* ------------------------------------------------------------------------------------- */

#ifndef MAINFRAME_H
#define MAINFRAME_H

/* ------------------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------------------- */

#include <wx/wx.h>
#include <wx/image.h>
#include <wx/treectrl.h>
#include <wx/statline.h>

#include <wx/points.h>
#include <wx/barchartpoints.h>
#include <wx/bar3dchartpoints.h>
#include <wx/chartctrl.h>
#include <wx/chartcolors.h>
#include <wx/piechartpoints.h>
#include <wx/pie3dchartpoints.h>
#include <wx/chart.h>

#include <wx/datetime.h>

#include <mpi.h>

#define THREE_OPT 1
#define SUBG 2
#define LS 3
#define PERT 4
#define CONS 5

/* ------------------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------------------- */
enum
{
    ID_BUTTON_START
};

class MainFrame: public wxFrame {

public:
    MainFrame(
        wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition,
        const wxSize& size=wxDefaultSize, long style=wxDEFAULT_FRAME_STYLE
    );

    void onApplyClick();
    void onAddAgent(int type, wxString ip, int mpi_ref, int *params);
    void onRemoveAgent(int type, int mpi_ref);

    void initMD();
    void ExecAgInitMD(MPI_Comm communicator);

private:
    void set_properties();
    void do_layout();

    void onStartClick(wxCommandEvent &event);
    void onStopClick(wxCommandEvent &event);
    void onPlotClick(wxCommandEvent &event);
    void onAddClick(wxCommandEvent &event);
    void onRemoveClick(wxCommandEvent &event);
    void onEditClick(wxCommandEvent &event);

    void refresh_statistics();
    wxString getTime();

protected:
    wxStatusBar *main_frame_statusbar;

    wxButton *start_button, *stop_button, *plot_button,
        *add_button, *remove_button, *edit_button;

    wxStaticLine *button_static_line;
    wxTreeCtrl *team_tree_ctrl;
};

/* ------------------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------------------- */

class MyTreeItemData: public wxTreeItemData {

public:
    MyTreeItemData(int type, int *params);
    void GetId();
    void SetId();
    int GetType();
    void SetParams(int *params);
    int *GetParams();

private:

protected:
    int type_t;
    int *params_t;
};



#endif /* MAINFRAME_H */



