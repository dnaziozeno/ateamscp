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
    *pert_ateam_param, *cons_ateam_param, *primal_ateam_param,
    *dual_ateam_param;

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

    primal_button = new wxButton(this, 135, _("Add PRIMAL"));
    const wxString primal_combo_box_choices[] = {_("127.0.0.1")};
    primal_combo_box = new wxComboBox(
        this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize,
        1, primal_combo_box_choices, wxCB_DROPDOWN
    );
    primal_configure_button = new wxButton(this, 136, _("Configure"));

    dual_button = new wxButton(this, 137, _("Add DUAL"));
    const wxString dual_combo_box_choices[] = {_("127.0.0.1")};
    dual_combo_box = new wxComboBox(
        this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize,
        1, dual_combo_box_choices, wxCB_DROPDOWN
    );
    dual_configure_button = new wxButton(this, 138, _("Configure"));

    set_properties();
    do_layout();

    three_opt_button->Disable();
    three_opt_combo_box->Disable();
    three_opt_configure_button->Disable();
    subg_button->Disable();
    subg_combo_box->Disable();
    subg_configure_button->Disable();

    init_mp_button->Disable();
    init_mp_combo_box->Disable();
    init_mp_configure_button->Disable();

    ls_button->Disable();
    ls_combo_box->Disable();
    ls_configure_button->Disable();
    pert_button->Disable();
    pert_combo_box->Disable();
    pert_configure_button->Disable();
    cons_button->Disable();
    cons_combo_box->Disable();
    cons_configure_button->Disable();
    primal_button->Disable();
    primal_combo_box->Disable();
    primal_configure_button->Disable();
    dual_button->Disable();
    dual_combo_box->Disable();
    dual_configure_button->Disable();

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
    Connect(135, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(InitMemories::onPrimalClick));
    Connect(136, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(InitMemories::onConfigurePrimalClick));
    Connect(137, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(InitMemories::onDualClick));
    Connect(138, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(InitMemories::onConfigureDualClick));

    init_md_ateam_param = new AteamParam(this, wxID_ANY, wxEmptyString, wxPoint(420, 200));
    three_opt_ateam_param = new AteamParam(this, wxID_ANY, wxEmptyString, wxPoint(420, 200));
    subg_ateam_param = new AteamParam(this, wxID_ANY, wxEmptyString, wxPoint(420, 200));
    init_mp_ateam_param = new AteamParam(this, wxID_ANY, wxEmptyString, wxPoint(420, 200));
    ls_ateam_param = new AteamParam(this, wxID_ANY, wxEmptyString, wxPoint(420, 200));
    pert_ateam_param = new AteamParam(this, wxID_ANY, wxEmptyString, wxPoint(420, 200));
    cons_ateam_param = new AteamParam(this, wxID_ANY, wxEmptyString, wxPoint(420, 200));
    primal_ateam_param = new AteamParam(this, wxID_ANY, wxEmptyString, wxPoint(420, 200));
    dual_ateam_param = new AteamParam(this, wxID_ANY, wxEmptyString, wxPoint(420, 200));
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

    primal_button->SetMinSize(wxSize(90, 27));
    primal_combo_box->SetMinSize(wxSize(181, 27));
    primal_combo_box->SetSelection(-1);
    primal_configure_button->SetMinSize(wxSize(90, 27));

    dual_button->SetMinSize(wxSize(90, 27));
    dual_combo_box->SetMinSize(wxSize(181, 27));
    dual_combo_box->SetSelection(-1);
    dual_configure_button->SetMinSize(wxSize(90, 27));
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void InitMemories::do_layout()
{
    wxFlexGridSizer* main_grid_sizer = new wxFlexGridSizer(5, 1, 2, 2);
    wxFlexGridSizer* down_grid_sizer = new wxFlexGridSizer(6, 3, 5, 3);
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
    down_grid_sizer->Add(primal_button, 0, wxBOTTOM, 4);
    down_grid_sizer->Add(primal_combo_box, 0, wxBOTTOM, 4);
    down_grid_sizer->Add(primal_configure_button, 0, 0, 0);
    down_grid_sizer->Add(dual_button, 0, wxBOTTOM, 4);
    down_grid_sizer->Add(dual_combo_box, 0, wxBOTTOM, 4);
    down_grid_sizer->Add(dual_configure_button, 0, 0, 0);

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

    char *argvInitMD[6];
    argvInitMD[0] = (char *) malloc (512 * sizeof(char));
    argvInitMD[1] = (char *) malloc (512 * sizeof(char));
    argvInitMD[2] = (char *) malloc (512 * sizeof(char));
    argvInitMD[3] = (char *) malloc (512 * sizeof(char));
    argvInitMD[4] = (char *) malloc (512 * sizeof(char));
    argvInitMD[5] = NULL;

    strcpy(argvInitMD[0], "/home/naziozeno/Documents/projeto-final/AteamSCP/20_40_20");
    strcpy(argvInitMD[1], "/home/naziozeno/Documents/projeto-final/AteamSCP/teste/");

    int *params = init_md_ateam_param->interfaceToParams();
    strcpy(argvInitMD[2], wxString::Format(wxT("%i"), params[2]).mb_str());
    strcpy(argvInitMD[3], wxString::Format(wxT("%i"), params[12]).mb_str());
    strcpy(argvInitMD[4], wxString::Format(wxT("%i"), params[16]).mb_str());

    MPI_Comm com2;
    int errcodes[1];

    MPI_Init(NULL, NULL);

    MPI_Comm_spawn(
        "/home/naziozeno/Documents/projeto-final/AteamSCP/AteamMPI/initMD",
        argvInitMD, 1, MPI_INFO_NULL, 0,  MPI_COMM_SELF, &com2, errcodes
    );

    init_mp_button->Enable();
    init_mp_combo_box->Enable();
    init_mp_configure_button->Enable();

    three_opt_button->Enable();
    three_opt_combo_box->Enable();
    three_opt_configure_button->Enable();

    dual_button->Enable();
    dual_combo_box->Enable();
    dual_configure_button->Enable();

    init_md_button->Disable();
    init_md_combo_box->Disable();
    init_md_configure_button->Disable();

    //MainFrame *main_frame = (MainFrame *) this->GetParent();
    main_frame->onApplyClick();
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
    char *argvThreeOpt[5];

    argvThreeOpt[0] = (char *) malloc (512 * sizeof(char));
    argvThreeOpt[1] = (char *) malloc (16 * sizeof(char));
    argvThreeOpt[2] = (char *) malloc (16 * sizeof(char));
    argvThreeOpt[3] = (char *) malloc (16 * sizeof(char));
    argvThreeOpt[4] = NULL;

    strcpy(argvThreeOpt[0], "/home/naziozeno/Documents/projeto-final/AteamSCP/20_40_20");

    int *params = three_opt_ateam_param->interfaceToParams();
    strcpy(argvThreeOpt[1], wxString::Format(wxT("%i"), params[1]).mb_str());
    strcpy(argvThreeOpt[2], wxString::Format(wxT("%i"), params[6]).mb_str());
    strcpy(argvThreeOpt[3], wxString::Format(wxT("%i"), params[11]).mb_str());

    int errcodes[1];
    MPI_Comm *comm = (MPI_Comm *) malloc(sizeof(MPI_Comm));

    MPI_Comm_spawn(
        "/home/naziozeno/Documents/projeto-final/AteamSCP/AteamMPI/ag_3opt",
        argvThreeOpt, 1, MPI_INFO_NULL, 0,  MPI_COMM_SELF, comm, errcodes
    );

    MainFrame *main_frame = (MainFrame *) this->GetParent();
    main_frame->onAddAgent(
        THREE_OPT, three_opt_combo_box->GetValue(), comm,
        three_opt_ateam_param->interfaceToParams()
    );
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
    char *argvSubg[4];

    argvSubg[0] = (char *) malloc (512 * sizeof(char));
    argvSubg[1] = (char *) malloc (16 * sizeof(char));
    argvSubg[2] = (char *) malloc (16 * sizeof(char));
    argvSubg[3] = NULL;

    strcpy(argvSubg[0], "/home/naziozeno/Documents/projeto-final/AteamSCP/20_40_20");

    int *params = three_opt_ateam_param->interfaceToParams();
    strcpy(argvSubg[1], wxString::Format(wxT("%i"), params[6]).mb_str());
    strcpy(argvSubg[2], wxString::Format(wxT("%i"), params[11]).mb_str());

    int errcodes[1];
    MPI_Comm *comm = (MPI_Comm *) malloc(sizeof(MPI_Comm));

    MPI_Comm_spawn(
        "/home/naziozeno/Documents/projeto-final/AteamSCP/AteamMPI/ag_subgr",
        argvSubg, 1, MPI_INFO_NULL, 0,  MPI_COMM_SELF, comm, errcodes
    );

    MainFrame *main_frame = (MainFrame *) this->GetParent();
    main_frame->onAddAgent(
        SUBG, subg_combo_box->GetValue(), comm,
        subg_ateam_param->interfaceToParams()
    );
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
    char *argvInitMP[6];

    argvInitMP[0] = (char *) malloc (512 * sizeof(char));
    argvInitMP[1] = (char *) malloc (512 * sizeof(char));
    argvInitMP[2] = (char *) malloc (512 * sizeof(char));
    argvInitMP[3] = (char *) malloc (512 * sizeof(char));
    argvInitMP[4] = (char *) malloc (512 * sizeof(char));
    argvInitMP[5] = NULL;

    strcpy(argvInitMP[0], "/home/naziozeno/Documents/projeto-final/AteamSCP/20_40_20");
    strcpy(argvInitMP[1], "/home/naziozeno/Documents/projeto-final/AteamSCP/teste/");

    int *params = init_mp_ateam_param->interfaceToParams();
    strcpy(argvInitMP[2], wxString::Format(wxT("%i"), params[3]).mb_str());
    strcpy(argvInitMP[3], wxString::Format(wxT("%i"), params[13]).mb_str());
    strcpy(argvInitMP[4], wxString::Format(wxT("%i"), params[18]).mb_str());

    MPI_Comm com2;
    int errcodes[1];

    MPI_Comm_spawn(
        "/home/naziozeno/Documents/projeto-final/AteamSCP/AteamMPI/initMP",
        argvInitMP, 1, MPI_INFO_NULL, 0,  MPI_COMM_SELF, &com2, errcodes
    );

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

    primal_button->Enable();
    primal_combo_box->Enable();
    primal_configure_button->Enable();

    init_mp_button->Disable();
    init_mp_combo_box->Disable();
    init_mp_configure_button->Disable();
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
    char *argvLs[6];

    argvLs[0] = (char *) malloc (512 * sizeof(char));
    argvLs[1] = (char *) malloc (16 * sizeof(char));
    argvLs[2] = (char *) malloc (16 * sizeof(char));
    argvLs[3] = (char *) malloc (16 * sizeof(char));
    argvLs[4] = (char *) malloc (16 * sizeof(char));
    argvLs[5] = NULL;

    strcpy(argvLs[0], "/home/naziozeno/Documents/projeto-final/AteamSCP/20_40_20");

    int *params = three_opt_ateam_param->interfaceToParams();
    strcpy(argvLs[1], wxString::Format(wxT("%i"), params[11]).mb_str());
    strcpy(argvLs[2], wxString::Format(wxT("%i"), params[19]).mb_str());

    int errcodes[1];
    MPI_Comm *comm = (MPI_Comm *) malloc(sizeof(MPI_Comm));

    MPI_Comm_spawn(
        "/home/naziozeno/Documents/projeto-final/AteamSCP/AteamMPI/ag_search",
        argvLs, 1, MPI_INFO_NULL, 0,  MPI_COMM_SELF, comm, errcodes
    );

    MainFrame *main_frame = (MainFrame *) this->GetParent();
    main_frame->onAddAgent(
        LS, ls_combo_box->GetValue(), comm,
        ls_ateam_param->interfaceToParams()
    );
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
    char *argvPert[6];

    argvPert[0] = (char *) malloc (512 * sizeof(char));
    argvPert[1] = (char *) malloc (16 * sizeof(char));
    argvPert[2] = (char *) malloc (16 * sizeof(char));
    argvPert[3] = (char *) malloc (16 * sizeof(char));
    argvPert[4] = (char *) malloc (16 * sizeof(char));
    argvPert[5] = NULL;

    strcpy(argvPert[0], "/home/naziozeno/Documents/projeto-final/AteamSCP/20_40_20");

    int *params = three_opt_ateam_param->interfaceToParams();
    strcpy(argvPert[1], wxString::Format(wxT("%i"), params[1]).mb_str());
    strcpy(argvPert[2], wxString::Format(wxT("%i"), params[3]).mb_str());
    strcpy(argvPert[3], wxString::Format(wxT("%i"), params[11]).mb_str());
    strcpy(argvPert[4], wxString::Format(wxT("%i"), params[8]).mb_str());

    int errcodes[1];
    MPI_Comm *comm = (MPI_Comm *) malloc(sizeof(MPI_Comm));

    MPI_Comm_spawn(
        "/home/naziozeno/Documents/projeto-final/AteamSCP/AteamMPI/ag_adjust",
        argvPert, 1, MPI_INFO_NULL, 0,  MPI_COMM_SELF, comm, errcodes
    );

    MainFrame *main_frame = (MainFrame *) this->GetParent();
    main_frame->onAddAgent(
        PERT, pert_combo_box->GetValue(), comm,
        pert_ateam_param->interfaceToParams()
    );
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
    char *argvCons[7];

    argvCons[0] = (char *) malloc (512 * sizeof(char));
    argvCons[1] = (char *) malloc (16 * sizeof(char));
    argvCons[2] = (char *) malloc (16 * sizeof(char));
    argvCons[3] = (char *) malloc (16 * sizeof(char));
    argvCons[4] = (char *) malloc (16 * sizeof(char));
    argvCons[5] = (char *) malloc (16 * sizeof(char));
    argvCons[6] = NULL;

    strcpy(argvCons[0], "/home/naziozeno/Documents/projeto-final/AteamSCP/20_40_20");

    int *params = three_opt_ateam_param->interfaceToParams();
    strcpy(argvCons[1], wxString::Format(wxT("%i"), params[1]).mb_str());
    strcpy(argvCons[2], wxString::Format(wxT("%i"), params[6]).mb_str());
    strcpy(argvCons[3], wxString::Format(wxT("%i"), params[11]).mb_str());
    strcpy(argvCons[4], wxString::Format(wxT("%i"), params[15]).mb_str());
    strcpy(argvCons[5], wxString::Format(wxT("%i"), params[18]).mb_str());

    int errcodes[1];
    MPI_Comm *comm = (MPI_Comm *) malloc(sizeof(MPI_Comm));

    MPI_Comm_spawn(
        "/home/naziozeno/Documents/projeto-final/AteamSCP/AteamMPI/ag_consen",
        argvCons, 1, MPI_INFO_NULL, 0,  MPI_COMM_SELF, comm, errcodes
    );

    MainFrame *main_frame = (MainFrame *) this->GetParent();
    main_frame->onAddAgent(
        CONS, cons_combo_box->GetValue(), comm,
        cons_ateam_param->interfaceToParams()
    );
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
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void InitMemories::onPrimalClick(wxCommandEvent &event)
{
    char *argvPrimal[7];

    argvPrimal[0] = (char *) malloc (512 * sizeof(char));
    argvPrimal[1] = (char *) malloc (16 * sizeof(char));
    argvPrimal[2] = (char *) malloc (16 * sizeof(char));
    argvPrimal[3] = (char *) malloc (16 * sizeof(char));
    argvPrimal[4] = (char *) malloc (16 * sizeof(char));
    argvPrimal[5] = (char *) malloc (16 * sizeof(char));
    argvPrimal[6] = NULL;

    strcpy(argvPrimal[0], "/home/naziozeno/Documents/projeto-final/AteamSCP/20_40_20");

    int *params = three_opt_ateam_param->interfaceToParams();
    strcpy(argvPrimal[1], wxString::Format(wxT("%i"), params[1]).mb_str());
    strcpy(argvPrimal[2], wxString::Format(wxT("%i"), params[3]).mb_str());
    strcpy(argvPrimal[3], wxString::Format(wxT("%i"), params[6]).mb_str());
    strcpy(argvPrimal[4], wxString::Format(wxT("%i"), params[11]).mb_str());
    strcpy(argvPrimal[5], wxString::Format(wxT("%i"), params[14]).mb_str());

    int errcodes[1];
    MPI_Comm *comm = (MPI_Comm *) malloc(sizeof(MPI_Comm));

    MPI_Comm_spawn(
        "/home/naziozeno/Documents/projeto-final/AteamSCP/AteamMPI/ag_primal",
        argvPrimal, 1, MPI_INFO_NULL, 0,  MPI_COMM_SELF, comm, errcodes
    );

    MainFrame *main_frame = (MainFrame *) this->GetParent();
    main_frame->onAddAgent(
        PRIMAL, cons_combo_box->GetValue(), comm,
        cons_ateam_param->interfaceToParams()
    );
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void InitMemories::onConfigurePrimalClick(wxCommandEvent &event)
{
    cons_ateam_param->Show();
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void InitMemories::onDualClick(wxCommandEvent &event)
{
    char *argvDual[8];

    argvDual[0] = (char *) malloc (512 * sizeof(char));
    argvDual[1] = (char *) malloc (16 * sizeof(char));
    argvDual[2] = (char *) malloc (16 * sizeof(char));
    argvDual[3] = (char *) malloc (16 * sizeof(char));
    argvDual[4] = (char *) malloc (16 * sizeof(char));
    argvDual[5] = (char *) malloc (16 * sizeof(char));
    argvDual[6] = (char *) malloc (16 * sizeof(char));
    argvDual[7] = NULL;

    strcpy(argvDual[0], "/home/naziozeno/Documents/projeto-final/AteamSCP/20_40_20");

    int *params = three_opt_ateam_param->interfaceToParams();
    strcpy(argvDual[1], wxString::Format(wxT("%i"), params[1]).mb_str());
    strcpy(argvDual[2], wxString::Format(wxT("%i"), params[2]).mb_str());
    strcpy(argvDual[3], wxString::Format(wxT("%i"), params[6]).mb_str());
    strcpy(argvDual[4], wxString::Format(wxT("%i"), params[11]).mb_str());
    strcpy(argvDual[5], wxString::Format(wxT("%i"), params[12]).mb_str());
    strcpy(argvDual[6], wxString::Format(wxT("%i"), params[17]).mb_str());

    int errcodes[1];
    MPI_Comm *comm = (MPI_Comm *) malloc(sizeof(MPI_Comm));

    MPI_Comm_spawn(
        "/home/naziozeno/Documents/projeto-final/AteamSCP/AteamMPI/ag_dual",
        argvDual, 1, MPI_INFO_NULL, 0,  MPI_COMM_SELF, comm, errcodes
    );

    MainFrame *main_frame = (MainFrame *) this->GetParent();
    main_frame->onAddAgent(
        DUAL, cons_combo_box->GetValue(), comm,
        cons_ateam_param->interfaceToParams()
    );
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void InitMemories::onConfigureDualClick(wxCommandEvent &event)
{
    cons_ateam_param->Show();
}


/* ------------------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------------------- */
