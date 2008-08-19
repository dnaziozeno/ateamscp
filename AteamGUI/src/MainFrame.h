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

/* ------------------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------------------- */

#include <wx/wx.h>
#include <wx/image.h>

/* ------------------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------------------- */

#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <wx/treectrl.h>
#include <wx/statline.h>

enum
{
    ID_BUTTON_START
};

class MainFrame: public wxFrame {

public:
    MainFrame(wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_FRAME_STYLE);

private:
    void set_properties();
    void do_layout();
    void on_start_click(wxCommandEvent &event);
    void on_stop_click(wxCommandEvent &event);
    void on_plot_click(wxCommandEvent &event);
    void on_add_click(wxCommandEvent &event);
    void on_remove_click(wxCommandEvent &event);
    void on_edit_click(wxCommandEvent &event);
    //DECLARE_EVENT_TABLE()


protected:
    wxStatusBar* main_frame_statusbar;
    wxButton* start_button;
    wxButton* stop_button;
    wxButton* plot_button;
    wxButton* add_button;
    wxButton* remove_button;
    wxButton* edit_button;
    wxStaticLine* button_static_line;
    wxTreeCtrl* team_tree_ctrl;

};

#endif // MAINFRAME_H

/* ------------------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------------------- */
