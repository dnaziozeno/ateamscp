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

#ifndef GRAPH_H
#define GRAPH_H

/* ------------------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------------------- */

#include <wx/wx.h>
#include <wx/statline.h>

/* ------------------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------------------- */

class MyCanvas:public wxScrolledWindow {

public:
    MyCanvas(wxWindow *parent, const wxPoint& pos, const wxSize& size);

    virtual void OnDraw(wxDC& dc);
    void Plot1(wxDC& dc,int w_,int h_);
    void Plot2(wxDC& dc,int w_,int h_);
};

class Graph:public wxFrame {

public:
    Graph(
        wxWindow *parent, const wxWindowID id, const wxString& title,
        const wxPoint& pos, const wxSize& size
    );

    void OnClose(wxCloseEvent& event);
    void onCloseWindow(wxCommandEvent &event);

private:
    void set_properties();
    void do_layout();

protected:
    wxStatusBar* plot_statusbar;
    wxButton* configure_button;
    wxButton* hide_button;
    wxStaticLine* button_static_line;
    wxPanel* plot_panel;
    wxStaticLine* panel_static_line;
    wxTextCtrl* report_text_ctrl;
};

/* ------------------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------------------- */

#endif /* GRAPH_H */
