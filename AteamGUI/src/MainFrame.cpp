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

#include "MainFrame.h"
#include "AteamParam.h"

/* ------------------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------------------- */

/*
BEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_BUTTON(11, MainFrame::on_start_click)
END_EVENT_TABLE()
*/

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
MainFrame::MainFrame(wxWindow* parent, int id, const wxString& title, const wxPoint& pos, const wxSize& size, long style):
    wxFrame(parent, id, title, pos, size, wxDEFAULT_FRAME_STYLE)
{
    main_frame_statusbar = CreateStatusBar(1, 0);

    start_button = new wxButton(this, 11, _("Start"));
    stop_button = new wxButton(this, 12, _("Stop"));
    plot_button = new wxButton(this, 13, _("Plot"));
    add_button = new wxButton(this, 14, _("Add"));
    remove_button = new wxButton(this, 15, _("Remove"));
    edit_button = new wxButton(this, 16, _("Edit"));

    Connect(11, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::on_start_click));
    Connect(12, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::on_stop_click));
    Connect(13, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::on_plot_click));
    Connect(14, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::on_add_click));
    Connect(15, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::on_remove_click));
    Connect(16, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::on_edit_click));

    button_static_line = new wxStaticLine(this, wxID_ANY);

    team_tree_ctrl = new wxTreeCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE);
    team_tree_ctrl->SetWindowStyle(wxTR_HAS_BUTTONS|wxSUNKEN_BORDER);

/*************************************************************************************************/
    wxTreeItemId id_root = team_tree_ctrl->AddRoot(_T("A-Team Set Covering Problem"), -1, -1, NULL);

    wxTreeItemId idy;
    idy = team_tree_ctrl->InsertItem(id_root, NULL, _T("Agent-01 | 127.0.0.1 | 12:00"), -1, -1, NULL);
    idy = team_tree_ctrl->InsertItem(id_root, NULL, _T("Agent-02 | 127.0.0.1 | 12:00"), -1, -1, NULL);
    idy = team_tree_ctrl->InsertItem(id_root, NULL, _T("Agent-03 | 127.0.0.1 | 12:00"), -1, -1, NULL);
    idy = team_tree_ctrl->InsertItem(id_root, NULL, _T("Agent-04 | 127.0.0.1 | 12:00"), -1, -1, NULL);
    idy = team_tree_ctrl->InsertItem(id_root, NULL, _T("Agent-05 | 127.0.0.1 | 12:00"), -1, -1, NULL);
    idy = team_tree_ctrl->InsertItem(id_root, NULL, _T("Agent-06 | 127.0.0.1 | 12:00"), -1, -1, NULL);
    idy = team_tree_ctrl->InsertItem(id_root, NULL, _T("Agent-07 | 127.0.0.1 | 12:00"), -1, -1, NULL);
    idy = team_tree_ctrl->InsertItem(id_root, NULL, _T("Agent-08 | 127.0.0.1 | 12:00"), -1, -1, NULL);
    idy = team_tree_ctrl->InsertItem(id_root, NULL, _T("Agent-09 | 127.0.0.1 | 12:00"), -1, -1, NULL);
    idy = team_tree_ctrl->InsertItem(id_root, NULL, _T("Agent-10 | 127.0.0.1 | 12:00"), -1, -1, NULL);
    idy = team_tree_ctrl->InsertItem(id_root, NULL, _T("Agent-11 | 127.0.0.1 | 12:00"), -1, -1, NULL);
    idy = team_tree_ctrl->InsertItem(id_root, NULL, _T("Agent-12 | 127.0.0.1 | 12:00"), -1, -1, NULL);
    idy = team_tree_ctrl->InsertItem(id_root, NULL, _T("Agent-13 | 127.0.0.1 | 12:00"), -1, -1, NULL);
    idy = team_tree_ctrl->InsertItem(id_root, NULL, _T("Agent-14 | 127.0.0.1 | 12:00"), -1, -1, NULL);
    idy = team_tree_ctrl->InsertItem(id_root, NULL, _T("Agent-15 | 127.0.0.1 | 12:00"), -1, -1, NULL);
    idy = team_tree_ctrl->InsertItem(id_root, NULL, _T("Agent-16 | 127.0.0.1 | 12:00"), -1, -1, NULL);
    idy = team_tree_ctrl->InsertItem(id_root, NULL, _T("Agent-17 | 127.0.0.1 | 12:00"), -1, -1, NULL);
    idy = team_tree_ctrl->InsertItem(id_root, NULL, _T("Agent-18 | 127.0.0.1 | 12:00"), -1, -1, NULL);
    idy = team_tree_ctrl->InsertItem(id_root, NULL, _T("Agent-19 | 127.0.0.1 | 12:00"), -1, -1, NULL);
    idy = team_tree_ctrl->InsertItem(id_root, NULL, _T("Agent-20 | 127.0.0.1 | 12:00"), -1, -1, NULL);
    idy = team_tree_ctrl->InsertItem(id_root, NULL, _T("Agent-21 | 127.0.0.1 | 12:00"), -1, -1, NULL);
    idy = team_tree_ctrl->InsertItem(id_root, NULL, _T("Agent-22 | 127.0.0.1 | 12:00"), -1, -1, NULL);
    idy = team_tree_ctrl->InsertItem(id_root, NULL, _T("Agent-23 | 127.0.0.1 | 12:00"), -1, -1, NULL);
    idy = team_tree_ctrl->InsertItem(id_root, NULL, _T("Agent-24 | 127.0.0.1 | 12:00"), -1, -1, NULL);
    idy = team_tree_ctrl->InsertItem(id_root, NULL, _T("Agent-25 | 127.0.0.1 | 12:00"), -1, -1, NULL);
    idy = team_tree_ctrl->InsertItem(id_root, NULL, _T("Agent-26 | 127.0.0.1 | 12:00"), -1, -1, NULL);
    idy = team_tree_ctrl->InsertItem(id_root, NULL, _T("Agent-27 | 127.0.0.1 | 12:00"), -1, -1, NULL);
    idy = team_tree_ctrl->InsertItem(id_root, NULL, _T("Agent-28 | 127.0.0.1 | 12:00"), -1, -1, NULL);
/*************************************************************************************************/




    set_properties();
    do_layout();
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void MainFrame::set_properties()
{
    SetTitle(_("A-Team SCP"));
    int main_frame_statusbar_widths[] = { -1 };
    main_frame_statusbar->SetStatusWidths(1, main_frame_statusbar_widths);
    const wxString main_frame_statusbar_fields[] = {
        _("main_frame_statusbar")
    };
    for(int i = 0; i < main_frame_statusbar->GetFieldsCount(); ++i) {
        main_frame_statusbar->SetStatusText(main_frame_statusbar_fields[i], i);
    }
    start_button->SetMinSize(wxSize(95, 27));
    stop_button->SetMinSize(wxSize(95, 27));
    plot_button->SetMinSize(wxSize(95, 27));
    add_button->SetMinSize(wxSize(95, 27));
    remove_button->SetMinSize(wxSize(95, 27));
    edit_button->SetMinSize(wxSize(95, 27));
    team_tree_ctrl->SetMinSize(wxSize(291, 500));
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void MainFrame::do_layout()
{
    wxFlexGridSizer* main_grid_sizer = new wxFlexGridSizer(3, 1, 0, 0);
    wxFlexGridSizer* button_grid_sizer = new wxFlexGridSizer(2, 3, 4, 4);
    button_grid_sizer->Add(start_button, 0, 0, 0);
    button_grid_sizer->Add(stop_button, 0, 0, 0);
    button_grid_sizer->Add(plot_button, 0, 0, 0);
    button_grid_sizer->Add(add_button, 0, 0, 0);
    button_grid_sizer->Add(remove_button, 0, 0, 0);
    button_grid_sizer->Add(edit_button, 0, 0, 0);
    main_grid_sizer->Add(button_grid_sizer, 1, wxALL|wxEXPAND, 3);
    main_grid_sizer->Add(button_static_line, 0, wxALL|wxEXPAND, 4);
    main_grid_sizer->Add(team_tree_ctrl, 1, wxALL|wxEXPAND, 4);
    SetSizer(main_grid_sizer);
    main_grid_sizer->Fit(this);
    Layout();
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void MainFrame::on_start_click(wxCommandEvent &event)
{
    AteamParam *ateam_param = new AteamParam(NULL, wxID_ANY, wxEmptyString, wxPoint(400, 70));
    ateam_param->Show();
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void MainFrame::on_stop_click(wxCommandEvent &event)
{
    printf("Button Stop Clicked...\n");
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void MainFrame::on_plot_click(wxCommandEvent &event)
{
    printf("Button Plot Clicked...\n");
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void MainFrame::on_add_click(wxCommandEvent &event)
{
    printf("Button Add Clicked...\n");
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void MainFrame::on_remove_click(wxCommandEvent &event)
{
    printf("Button Remove Clicked...\n");
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void MainFrame::on_edit_click(wxCommandEvent &event)
{
    printf("Button Edit Clicked...\n");
}

/* ------------------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------------------- */
