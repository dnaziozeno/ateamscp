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

#include "InitMemories.h"
#include "AteamParam.h"
#include "../../../include/MainFrame.h"
#include "../../../include/IOParam.h"

#define THREE_OPT 1
#define SUBG 2
#define LS 3
#define PERT 4
#define CONS 5
#define PRIMAL 6
#define DUAL 7

/* ------------------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------------------- */

AteamParam *init_md_ateam_param, *three_opt_ateam_param,
    *subg_ateam_param, *init_mp_ateam_param, *ls_ateam_param,
    *pert_ateam_param, *cons_ateam_param;

MPI_Comm comm_three_opt;

bool start_team = false;

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
InitMemories::InitMemories(
    wxWindow* parent, int id, const wxString& title, const wxPoint& pos, const wxSize& size, long style
):wxFrame(
    parent, id, title, pos, size, wxICONIZE|wxCAPTION|wxMINIMIZE|wxCLOSE_BOX|wxMINIMIZE_BOX
)
{
    init_memories_statusbar = CreateStatusBar(1, 0);

    init_md_button = new wxButton(this, 121, _("Init MD"));
    const wxString init_md_combo_box_choices[] = {_("127.0.0.1")};
    init_md_combo_box = new wxComboBox(
        this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize,
        1, init_md_combo_box_choices, wxCB_DROPDOWN
    );
    init_md_configure_button = new wxButton(this, 122, _("Configure"));

    three_opt_button = new wxButton(this, 123, _("Add 3OPT"));
    const wxString three_opt_combo_box_choices[] = {_("127.0.0.1")};
    three_opt_combo_box = new wxComboBox(
        this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize,
        1, three_opt_combo_box_choices, wxCB_DROPDOWN
    );
    three_opt_configure_button = new wxButton(this, 124, _("Configure"));

    subg_button = new wxButton(this, 125, _("Add SUBG"));
    const wxString subg_combo_box_choices[] = {_("127.0.0.1")};
    subg_combo_box = new wxComboBox(
        this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize,
        1, subg_combo_box_choices, wxCB_DROPDOWN
    );
    subg_configure_button = new wxButton(this, 126, _("Configure"));

    mid_static_line = new wxStaticLine(this, wxID_ANY);
    init_mp_button = new wxButton(this, 127, _("Init MP"));
    const wxString init_mp_combo_box_choices[] = {_("127.0.0.1")};
    init_mp_combo_box = new wxComboBox(
        this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize,
        1, init_mp_combo_box_choices, wxCB_DROPDOWN
    );
    init_mp_configure_button = new wxButton(this, 128, _("Configure"));

    ls_button = new wxButton(this, 129, _("Add LS"));
    const wxString ls_combo_box_choices[] = {_("127.0.0.1")};
    ls_combo_box = new wxComboBox(
        this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize,
        1, ls_combo_box_choices, wxCB_DROPDOWN
    );
    ls_configure_button = new wxButton(this, 130, _("Configure"));

    pert_button = new wxButton(this, 131, _("Add PERT"));
    const wxString pert_combo_box_choices[] = {_("127.0.0.1")};
    pert_combo_box = new wxComboBox(
        this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize,
        1, pert_combo_box_choices, wxCB_DROPDOWN
    );
    pert_configure_button = new wxButton(this, 132, _("Configure"));

    cons_button = new wxButton(this, 133, _("Add CONS"));
    const wxString cons_combo_box_choices[] = {_("127.0.0.1")};
    cons_combo_box = new wxComboBox(
        this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize,
        1, cons_combo_box_choices, wxCB_DROPDOWN
    );
    cons_configure_button = new wxButton(this, 134, _("Configure"));

    set_properties();
    do_layout();

    three_opt_button->Disable();
    three_opt_combo_box->Disable();
    three_opt_configure_button->Disable();
    subg_button->Disable();
    subg_combo_box->Disable();
    subg_configure_button->Disable();

    ls_button->Disable();
    ls_combo_box->Disable();
    ls_configure_button->Disable();
    pert_button->Disable();
    pert_combo_box->Disable();
    pert_configure_button->Disable();
    cons_button->Disable();
    cons_combo_box->Disable();
    cons_configure_button->Disable();

    Connect(121, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(InitMemories::onInitMDClick));
    Connect(122, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(InitMemories::onConfigureInitMDClick));
    Connect(123, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(InitMemories::onThreeOPTClick));
    Connect(124, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(InitMemories::onConfigureThreeOPTClick));
    Connect(125, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(InitMemories::onSubGClick));
    Connect(126, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(InitMemories::onConfigureSubGClick));
    Connect(127, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(InitMemories::onInitMPClick));
    Connect(128, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(InitMemories::onConfigureInitMPClick));
    Connect(129, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(InitMemories::onLSClick));
    Connect(130, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(InitMemories::onConfigureLSClick));
    Connect(131, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(InitMemories::onPertClick));
    Connect(132, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(InitMemories::onConfigurePertClick));
    Connect(133, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(InitMemories::onConsClick));
    Connect(134, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(InitMemories::onConfigureConsClick));

    init_md_ateam_param = new AteamParam(this, wxID_ANY, wxEmptyString, wxPoint(420, 200));
    three_opt_ateam_param = new AteamParam(this, wxID_ANY, wxEmptyString, wxPoint(420, 200));
    subg_ateam_param = new AteamParam(this, wxID_ANY, wxEmptyString, wxPoint(420, 200));
    init_mp_ateam_param = new AteamParam(this, wxID_ANY, wxEmptyString, wxPoint(420, 200));
    ls_ateam_param = new AteamParam(this, wxID_ANY, wxEmptyString, wxPoint(420, 200));
    pert_ateam_param = new AteamParam(this, wxID_ANY, wxEmptyString, wxPoint(420, 200));
    cons_ateam_param = new AteamParam(this, wxID_ANY, wxEmptyString, wxPoint(420, 200));
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void InitMemories::set_properties()
{
    SetTitle(_("InitMemories"));

    int init_memories_statusbar_widths[] = { -1 };
    init_memories_statusbar->SetStatusWidths(1, init_memories_statusbar_widths);
    const wxString init_memories_statusbar_fields[] = {_("")};
    for(int i = 0; i < init_memories_statusbar->GetFieldsCount(); ++i) {
        init_memories_statusbar->SetStatusText(init_memories_statusbar_fields[i], i);
    }

    init_md_button->SetMinSize(wxSize(90, 27));
    init_md_combo_box->SetMinSize(wxSize(181, 27));
    init_md_combo_box->SetSelection(-1);
    init_md_configure_button->SetMinSize(wxSize(90, 27));

    three_opt_button->SetMinSize(wxSize(90, 27));
    three_opt_combo_box->SetMinSize(wxSize(181, 27));
    three_opt_combo_box->SetSelection(-1);
    three_opt_configure_button->SetMinSize(wxSize(90, 27));

    subg_button->SetMinSize(wxSize(90, 27));
    subg_combo_box->SetMinSize(wxSize(181, 27));
    subg_combo_box->SetSelection(-1);
    subg_configure_button->SetMinSize(wxSize(90, 27));

    init_mp_button->SetMinSize(wxSize(90, 27));
    init_mp_combo_box->SetMinSize(wxSize(181, 27));
    init_mp_combo_box->SetSelection(-1);
    init_mp_configure_button->SetMinSize(wxSize(90, 27));

    ls_button->SetMinSize(wxSize(90, 27));
    ls_combo_box->SetMinSize(wxSize(181, 27));
    ls_combo_box->SetSelection(-1);
    ls_configure_button->SetMinSize(wxSize(90, 27));

    pert_button->SetMinSize(wxSize(90, 27));
    pert_combo_box->SetMinSize(wxSize(181, 27));
    pert_combo_box->SetSelection(-1);
    pert_configure_button->SetMinSize(wxSize(90, 27));

    cons_button->SetMinSize(wxSize(90, 27));
    cons_combo_box->SetMinSize(wxSize(181, 27));
    cons_combo_box->SetSelection(-1);
    cons_configure_button->SetMinSize(wxSize(90, 27));
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void InitMemories::do_layout()
{
    wxFlexGridSizer* main_grid_sizer = new wxFlexGridSizer(5, 1, 2, 2);
    wxFlexGridSizer* down_grid_sizer = new wxFlexGridSizer(4, 3, 5, 3);
    wxFlexGridSizer* mid_buttons_grid_sizer = new wxFlexGridSizer(3, 3, 5, 3);

    mid_buttons_grid_sizer->Add(init_md_button, 0, wxTOP, 4);
    mid_buttons_grid_sizer->Add(init_md_combo_box, 0, wxTOP, 4);
    mid_buttons_grid_sizer->Add(init_md_configure_button, 0, wxTOP, 4);
    mid_buttons_grid_sizer->Add(three_opt_button, 0, 0, 0);
    mid_buttons_grid_sizer->Add(three_opt_combo_box, 0, 0, 0);
    mid_buttons_grid_sizer->Add(three_opt_configure_button, 0, 0, 0);
    mid_buttons_grid_sizer->Add(subg_button, 0, 0, 0);
    mid_buttons_grid_sizer->Add(subg_combo_box, 0, 0, 0);
    mid_buttons_grid_sizer->Add(subg_configure_button, 0, 0, 0);

    main_grid_sizer->Add(mid_buttons_grid_sizer, 1, wxALL|wxEXPAND, 2);
    main_grid_sizer->Add(mid_static_line, 0, wxALL|wxEXPAND, 2);

    down_grid_sizer->Add(init_mp_button, 0, 0, 0);
    down_grid_sizer->Add(init_mp_combo_box, 0, 0, 0);
    down_grid_sizer->Add(init_mp_configure_button, 0, 0, 0);
    down_grid_sizer->Add(ls_button, 0, 0, 0);
    down_grid_sizer->Add(ls_combo_box, 0, 0, 0);
    down_grid_sizer->Add(ls_configure_button, 0, 0, 0);
    down_grid_sizer->Add(pert_button, 0, 0, 0);
    down_grid_sizer->Add(pert_combo_box, 0, 0, 0);
    down_grid_sizer->Add(pert_configure_button, 0, 0, 0);
    down_grid_sizer->Add(cons_button, 0, wxBOTTOM, 4);
    down_grid_sizer->Add(cons_combo_box, 0, wxBOTTOM, 4);
    down_grid_sizer->Add(cons_configure_button, 0, 0, 0);

    main_grid_sizer->Add(down_grid_sizer, 1, wxALL|wxEXPAND, 2);
    SetSizer(main_grid_sizer);

    main_grid_sizer->Fit(this);
    Layout();
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void InitMemories::onInitMDClick(wxCommandEvent &event)
{
    MainFrame *main_frame = (MainFrame *) this->GetParent();

    int argcInitMD = 4;
    char *argvInitMD[4];

    argvInitMD[0] = (char *) malloc (512 * sizeof(char));
    argvInitMD[1] = (char *) malloc (512 * sizeof(char));
    argvInitMD[2] = (char *) malloc (512 * sizeof(char));
    argvInitMD[3] = NULL;

    strcpy(argvInitMD[0], "1");

    strcpy(argvInitMD[1], "/home/naziozeno/Documents/projeto-final/AteamSCP/20_40_20");
    strcpy(argvInitMD[2], "/home/naziozeno/Documents/projeto-final/AteamSCP/teste/");

    MPI_Comm com2;
    int errcodes[1];

    MPI_Init(NULL, NULL);

    MPI_Comm_spawn(
        "/home/naziozeno/Documents/projeto-final/AteamSCP/AteamMPI/initMD",
        argvInitMD, 1, MPI_INFO_NULL, 0,  MPI_COMM_SELF, &com2, errcodes
    );

    three_opt_button->Enable();
    three_opt_combo_box->Enable();
    three_opt_configure_button->Enable();

    init_md_button->Disable();
    init_md_combo_box->Disable();
    init_md_configure_button->Disable();

    if (!start_team)
    {
        main_frame->onApplyClick();
        start_team = true;
    }
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void InitMemories::onConfigureInitMDClick(wxCommandEvent &event)
{
    init_md_ateam_param->Show();
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void InitMemories::onThreeOPTClick(wxCommandEvent &event)
{
    int argcInitMD = 4;
    char *argvInitMD[4];

    argvInitMD[0] = (char *) malloc (512 * sizeof(char));
    argvInitMD[1] = (char *) malloc (512 * sizeof(char));
    argvInitMD[2] = NULL;

    strcpy(argvInitMD[0], "1");
    strcpy(argvInitMD[1], "/home/naziozeno/Documents/projeto-final/AteamSCP/20_40_20");

    int errcodes[1];

    MPI_Comm *comm = (MPI_Comm *) malloc(sizeof(MPI_Comm));

    MPI_Comm_spawn(
        "/home/naziozeno/Documents/projeto-final/AteamSCP/AteamMPI/ag_3opt",
        argvInitMD, 1, MPI_INFO_NULL, 0,  MPI_COMM_SELF, comm, errcodes
    );

    MainFrame *main_frame = (MainFrame *) this->GetParent();
    main_frame->onAddAgent(
        THREE_OPT, three_opt_combo_box->GetValue(), comm,
        three_opt_ateam_param->interfaceToParams()
    );

/*
    MPI_Comm_spawn(
        "/home/naziozeno/Documents/projeto-final/AteamSCP/AteamMPI/ag_3opt",
        argvInitMD, 1, MPI_INFO_NULL, 0,  MPI_COMM_SELF, &comm_three_opt, errcodes
    );

    MainFrame *main_frame = (MainFrame *) this->GetParent();
    main_frame->onAddAgent(
        THREE_OPT, three_opt_combo_box->GetValue(), comm,
        three_opt_ateam_param->interfaceToParams()
    );
*/
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void InitMemories::onConfigureThreeOPTClick(wxCommandEvent &event)
{
    three_opt_ateam_param->Show();
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void InitMemories::onSubGClick(wxCommandEvent &event)
{
    MainFrame *main_frame = (MainFrame *) this->GetParent();
    main_frame->onAddAgent(SUBG, subg_combo_box->GetValue(), (MPI_Comm *) NULL, subg_ateam_param->interfaceToParams());
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void InitMemories::onConfigureSubGClick(wxCommandEvent &event)
{
    subg_ateam_param->Show();
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void InitMemories::onInitMPClick(wxCommandEvent &event)
{
    int argcInitMP = 4;
    char *argvInitMP[4];

    argvInitMP[0] = (char *) malloc (512 * sizeof(char));
    argvInitMP[1] = (char *) malloc (512 * sizeof(char));
    argvInitMP[2] = (char *) malloc (512 * sizeof(char));
    argvInitMP[3] = NULL;

    strcpy(argvInitMP[0], "1");

    strcpy(argvInitMP[1], "/home/naziozeno/Documents/projeto-final/AteamSCP/20_40_20");
    strcpy(argvInitMP[2], "/home/naziozeno/Documents/projeto-final/AteamSCP/teste/");

    MPI_Comm com2;
    int errcodes[1];

    MPI_Comm_spawn(
        "/home/naziozeno/Documents/projeto-final/AteamSCP/AteamMPI/initMP",
        argvInitMP, 1, MPI_INFO_NULL, 0,  MPI_COMM_SELF, &com2, errcodes
    );

    //timerMP->Start(100, false);

    subg_button->Enable();
    subg_combo_box->Enable();
    subg_configure_button->Enable();

    ls_button->Enable();
    ls_combo_box->Enable();
    ls_configure_button->Enable();

    pert_button->Enable();
    pert_combo_box->Enable();
    pert_configure_button->Enable();

    cons_button->Enable();
    cons_combo_box->Enable();
    cons_configure_button->Enable();

    init_mp_button->Disable();
    init_mp_combo_box->Disable();
    init_mp_configure_button->Disable();

    if (!start_team)
    {
        MainFrame *main_frame = (MainFrame *) this->GetParent();
        main_frame->onApplyClick();
        start_team = true;
    }
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void InitMemories::onConfigureInitMPClick(wxCommandEvent &event)
{
    init_mp_ateam_param->Show();
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void InitMemories::onLSClick(wxCommandEvent &event)
{
    MainFrame *main_frame = (MainFrame *) this->GetParent();
    main_frame->onAddAgent(LS, ls_combo_box->GetValue(), (MPI_Comm *) NULL, ls_ateam_param->interfaceToParams());
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void InitMemories::onConfigureLSClick(wxCommandEvent &event)
{
    ls_ateam_param->Show();
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void InitMemories::onPertClick(wxCommandEvent &event)
{
    MainFrame *main_frame = (MainFrame *) this->GetParent();
    main_frame->onAddAgent(PERT, pert_combo_box->GetValue(), (MPI_Comm *) NULL, pert_ateam_param->interfaceToParams());
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void InitMemories::onConfigurePertClick(wxCommandEvent &event)
{
    pert_ateam_param->Show();
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void InitMemories::onConsClick(wxCommandEvent &event)
{
    MainFrame *main_frame = (MainFrame *) this->GetParent();
    main_frame->onAddAgent(CONS, cons_combo_box->GetValue(), (MPI_Comm *) NULL, cons_ateam_param->interfaceToParams());
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void InitMemories::onConfigureConsClick(wxCommandEvent &event)
{
    cons_ateam_param->Show();
}

/* ------------------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------------------- */
