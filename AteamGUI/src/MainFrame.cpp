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
#include "../include/InitMemories.h"
#include "../include/AteamParam.h"


extern "C" {
    #include "initMD.c"
}

/* ------------------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
MainFrame::MainFrame(
    wxWindow* parent, int id, const wxString& title, const wxPoint& pos, const wxSize& size, long style
):wxFrame(
    parent, id, title, pos, size, wxICONIZE|wxCAPTION|wxMINIMIZE|wxCLOSE_BOX|wxMINIMIZE_BOX
)
{
   // MPI_Init(0, NULL);

    main_frame_statusbar = CreateStatusBar(1, 0);

    start_button = new wxButton(this, 11, _("Start"));
    stop_button = new wxButton(this, 12, _("Stop"));
    plot_button = new wxButton(this, 13, _("Plot"));
    add_button = new wxButton(this, 14, _("Add"));
    remove_button = new wxButton(this, 15, _("Remove"));
    edit_button = new wxButton(this, 16, _("Edit"));

    stop_button->Disable();
    plot_button->Disable();
    add_button->Disable();
    remove_button->Disable();
    edit_button->Disable();

    Connect(11, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::onStartClick));
    Connect(12, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::onStopClick));
    Connect(13, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::onPlotClick));
    Connect(14, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::onAddClick));
    Connect(15, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::onRemoveClick));
    Connect(16, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::onEditClick));

    button_static_line = new wxStaticLine(this, wxID_ANY);

    team_tree_ctrl = new wxTreeCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE);
    team_tree_ctrl->SetWindowStyle(wxTR_HAS_BUTTONS|wxSUNKEN_BORDER);

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
        _("")
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
void MainFrame::onStartClick(wxCommandEvent &event)
{
    //InitMemories *init_memories = new InitMemories(this, wxID_ANY, wxEmptyString, wxPoint(100, 250));
    initMD();

    //init_memories->Show();
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void MainFrame::onStopClick(wxCommandEvent &event)
{
    stop_button->Disable();
    plot_button->Disable();
    add_button->Disable();
    remove_button->Disable();
    edit_button->Disable();
    start_button->Enable();

    team_tree_ctrl->DeleteAllItems();

    MPI_Comm interComSpawn1, interComSpawn2, interComSpawn3;
    int errcodes[1];

    MPI_Comm_spawn(
        "/home/naziozeno/Documents/projeto-final/AteamSCP/AteamGUI/src/stopATEAM",
        MPI_ARGV_NULL, 1, MPI_INFO_NULL, 0,  MPI_COMM_SELF, &interComSpawn1, errcodes
    );

    MPI_Comm_spawn(
        "/home/naziozeno/Documents/projeto-final/AteamSCP/AteamGUI/src/stopSERVER",
        MPI_ARGV_NULL, 1, MPI_INFO_NULL, 0,  MPI_COMM_SELF, &interComSpawn2, errcodes
    );

    MPI_Comm_spawn(
        "/home/naziozeno/Documents/projeto-final/AteamSCP/AteamGUI/src/stopFINAL",
        MPI_ARGV_NULL, 1, MPI_INFO_NULL, 0,  MPI_COMM_SELF, &interComSpawn3, errcodes
    );

    MPI_Finalize();
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void MainFrame::onPlotClick(wxCommandEvent &event)
{
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void MainFrame::onAddClick(wxCommandEvent &event)
{
    wxString input;
    input = wxGetTextFromUser(_T("Agent name:"), _T("Input Agent"), _T(""), this);

    //"Agent-01 | 127.0.0.1 | 12:00"

    wxTreeItemId idy;
    idy = team_tree_ctrl->InsertItem(team_tree_ctrl->GetRootItem(), NULL, input, -1, -1, NULL);

    // ACHAR UM LUGAR!
    team_tree_ctrl->ExpandAll();
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void MainFrame::onRemoveClick(wxCommandEvent &event)
{
    wxTreeItemId item = team_tree_ctrl->GetSelection();
    team_tree_ctrl->Delete(item);
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void MainFrame::onEditClick(wxCommandEvent &event)
{
    wxTreeItemId item = team_tree_ctrl->GetSelection();

    wxString input = wxGetTextFromUser(
        _T("Agent name:"), _T("Rename Agent"),
        team_tree_ctrl->GetItemText(item), this
    );

    team_tree_ctrl->SetItemText(item, input);
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void MainFrame::onApplyClick()
{
    team_tree_ctrl->AddRoot(_T("A-Team Set Covering Problem"), -1, -1, NULL);

    stop_button->Enable();
    //plot_button->Enable();
    //add_button->Enable();
    //remove_button->Enable();
    //edit_button->Enable();

    //start_button->Disable();
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void MainFrame::initMD()
{
    char *args[4];


    args[0] = (char *) malloc (512 * sizeof(char));
    args[1] = (char *) malloc (512 * sizeof(char));
    args[2] = (char *) malloc (512 * sizeof(char));
    args[3] = (char *) malloc (512 * sizeof(char));

    strcpy(args[0], "/home/naziozeno/Documents/projeto-final/AteamSCP/AteamGUI/src/initMD");
    strcpy(args[1], "1");
    strcpy(args[2], "/home/naziozeno/Documents/projeto-final/AteamSCP/AteamGUI/src/20_40_20");
    strcpy(args[3], "/home/naziozeno/Documents/projeto-final/AteamSCP/AteamGUI/src/teste");

    int com = InitMD(4, args);
    MPI_Comm com1 = (MPI_Comm) com;

    //MODIFICAR DEPOIS...
    onApplyClick();

}

/* ------------------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------------------- */
