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

#include <wx/statline.h>
#include <wx/wx.h>
#include <wx/mdi.h>
#include <wxPlot/wxPlot.h>

#include "Graph.h"

#define PI 3.1415926535
#define PERIMETERPTS 100
#define LEVELS 10

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
Graph::Graph(
    wxWindow *parent, const wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size
):wxFrame(
    parent, id, title, pos, size, wxICONIZE|wxCAPTION|wxMINIMIZE|wxCLOSE_BOX|wxMINIMIZE_BOX
)
{
    plot_statusbar = CreateStatusBar(1, 0);
    configure_button = new wxButton(this, wxID_ANY, wxT("Configure"));
    hide_button = new wxButton(this, wxID_ANY, wxT("Hide"));
    button_static_line = new wxStaticLine(this, wxID_ANY);
    plot_panel = new wxPanel(this, wxID_ANY);
    panel_static_line = new wxStaticLine(this, wxID_ANY);
    report_text_ctrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_MULTILINE|wxHSCROLL);

    set_properties();
    do_layout();

    Connect(wxEVT_CLOSE_WINDOW, wxCommandEventHandler(Graph::onCloseWindow));

    MyCanvas *canvas = new MyCanvas(this, wxPoint(4, 45), wxSize(700, 500));
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void Graph::set_properties()
{
    SetTitle(wxT("Charts"));

    int plot_statusbar_widths[] = { -1 };
    plot_statusbar->SetStatusWidths(1, plot_statusbar_widths);
    const wxString plot_statusbar_fields[] = { wxEmptyString };

    for(int i = 0; i < plot_statusbar->GetFieldsCount(); ++i) {
        plot_statusbar->SetStatusText(plot_statusbar_fields[i], i);
    }

    configure_button->SetMinSize(wxSize(100, 27));
    hide_button->SetMinSize(wxSize(100, 27));
    button_static_line->SetMinSize(wxSize(708, 2));
    plot_panel->SetMinSize(wxSize(700,500));
    panel_static_line->SetMinSize(wxSize(704, 2));
    report_text_ctrl->SetMinSize(wxSize(700, 140));
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void Graph::do_layout()
{
    wxFlexGridSizer* main_grid_sizer = new wxFlexGridSizer(3, 1, 2, 2);
    wxFlexGridSizer* down_grid_sizer = new wxFlexGridSizer(2, 1, 2, 2);
    wxFlexGridSizer* plot_grid_sizer = new wxFlexGridSizer(2, 1, 2, 2);
    wxFlexGridSizer* top_grid_sizer = new wxFlexGridSizer(2, 1, 2, 2);
    wxFlexGridSizer* button_grid_sizer = new wxFlexGridSizer(1, 5, 2, 2);

    button_grid_sizer->Add(configure_button, 0, 0, 0);
    button_grid_sizer->Add(hide_button, 0, 0, 0);
    top_grid_sizer->Add(button_grid_sizer, 1, wxALL|wxEXPAND, 4);
    top_grid_sizer->Add(button_static_line, 0, wxEXPAND, 0);
    main_grid_sizer->Add(top_grid_sizer, 1, wxEXPAND, 0);
    plot_grid_sizer->Add(plot_panel, 1, wxALL|wxEXPAND, 4);
    plot_grid_sizer->Add(panel_static_line, 0, wxALL|wxEXPAND, 2);
    down_grid_sizer->Add(plot_grid_sizer, 1, wxEXPAND, 0);
    main_grid_sizer->Add(down_grid_sizer, 1, wxEXPAND, 0);
    main_grid_sizer->Add(report_text_ctrl, 0, wxALL, 4);

    SetSizer(main_grid_sizer);
    main_grid_sizer->Fit(this);
    Layout();
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void Graph::onCloseWindow(wxCommandEvent &event)
{
    this->Show(false);
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
MyCanvas::MyCanvas(
    wxWindow *parent, const wxPoint& pos, const wxSize& size
):wxScrolledWindow(
    parent, -1, pos, size, wxSUNKEN_BORDER | wxNO_FULL_REPAINT_ON_RESIZE)
{
    SetBackgroundColour(wxColour(_T("BLACK")));
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void MyCanvas::OnDraw(wxDC& dc)
{
    wxBrush brush(*wxBLACK_BRUSH);
    dc.SetBackground(brush);
    Plot1(dc,0,0);
    Plot2(dc,0,0);
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void MyCanvas::Plot1(wxDC& dc, int w_, int h_)
{
  wxPlot *wxplot;
  int w,h;
  static PLFLT x[101], y[101];
  static PLFLT x1[101], y1[101];

  this->GetSize(&w,&h);

  if(w_!=0 && h_!=0)
    {
      w=w_;
      h=h_;
    }
  
  wxplot=new wxPlot(&dc, w, h);

  PLINT space0 = 0, mark0 = 0, space1 = 1500, mark1 = 1500;
  int i;

/* For the final graph we wish to override the default tick intervals, and
 * so do not use plenv().
*/
  wxplot->adv(0);

/* Use standard viewport, and define X range from 0 to 360 degrees, Y range
 * from -1.2 to 1.2. 
*/
  wxplot->vsta();
  wxplot->wind(0.0, 400, -10, 10);
  
/* Draw a box with ticks spaced 60 degrees apart in X, and 0.2 in Y. */

  wxplot->col0(5);
    
  wxplot->box("bcnstv", 50, 2, "bcnstv", 2, 2);

/* Superimpose a dashed line grid, with 1.5 mm marks and spaces. 
 * plstyl expects a pointer!
*/
  wxplot->styl(1, &mark1, &space1);
  wxplot->col0(5);
  wxplot->box("g", 20.0, 0, "g", 2, 0);
  wxplot->styl(0, &mark0, &space0);

  wxplot->col0(5);
  wxplot->lab("Time (seconds)", "? Solutions ?", "Graph of Solutions");

    for (i = 0; i < 101; i++) {
      x[i] = 3.6 * i;
      y[i] = sin(x[i] * PI / 180.0);

      x1[i] = (i) * 10;
      y1[i] = (2 * i);
    }

  wxplot->col0(3);
  wxplot->line(101, x, y);

  //wxplot->col0(2);
  //wxplot->line(101, x1, y1);  

  delete(wxplot);
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void MyCanvas::Plot2(wxDC& dc, int w_, int h_)
{
  wxPlot *wxplot;
  int w,h;
  static PLFLT x[101], y[101];
  static PLFLT x1[101], y1[101];

  this->GetSize(&w,&h);

  if(w_!=0 && h_!=0)
    {
      w=w_;
      h=h_;
    }
  
  wxplot=new wxPlot(&dc, w, h);

  PLINT space0 = 0, mark0 = 0, space1 = 1500, mark1 = 1500;
  int i;

/* For the final graph we wish to override the default tick intervals, and
 * so do not use plenv().
*/
  wxplot->adv(0);

/* Use standard viewport, and define X range from 0 to 360 degrees, Y range
 * from -1.2 to 1.2. 
*/
  wxplot->vsta();
  wxplot->wind(0.0, 400, -10, 10);
  
/* Draw a box with ticks spaced 60 degrees apart in X, and 0.2 in Y. */

  wxplot->col0(5);
    
  wxplot->box("bcnstv", 50, 2, "bcnstv", 2, 2);

/* Superimpose a dashed line grid, with 1.5 mm marks and spaces. 
 * plstyl expects a pointer!
*/
  wxplot->styl(1, &mark1, &space1);
  wxplot->col0(5);
  wxplot->box("g", 20.0, 0, "g", 2, 0);
  wxplot->styl(0, &mark0, &space0);

  wxplot->col0(5);
  wxplot->lab("Time (seconds)", "? Solutions ?", "Graph of Solutions");

    for (i = 0; i < 101; i++) {
      x[i] = 3.6 * i;
      y[i] = sin(x[i] * PI / 180.0);

      x1[i] = (i) * 10;
      y1[i] = (2 * i);
    }

  wxplot->col0(2);
  wxplot->line(101, x1, y1);  

  delete(wxplot);
}
