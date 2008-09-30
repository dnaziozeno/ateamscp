#include "InitMemories.h"
#include "AteamParam.h"
#include "../../../include/MainFrame.h"

wxTimer *timerMP, *timerMD;

InitMemories::InitMemories(wxWindow* parent, int id, const wxString& title, const wxPoint& pos, const wxSize& size, long style):
    wxFrame(parent, id, title, pos, size, wxICONIZE|wxCAPTION|wxMINIMIZE|wxCLOSE_BOX|wxMINIMIZE_BOX)
{
    init_memories_statusbar = CreateStatusBar(1, 0);
    apply_button = new wxButton(this, 120, _("Apply"));
    configure_button = new wxButton(this, 121, _("Configure"));
    cancel_button = new wxButton(this, 122, _("Cancel"));
    top_static_line = new wxStaticLine(this, wxID_ANY);
    init_md_button = new wxButton(this, 123, _("Init MD"));
    const wxString *init_md_combo_box_choices = NULL;
    init_md_combo_box = new wxComboBox(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0, init_md_combo_box_choices, wxCB_DROPDOWN);
    three_opt_button = new wxButton(this, 124, _("Add 3OPT"));
    const wxString *three_opt_combo_box_choices = NULL;
    three_opt_combo_box = new wxComboBox(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0, three_opt_combo_box_choices, wxCB_DROPDOWN);
    subg_button = new wxButton(this, 125, _("Add SUBG"));
    const wxString *subg_combo_box_choices = NULL;
    subg_combo_box = new wxComboBox(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0, subg_combo_box_choices, wxCB_DROPDOWN);
    mid_static_line = new wxStaticLine(this, wxID_ANY);
    init_mp_button = new wxButton(this, 126, _("Init MP"));
    const wxString *init_mp_combo_box_choices = NULL;
    init_mp_combo_box = new wxComboBox(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0, init_mp_combo_box_choices, wxCB_DROPDOWN);
    ls_button = new wxButton(this, 127, _("Add LS"));
    const wxString *ls_combo_box_choices = NULL;
    ls_combo_box = new wxComboBox(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0, ls_combo_box_choices, wxCB_DROPDOWN);
    pert_button = new wxButton(this, 128, _("Add PERT"));
    const wxString *pert_combo_box_choices = NULL;
    pert_combo_box = new wxComboBox(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0, pert_combo_box_choices, wxCB_DROPDOWN);
    cons_button = new wxButton(this, 129, _("Add CONS"));
    const wxString *cons_combo_box_choices = NULL;
    cons_combo_box = new wxComboBox(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0, cons_combo_box_choices, wxCB_DROPDOWN);

    set_properties();
    do_layout();

    timerMD = new wxTimer(this, 990);
    timerMP = new wxTimer(this, 991);

    three_opt_button->Disable();
    three_opt_combo_box->Disable();
    subg_button->Disable();
    subg_combo_box->Disable();

    ls_button->Disable();
    ls_combo_box->Disable();
    pert_button->Disable();
    pert_combo_box->Disable();
    cons_button->Disable();
    cons_combo_box->Disable();

    Connect(120, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(InitMemories::onApplyClick));
    Connect(121, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(InitMemories::onConfigureClick));
    Connect(122, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(InitMemories::onCancelClick));
    Connect(123, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(InitMemories::onInitMDClick));
    Connect(124, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(InitMemories::onThreeOPTClick));
    Connect(125, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(InitMemories::onSubGClick));
    Connect(126, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(InitMemories::onInitMPClick));
    Connect(127, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(InitMemories::onLSClick));
    Connect(128, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(InitMemories::onPertClick));
    Connect(129, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(InitMemories::onConsClick));

    Connect(990, wxEVT_TIMER, wxCommandEventHandler(InitMemories::onTimerMDEvent));
    Connect(991, wxEVT_TIMER, wxCommandEventHandler(InitMemories::onTimerMPEvent));
}

void InitMemories::onTimerMDEvent(wxCommandEvent &event)
{
    three_opt_button->Enable();
    three_opt_combo_box->Enable();

    init_md_button->Disable();
    init_md_combo_box->Disable();

    timerMD->Stop();
}

void InitMemories::onTimerMPEvent(wxCommandEvent &event)
{
    subg_button->Enable();
    subg_combo_box->Enable();

    ls_button->Enable();
    ls_combo_box->Enable();
    pert_button->Enable();
    pert_combo_box->Enable();
    cons_button->Enable();
    cons_combo_box->Enable();

    init_mp_button->Disable();
    init_mp_combo_box->Disable();

    timerMP->Stop();
}

void InitMemories::onApplyClick(wxCommandEvent &event)
{

}

void InitMemories::onConfigureClick(wxCommandEvent &event)
{
    AteamParam *ateam_param = new AteamParam(this, wxID_ANY, wxEmptyString, wxPoint(400, 100));
    ateam_param->Show();
}

void InitMemories::onCancelClick(wxCommandEvent &event)
{
}

void InitMemories::onInitMDClick(wxCommandEvent &event)
{
    /*
    wxMessageBox(
        wxString(_("COD 201:\nInitMD could not start. ")) +
        _("Check the IP: ") + init_md_combo_box->GetValue() +
        _("\n\nFor details:\nhttp://www.inf.ufg.br/~diocleciano/ateamscp"),
        _("InitMemories Error"), wxICON_ERROR|wxOK, this
    );
    */

    MainFrame *main_frame = (MainFrame *) this->GetParent();
    //main_frame->onMemoriesInitMDClick();

    timerMD->Start(100, false);

    //system("mpd &");
    //system("mpiexec -n 1 ../AteamApp/initMD 1 ../20_40_20 ../");
}

void InitMemories::onThreeOPTClick(wxCommandEvent &event)
{
}

void InitMemories::onSubGClick(wxCommandEvent &event)
{
}

void InitMemories::onInitMPClick(wxCommandEvent &event)
{
    timerMP->Start(100, false);
}

void InitMemories::onLSClick(wxCommandEvent &event)
{
}

void InitMemories::onPertClick(wxCommandEvent &event)
{
}

void InitMemories::onConsClick(wxCommandEvent &event)
{
}

void InitMemories::set_properties()
{
    SetTitle(_("InitMemories"));
    int init_memories_statusbar_widths[] = { -1 };
    init_memories_statusbar->SetStatusWidths(1, init_memories_statusbar_widths);
    const wxString init_memories_statusbar_fields[] = {
        _("")
    };
    for(int i = 0; i < init_memories_statusbar->GetFieldsCount(); ++i) {
        init_memories_statusbar->SetStatusText(init_memories_statusbar_fields[i], i);
    }
    apply_button->SetMinSize(wxSize(90, 27));
    configure_button->SetMinSize(wxSize(90, 27));
    cancel_button->SetMinSize(wxSize(90, 27));
    init_md_button->SetMinSize(wxSize(90, 27));
    init_md_combo_box->SetMinSize(wxSize(181, 27));
    three_opt_button->SetMinSize(wxSize(90, 27));
    three_opt_combo_box->SetMinSize(wxSize(181, 27));
    subg_button->SetMinSize(wxSize(90, 27));
    subg_combo_box->SetMinSize(wxSize(181, 27));
    init_mp_button->SetMinSize(wxSize(90, 27));
    init_mp_combo_box->SetMinSize(wxSize(181, 27));
    ls_button->SetMinSize(wxSize(90, 27));
    ls_combo_box->SetMinSize(wxSize(181, 27));
    pert_button->SetMinSize(wxSize(90, 27));
    pert_combo_box->SetMinSize(wxSize(181, 27));
    cons_button->SetMinSize(wxSize(90, 27));
    cons_combo_box->SetMinSize(wxSize(181, 27));
}


void InitMemories::do_layout()
{
    wxFlexGridSizer* main_grid_sizer = new wxFlexGridSizer(5, 1, 2, 2);
    wxFlexGridSizer* down_grid_sizer = new wxFlexGridSizer(4, 2, 5, 3);
    wxFlexGridSizer* mid_buttons_grid_sizer = new wxFlexGridSizer(3, 2, 5, 3);
    wxFlexGridSizer* grid_sizer_1 = new wxFlexGridSizer(1, 3, 2, 2);
    grid_sizer_1->Add(apply_button, 0, wxTOP, 4);
    grid_sizer_1->Add(configure_button, 0, wxTOP, 4);
    grid_sizer_1->Add(cancel_button, 0, wxTOP, 4);
    main_grid_sizer->Add(grid_sizer_1, 1, wxLEFT|wxEXPAND, 2);
    main_grid_sizer->Add(top_static_line, 0, wxALL|wxEXPAND, 2);
    mid_buttons_grid_sizer->Add(init_md_button, 0, 0, 0);
    mid_buttons_grid_sizer->Add(init_md_combo_box, 0, 0, 0);
    mid_buttons_grid_sizer->Add(three_opt_button, 0, 0, 0);
    mid_buttons_grid_sizer->Add(three_opt_combo_box, 0, 0, 0);
    mid_buttons_grid_sizer->Add(subg_button, 0, 0, 0);
    mid_buttons_grid_sizer->Add(subg_combo_box, 0, 0, 0);
    main_grid_sizer->Add(mid_buttons_grid_sizer, 1, wxALL|wxEXPAND, 2);
    main_grid_sizer->Add(mid_static_line, 0, wxALL|wxEXPAND, 2);
    down_grid_sizer->Add(init_mp_button, 0, 0, 0);
    down_grid_sizer->Add(init_mp_combo_box, 0, 0, 0);
    down_grid_sizer->Add(ls_button, 0, 0, 0);
    down_grid_sizer->Add(ls_combo_box, 0, 0, 0);
    down_grid_sizer->Add(pert_button, 0, 0, 0);
    down_grid_sizer->Add(pert_combo_box, 0, 0, 0);
    down_grid_sizer->Add(cons_button, 0, wxBOTTOM, 4);
    down_grid_sizer->Add(cons_combo_box, 0, wxBOTTOM, 4);
    main_grid_sizer->Add(down_grid_sizer, 1, wxALL|wxEXPAND, 2);
    SetSizer(main_grid_sizer);
    main_grid_sizer->Fit(this);
    Layout();
}
