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

#include "IOParam.h"
#include "AteamParam.h"

/* ------------------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------------------- */

/*
BEGIN_EVENT_TABLE(AteamParam, wxFrame)
    EVT_BUTTON(111, AteamParam::on_open_click)
    EVT_BUTTON(112, AteamParam::on_save_click)
    EVT_BUTTON(113, AteamParam::on_reset_click)
    EVT_BUTTON(114, AteamParam::on_apply_click)
    EVT_BUTTON(115, AteamParam::on_hide_click)
END_EVENT_TABLE()
*/

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
AteamParam::AteamParam(
    wxWindow* parent, int id, const wxString& title, const wxPoint& pos, const wxSize& size, long style
):wxFrame(
    parent, id, title, pos, size, wxICONIZE|wxCAPTION|wxMINIMIZE|wxCLOSE_BOX|wxMINIMIZE_BOX
)
{
    ateam_param_statusbar = CreateStatusBar(1, 0);

    open_button = new wxButton(this, 111, _("Open"));
    save_button = new wxButton(this, 112, _("Save"));
    reset_button = new wxButton(this, 113, _("Reset"));
    apply_button = new wxButton(this, 114, _("Apply"));
    hide_button = new wxButton(this, 115, _("Hide"));

    /** NO WXHELPER HERE --> */

    Connect(111, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AteamParam::onOpen));
    Connect(112, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AteamParam::onSave));
    Connect(113, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AteamParam::onReset));
    Connect(114, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AteamParam::onApply));
    Connect(115, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AteamParam::onHide));

    /** <---------------- */

    button_static_line_01 = new wxStaticLine(this, wxID_ANY);

    dual_worst_checkbox = new wxCheckBox(this, wxID_ANY, _("DualWorst"));
    primal_worst_checkbox = new wxCheckBox(this, wxID_ANY, _("PrimalWorst"));
    random_init_mp_checkbox = new wxCheckBox(this, wxID_ANY, _("RandomInitMP"));

    check_static_line_01 = new wxStaticLine(
        this, wxID_ANY, wxDefaultPosition,
        wxDefaultSize, wxLI_VERTICAL
    );

    random_primal_checkbox = new wxCheckBox(this, wxID_ANY, _("RandomPrimal"));
    random_consensus_checkbox = new wxCheckBox(this, wxID_ANY, _("RandomConsensus"));
    random_init_md_checkbox = new wxCheckBox(this, wxID_ANY, _("RandomInitMD"));

    check_static_line_02 = new wxStaticLine(
        this, wxID_ANY, wxDefaultPosition,
        wxDefaultSize, wxLI_VERTICAL
    );

    random_dual_checkbox = new wxCheckBox(this, wxID_ANY, _("RandomDual"));
    balas_ng_cut_checkbox = new wxCheckBox(this, wxID_ANY, _("BalasNgCut"));
    equal_cost_sol_checkbox = new wxCheckBox(this, wxID_ANY, _("EqualCostSol"));

    check_static_line_03 = new wxStaticLine(this, wxID_ANY);

    time_sleeping_label = new wxStaticText(this, wxID_ANY, _("TimeSleeping:"));
    time_sleeping_spin_ctrl = new wxSpinCtrl(
        this, wxID_ANY, wxT("120"), wxDefaultPosition,
        wxDefaultSize, wxSP_ARROW_KEYS, 0, 1000
    );

    max_len_dual_mem_label = new wxStaticText(this, wxID_ANY, _("MaxLenDualMem:"));
    max_len_dual_mem_spin_ctrl = new wxSpinCtrl(
        this, wxID_ANY, wxT("300"), wxDefaultPosition,
        wxDefaultSize, wxSP_ARROW_KEYS, 0, 1000
    );

    max_len_primal_mem_label = new wxStaticText(this, wxID_ANY, _("MaxLenPrimalMem:"));
    max_len_primal_mem_spin_ctrl = new wxSpinCtrl(
        this, wxID_ANY, wxT("600"), wxDefaultPosition,
        wxDefaultSize, wxSP_ARROW_KEYS, 0, 1000
    );

    max_len_cut_mem_label = new wxStaticText(this, wxID_ANY, _("MaxLenCutMem:"));
    max_len_cut_mem_spin_ctrl = new wxSpinCtrl(
        this, wxID_ANY, wxT("50021"), wxDefaultPosition,
        wxDefaultSize, wxSP_ARROW_KEYS, 0, 100000
    );

    len_hash_tab_label = new wxStaticText(this, wxID_ANY, _("LenHashTab:"));
    len_hash_tab_spin_ctrl = new wxSpinCtrl(
        this, wxID_ANY, wxT("40009"), wxDefaultPosition,
        wxDefaultSize, wxSP_ARROW_KEYS, 0, 100000
    );

    cut_sof_sol_label = new wxStaticText(this, wxID_ANY, _("CutSofSol:"));
    cut_sof_sol_spin_ctrl = new wxSpinCtrl(
        this, wxID_ANY, wxT("500"), wxDefaultPosition,
        wxDefaultSize, wxSP_ARROW_KEYS, 0, 1000
    );

    max_cut_gen_label = new wxStaticText(this, wxID_ANY, _("MaxCutGen:"));
    max_cut_gen_spin_ctrl = new wxSpinCtrl(
        this, wxID_ANY, wxT("200"), wxDefaultPosition,
        wxDefaultSize, wxSP_ARROW_KEYS, 0, 1000
    );

    spin_static_line_01 = new wxStaticLine(
        this, wxID_ANY, wxDefaultPosition,
        wxDefaultSize, wxLI_VERTICAL
    );

    min_change_label = new wxStaticText(this, wxID_ANY, _("MinChange:"));
    min_change_spin_ctrl = new wxSpinCtrl(
        this, wxID_ANY, wxT("4"), wxDefaultPosition,
        wxDefaultSize, wxSP_ARROW_KEYS, 0, 100
    );

    max_exe_time_label = new wxStaticText(this, wxID_ANY, _("MaxExeTime:"));
    max_exe_time_spin_ctrl = new wxSpinCtrl(
        this, wxID_ANY, wxT("108000"), wxDefaultPosition,
        wxDefaultSize, wxSP_ARROW_KEYS, 0, 1000000
    );

    reduc_perc_label = new wxStaticText(this, wxID_ANY, _("ReducPerc:"));
    reduc_perc_spin_ctrl = new wxSpinCtrl(
        this, wxID_ANY, wxT("30"), wxDefaultPosition,
        wxDefaultSize, wxSP_ARROW_KEYS, 0, 100
    );

    restr_value_label = new wxStaticText(this, wxID_ANY, _("RestrValue:"));
    restr_value_spin_ctrl = new wxSpinCtrl(
        this, wxID_ANY, wxT("90"), wxDefaultPosition,
        wxDefaultSize, wxSP_ARROW_KEYS, 0, 100
    );

    tabu_iterations_label = new wxStaticText(this, wxID_ANY, _("TabuIterations:"));
    tabu_iterations_spin_ctrl = new wxSpinCtrl(
        this, wxID_ANY, wxT("2"), wxDefaultPosition,
        wxDefaultSize, wxSP_ARROW_KEYS, 0, 100
    );

    max_sol_dual_ag_label = new wxStaticText(this, wxID_ANY, _("MaxSolDualAg:"));
    max_sol_dual_ag_spin_ctrl = new wxSpinCtrl(
        this, wxID_ANY, wxT("50"), wxDefaultPosition,
        wxDefaultSize, wxSP_ARROW_KEYS, 0, 100
    );

    max_sol_primal_ag_label = new wxStaticText(this, wxID_ANY, _("MaxSolPrimalAg:"));
    max_sol_primal_ag_spin_ctrl = new wxSpinCtrl(
        this, wxID_ANY, wxT("100"), wxDefaultPosition,
        wxDefaultSize, wxSP_ARROW_KEYS, 0, 100
    );

    set_properties();
    do_layout();
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void AteamParam::set_properties()
{
    SetTitle(_("A-Team Param"));

    int ateam_param_statusbar_widths[] = { -1 };
    ateam_param_statusbar->SetStatusWidths(1, ateam_param_statusbar_widths);

    const wxString ateam_param_statusbar_fields[] = { _("") };

    for(int i = 0; i < ateam_param_statusbar->GetFieldsCount(); ++i) {
        ateam_param_statusbar->SetStatusText(ateam_param_statusbar_fields[i], i);
    }

    open_button->SetMinSize(wxSize(88, 27));
    save_button->SetMinSize(wxSize(88, 27));
    reset_button->SetMinSize(wxSize(88, 27));
    apply_button->SetMinSize(wxSize(88, 27));
    hide_button->SetMinSize(wxSize(88, 27));

    dual_worst_checkbox->SetMinSize(wxSize(125, 22));
    dual_worst_checkbox->SetToolTip(_("(T/F) aceita (ou nao) sol. pior que a pior da mem."));
    dual_worst_checkbox->SetValue(1);

    primal_worst_checkbox->SetMinSize(wxSize(125, 22));
    primal_worst_checkbox->SetToolTip(_("(T/F) aceita (ou nao) sol. pior que a pior da mem."));

    random_init_mp_checkbox->SetMinSize(wxSize(125, 22));
    random_init_mp_checkbox->SetToolTip(_("(T/F) greedy randomica inicia a mem. sol. primais"));
    random_init_mp_checkbox->SetValue(1);

    random_primal_checkbox->SetMinSize(wxSize(148, 22));
    random_primal_checkbox->SetToolTip(_("(T/F) greedy randomica usada no ag. primal greedy"));

    random_consensus_checkbox->SetMinSize(wxSize(148, 22));
    random_consensus_checkbox->SetToolTip(_("(T/F) greedy randomica usada no ag. consensus."));
    random_consensus_checkbox->SetValue(1);

    random_init_md_checkbox->SetMinSize(wxSize(116, 22));
    random_init_md_checkbox->SetToolTip(_("(T/F) greedy randomica inicia a mem. sol. duais"));
    random_init_md_checkbox->SetValue(1);

    random_dual_checkbox->SetMinSize(wxSize(106, 22));
    random_dual_checkbox->SetToolTip(_("(T/F) greedy randomica usada no ag. dual greedy"));
    random_dual_checkbox->SetValue(1);

    balas_ng_cut_checkbox->SetMinSize(wxSize(110, 22));
    balas_ng_cut_checkbox->SetToolTip(_("(T/F) Se T gera cortes do tipo BN senao BR"));

    equal_cost_sol_checkbox->SetMinSize(wxSize(109, 22));
    equal_cost_sol_checkbox->SetToolTip(_("(T/F) Se T o serverMP aceita sol. de mesmo custo."));
    equal_cost_sol_checkbox->SetValue(1);

    time_sleeping_label->SetMinSize(wxSize(130, 18));
    time_sleeping_label->SetToolTip(_("Tempo em que um agente tera a execucao suspensa"));
    time_sleeping_spin_ctrl->SetMinSize(wxSize(80, 23));
    time_sleeping_spin_ctrl->SetToolTip(_("Tempo em que um agente tera a execucao suspensa"));

    max_len_dual_mem_label->SetMinSize(wxSize(130, 18));
    max_len_dual_mem_label->SetToolTip(_("Tamanho maximo da memoria de solucoes duais"));
    max_len_dual_mem_spin_ctrl->SetMinSize(wxSize(80, 23));
    max_len_dual_mem_spin_ctrl->SetToolTip(_("Tamanho maximo da memoria de solucoes duais"));

    max_len_primal_mem_label->SetMinSize(wxSize(130, 18));
    max_len_primal_mem_label->SetToolTip(_("Tamanho maximo da memoria de solucoes primais"));
    max_len_primal_mem_spin_ctrl->SetMinSize(wxSize(80, 23));
    max_len_primal_mem_spin_ctrl->SetToolTip(_("Tamanho maximo da memoria de solucoes primais"));

    max_len_cut_mem_label->SetMinSize(wxSize(130, 18));
    max_len_cut_mem_label->SetToolTip(_("503 1009 5003 10007 20011 30011 40009 50021 60013"));
    max_len_cut_mem_spin_ctrl->SetMinSize(wxSize(80, 23));
    max_len_cut_mem_spin_ctrl->SetToolTip(_("503 1009 5003 10007 20011 30011 40009 50021 60013"));

    len_hash_tab_label->SetMinSize(wxSize(130, 18));
    len_hash_tab_label->SetToolTip(_("401  809 4001  8009 16001 24001 32003 40009 48017"));
    len_hash_tab_spin_ctrl->SetMinSize(wxSize(80, 23));
    len_hash_tab_spin_ctrl->SetToolTip(_("401  809 4001  8009 16001 24001 32003 40009 48017"));

    cut_sof_sol_label->SetMinSize(wxSize(130, 18));
    cut_sof_sol_label->SetToolTip(_("Numero maximo de cortes adicionados ao problema"));
    cut_sof_sol_spin_ctrl->SetMinSize(wxSize(80, 23));
    cut_sof_sol_spin_ctrl->SetToolTip(_("Numero maximo de cortes adicionados ao problema"));

    max_cut_gen_label->SetMinSize(wxSize(130, 18));
    max_cut_gen_label->SetToolTip(_("Numero maximo de cortes gerados a partir de uma solucao"));
    max_cut_gen_spin_ctrl->SetMinSize(wxSize(80, 23));
    max_cut_gen_spin_ctrl->SetToolTip(_("Numero maximo de cortes gerados a partir de uma solucao"));

    min_change_label->SetMinSize(wxSize(130, 18));
    min_change_label->SetToolTip(_("Numero minimo (%) de trocas de 0->1 na heu rand adjust"));
    min_change_spin_ctrl->SetMinSize(wxSize(80, 23));
    min_change_spin_ctrl->SetToolTip(_("Numero minimo (%) de trocas de 0->1 na heu rand adjust"));

    max_exe_time_label->SetMinSize(wxSize(130, 18));
    max_exe_time_label->SetToolTip(_("Tempo maximo que um agente sera executado (segs)"));
    max_exe_time_spin_ctrl->SetMinSize(wxSize(80, 23));
    max_exe_time_spin_ctrl->SetToolTip(_("Tempo maximo que um agente sera executado (segs)"));

    reduc_perc_label->SetMinSize(wxSize(130, 18));
    reduc_perc_label->SetToolTip(_("Percentual a somar em uma var. dual (heur. greedy)"));
    reduc_perc_spin_ctrl->SetMinSize(wxSize(80, 23));
    reduc_perc_spin_ctrl->SetToolTip(_("Percentual a somar em uma var. dual (heur. greedy)"));

    restr_value_label->SetMinSize(wxSize(130, 18));
    restr_value_label->SetToolTip(_("% do conj. de variaveis na greedy de Feo & Resende"));
    restr_value_spin_ctrl->SetMinSize(wxSize(80, 23));
    restr_value_spin_ctrl->SetToolTip(_("% do conj. de variaveis na greedy de Feo & Resende"));

    tabu_iterations_label->SetMinSize(wxSize(130, 18));
    tabu_iterations_label->SetToolTip(_("Tamanho do tabu na her. busca local (em % nr. col)"));
    tabu_iterations_spin_ctrl->SetMinSize(wxSize(80, 23));
    tabu_iterations_spin_ctrl->SetToolTip(_("Tamanho do tabu na her. busca local (em % nr. col)"));

    max_sol_dual_ag_label->SetMinSize(wxSize(130, 18));
    max_sol_dual_ag_label->SetToolTip(_("Max. sol. geradas por um agente dual (% da mem.)"));
    max_sol_dual_ag_spin_ctrl->SetMinSize(wxSize(80, 23));
    max_sol_dual_ag_spin_ctrl->SetToolTip(_("Max. sol. geradas por um agente dual (% da mem.)"));

    max_sol_primal_ag_label->SetMinSize(wxSize(130, 18));
    max_sol_primal_ag_label->SetToolTip(_("Max. sol. geradas por um agente primal (% da mem.)"));
    max_sol_primal_ag_spin_ctrl->SetMinSize(wxSize(80, 23));
    max_sol_primal_ag_spin_ctrl->SetToolTip(_("Max. sol. geradas por um agente primal (% da mem.)"));

    AteamParam::defaultParams();
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void AteamParam::do_layout()
{
    wxFlexGridSizer* main_grid_sizer = new wxFlexGridSizer(5, 1, 0, 0);
    wxFlexGridSizer* spin_grid_sizer_main = new wxFlexGridSizer(2, 3, 0, 8);
    wxFlexGridSizer* spin_grid_sizer_02 = new wxFlexGridSizer(7, 1, 0, 0);
    wxFlexGridSizer* max_sol_primal_ag_grid_sizer = new wxFlexGridSizer(1, 2, 1, 1);
    wxFlexGridSizer* max_sol_dual_ag_grid_sizer = new wxFlexGridSizer(1, 2, 1, 1);
    wxFlexGridSizer* tabu_iterations_grid_sizer = new wxFlexGridSizer(1, 2, 1, 1);
    wxFlexGridSizer* restr_value_grid_sizer = new wxFlexGridSizer(1, 2, 1, 1);
    wxFlexGridSizer* reduc_perc_grid_sizer = new wxFlexGridSizer(1, 2, 1, 1);
    wxFlexGridSizer* max_exe_time_grid_sizer = new wxFlexGridSizer(1, 2, 1, 1);
    wxFlexGridSizer* min_change_grid_sizer = new wxFlexGridSizer(1, 2, 1, 1);
    wxFlexGridSizer* spin_grid_sizer_01 = new wxFlexGridSizer(7, 1, 0, 0);
    wxFlexGridSizer* max_cut_gen_grid_sizer = new wxFlexGridSizer(1, 2, 1, 1);
    wxFlexGridSizer* cut_sof_sol_grid_sizer = new wxFlexGridSizer(1, 2, 1, 1);
    wxFlexGridSizer* len_hash_tab_grid_sizer = new wxFlexGridSizer(1, 2, 1, 1);
    wxFlexGridSizer* max_len_cut_mem_grid_sizer = new wxFlexGridSizer(1, 2, 1, 1);
    wxFlexGridSizer* max_len_primal_mem_grid_sizer = new wxFlexGridSizer(1, 2, 1, 1);
    wxFlexGridSizer* max_len_dual_mem_grid_sizer = new wxFlexGridSizer(1, 2, 1, 1);
    wxFlexGridSizer* time_sleeping_grid_sizer = new wxFlexGridSizer(1, 2, 1, 1);
    wxFlexGridSizer* check_grid_sizer_main = new wxFlexGridSizer(1, 5, 2, 2);
    wxFlexGridSizer* check_grid_sizer_03 = new wxFlexGridSizer(3, 1, 0, 0);
    wxFlexGridSizer* check_grid_sizer_02 = new wxFlexGridSizer(3, 1, 0, 0);
    wxFlexGridSizer* check_grid_sizer_01 = new wxFlexGridSizer(3, 1, 0, 0);
    wxFlexGridSizer* button_grid_sizer = new wxFlexGridSizer(1, 5, 4, 4);

    button_grid_sizer->Add(open_button, 0, 0, 0);
    button_grid_sizer->Add(save_button, 0, 0, 0);
    button_grid_sizer->Add(reset_button, 0, 0, 0);
    button_grid_sizer->Add(apply_button, 0, 0, 0);
    button_grid_sizer->Add(hide_button, 0, 0, 0);

    main_grid_sizer->Add(button_grid_sizer, 1, wxALL|wxEXPAND, 4);
    main_grid_sizer->Add(button_static_line_01, 0, wxLEFT|wxRIGHT|wxBOTTOM|wxEXPAND, 4);
    check_grid_sizer_01->Add(dual_worst_checkbox, 0, 0, 0);
    check_grid_sizer_01->Add(primal_worst_checkbox, 0, 0, 0);
    check_grid_sizer_01->Add(random_init_mp_checkbox, 0, 0, 0);
    check_grid_sizer_main->Add(check_grid_sizer_01, 1, wxLEFT|wxRIGHT|wxTOP|wxEXPAND, 3);
    check_grid_sizer_main->Add(check_static_line_01, 0, wxALL|wxEXPAND, 4);
    check_grid_sizer_02->Add(random_primal_checkbox, 0, wxLEFT|wxRIGHT, 10);
    check_grid_sizer_02->Add(random_consensus_checkbox, 0, wxLEFT|wxRIGHT, 10);
    check_grid_sizer_02->Add(random_init_md_checkbox, 0, wxLEFT|wxRIGHT, 10);
    check_grid_sizer_main->Add(check_grid_sizer_02, 1, wxLEFT|wxRIGHT|wxTOP|wxEXPAND, 3);
    check_grid_sizer_main->Add(check_static_line_02, 0, wxALL|wxEXPAND, 4);
    check_grid_sizer_03->Add(random_dual_checkbox, 0, wxLEFT, 11);
    check_grid_sizer_03->Add(balas_ng_cut_checkbox, 0, wxLEFT, 11);
    check_grid_sizer_03->Add(equal_cost_sol_checkbox, 0, wxLEFT, 11);
    check_grid_sizer_main->Add(check_grid_sizer_03, 1, wxLEFT|wxRIGHT|wxTOP|wxEXPAND, 3);
    main_grid_sizer->Add(check_grid_sizer_main, 1, wxEXPAND, 0);
    main_grid_sizer->Add(check_static_line_03, 0, wxALL|wxEXPAND, 4);
    time_sleeping_grid_sizer->Add(time_sleeping_label, 0, wxALIGN_CENTER_VERTICAL, 0);
    time_sleeping_grid_sizer->Add(time_sleeping_spin_ctrl, 0, 0, 0);
    spin_grid_sizer_01->Add(time_sleeping_grid_sizer, 1, wxEXPAND, 0);
    max_len_dual_mem_grid_sizer->Add(max_len_dual_mem_label, 0, wxALIGN_CENTER_VERTICAL, 0);
    max_len_dual_mem_grid_sizer->Add(max_len_dual_mem_spin_ctrl, 0, 0, 0);
    spin_grid_sizer_01->Add(max_len_dual_mem_grid_sizer, 1, wxEXPAND, 0);
    max_len_primal_mem_grid_sizer->Add(max_len_primal_mem_label, 0, wxALIGN_CENTER_VERTICAL, 0);
    max_len_primal_mem_grid_sizer->Add(max_len_primal_mem_spin_ctrl, 0, 0, 0);
    spin_grid_sizer_01->Add(max_len_primal_mem_grid_sizer, 1, wxEXPAND, 0);
    max_len_cut_mem_grid_sizer->Add(max_len_cut_mem_label, 0, wxALIGN_CENTER_VERTICAL, 0);
    max_len_cut_mem_grid_sizer->Add(max_len_cut_mem_spin_ctrl, 0, 0, 0);
    spin_grid_sizer_01->Add(max_len_cut_mem_grid_sizer, 1, wxEXPAND, 0);
    len_hash_tab_grid_sizer->Add(len_hash_tab_label, 0, wxALIGN_CENTER_VERTICAL, 0);
    len_hash_tab_grid_sizer->Add(len_hash_tab_spin_ctrl, 0, 0, 0);
    spin_grid_sizer_01->Add(len_hash_tab_grid_sizer, 1, wxEXPAND, 0);
    cut_sof_sol_grid_sizer->Add(cut_sof_sol_label, 0, wxALIGN_CENTER_VERTICAL, 0);
    cut_sof_sol_grid_sizer->Add(cut_sof_sol_spin_ctrl, 0, 0, 0);
    spin_grid_sizer_01->Add(cut_sof_sol_grid_sizer, 1, wxEXPAND, 0);
    max_cut_gen_grid_sizer->Add(max_cut_gen_label, 0, wxALIGN_CENTER_VERTICAL, 0);
    max_cut_gen_grid_sizer->Add(max_cut_gen_spin_ctrl, 0, 0, 0);
    spin_grid_sizer_01->Add(max_cut_gen_grid_sizer, 1, wxEXPAND, 0);
    spin_grid_sizer_main->Add(spin_grid_sizer_01, 1, wxLEFT|wxRIGHT|wxBOTTOM|wxEXPAND, 5);
    spin_grid_sizer_main->Add(spin_static_line_01, 0, wxEXPAND, 0);
    min_change_grid_sizer->Add(min_change_label, 0, wxALIGN_CENTER_VERTICAL, 0);
    min_change_grid_sizer->Add(min_change_spin_ctrl, 0, 0, 0);
    spin_grid_sizer_02->Add(min_change_grid_sizer, 1, wxEXPAND, 0);
    max_exe_time_grid_sizer->Add(max_exe_time_label, 0, wxALIGN_CENTER_VERTICAL, 0);
    max_exe_time_grid_sizer->Add(max_exe_time_spin_ctrl, 0, 0, 0);
    spin_grid_sizer_02->Add(max_exe_time_grid_sizer, 1, wxEXPAND, 0);
    reduc_perc_grid_sizer->Add(reduc_perc_label, 0, wxALIGN_CENTER_VERTICAL, 0);
    reduc_perc_grid_sizer->Add(reduc_perc_spin_ctrl, 0, 0, 0);
    spin_grid_sizer_02->Add(reduc_perc_grid_sizer, 1, wxEXPAND, 0);
    restr_value_grid_sizer->Add(restr_value_label, 0, wxALIGN_CENTER_VERTICAL, 0);
    restr_value_grid_sizer->Add(restr_value_spin_ctrl, 0, 0, 0);
    spin_grid_sizer_02->Add(restr_value_grid_sizer, 1, wxEXPAND, 0);
    tabu_iterations_grid_sizer->Add(tabu_iterations_label, 0, wxALIGN_CENTER_VERTICAL, 0);
    tabu_iterations_grid_sizer->Add(tabu_iterations_spin_ctrl, 0, 0, 0);
    spin_grid_sizer_02->Add(tabu_iterations_grid_sizer, 1, wxEXPAND, 0);
    max_sol_dual_ag_grid_sizer->Add(max_sol_dual_ag_label, 0, wxALIGN_CENTER_VERTICAL, 0);
    max_sol_dual_ag_grid_sizer->Add(max_sol_dual_ag_spin_ctrl, 0, 0, 0);
    spin_grid_sizer_02->Add(max_sol_dual_ag_grid_sizer, 1, wxEXPAND, 0);
    max_sol_primal_ag_grid_sizer->Add(max_sol_primal_ag_label, 0, wxALIGN_CENTER_VERTICAL, 0);
    max_sol_primal_ag_grid_sizer->Add(max_sol_primal_ag_spin_ctrl, 0, 0, 0);
    spin_grid_sizer_02->Add(max_sol_primal_ag_grid_sizer, 1, wxEXPAND, 0);
    spin_grid_sizer_main->Add(spin_grid_sizer_02, 1, wxLEFT|wxRIGHT|wxBOTTOM|wxEXPAND, 5);
    main_grid_sizer->Add(spin_grid_sizer_main, 1, wxALL|wxEXPAND, 2);

    SetSizer(main_grid_sizer);
    main_grid_sizer->Fit(this);
    Layout();
}

/* ------------------------------------------------------------------------------------- */
/* Abre um arquivo de parametros de configuracao e apresenta os valores na interface. O  */
/* local e o nome do arquivo sao obtidos de um widget padrao do sistema, que na          */
/* biblioteca wxWidgets e representado pelo componente wxFileSelector. Apenas os         */
/* arquivos com a extensao .atp poderao ser selecionados.                                */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void AteamParam::onOpen(wxCommandEvent &event)
{
    SetStatusText(_(""), 0);

    wxString fname = wxFileSelector(
        _("Open Ateam-Params File"), (const wxChar *) NULL, (const wxChar *) NULL,
        _("atp"), _("Ateams-Params Files (*.atp)|*.atp"), wxOPEN, this
    );

    if (!fname.empty())
    {
        int *params = IOParam::readParams(fname.mb_str(wxConvUTF8));
        if (params == NULL || params[0] == IOPARAMS_FAILURE)
        {
        /** ***************************** MENSAGENS DE ERRO *************************************/
            wxMessageBox(
                _("COD 001:\nFailed to open file \"") + fname.AfterLast('/') + _("\"\n\n") +
                _("For details:\nhttp://www.inf.ufg.br/~diocleciano/ateamscp"),
                _("A-Team Param Error"), wxICON_ERROR|wxOK, this
            );
        }
        else
        {
            paramsToInterface(params);
            SetStatusText(_("File \"") + fname.AfterLast('/') + _("\" loaded with success."), 0);
        }

        free(params);
    }
}

/* ------------------------------------------------------------------------------------- */
/* Salva os valores dos parametros representados na interface em uma arquivo. O local e  */
/* o nome do arquivo sao obtidos de um widget padrao do sistema, que na biblioteca       */
/* wxWidgets e representado pelo componente wxFileSelector. A extensao do arquivo foi    */
/* definida como .atp.                                                                   */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void AteamParam::onSave(wxCommandEvent &event)
{
    SetStatusText(_(""), 0);

    wxString fname = wxFileSelector(
        _("Save Ateam_params File"), (const wxChar *) NULL, (const wxChar *) NULL,
        _("atp"), _("Ateams-Params Files (*.atp)|*.atp"), wxSAVE, this
    );

    if (!fname.empty())
    {
        int *params = interfaceToParams();

        if (fname.AfterLast('.') != _("atp")) fname.Append(_(".atp"));
        if (wxFile::Exists(fname))
        {
            if (wxMessageBox(
                _("The file ") + fname + _("\nalready exists. Do you want to overwrite it?"),
                _("A-Team Param Error"), wxICON_QUESTION|wxYES_NO, this
            ) == wxNO) return;
        }

        if (IOParam::writeParams(fname.mb_str(wxConvUTF8), params) == IOPARAMS_FAILURE)
        {
        /** ***************************** MENSAGENS DE ERRO *************************************/
            wxMessageBox(
                _("COD 001:\nFailed to save file \"") + fname.AfterLast('/') +
                _("\"\n\n") + _("For details:\nhttp://www.inf.ufg.br/~diocleciano/ateamscp"),
                _("A-Team Param Error"), wxICON_ERROR|wxOK, this
            );
        }
        else
        {
            SetStatusText(_("File \"") + fname.AfterLast('/') + _("\" saved with success."), 0);
        }

        if (params != NULL) free(params);
    }
}

/* ------------------------------------------------------------------------------------- */
/* Atribui os valores do arquivo de parametros default (etc/ateam_param.model) para a    */
/* interface grafica.                                                                    */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void AteamParam::onReset(wxCommandEvent &event)
{
    AteamParam::defaultParams();
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void AteamParam::onApply(wxCommandEvent &event)
{
    SetStatusText(_("BUTTON APPLY CLICKED..."), 0);
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void AteamParam::onHide(wxCommandEvent &event)
{
    this->Hide();
}

/* ------------------------------------------------------------------------------------- */
/* Atribui aos campos da interface grafica os valores do arquivo de configuracao default */
/* do A-Team. Esse arquivo deve ser encontrado em etc/ateam_param.model e tem tambem a   */
/* responsabilidade de definir o padrao do arquivo de parametros. As alteracoes          */
/* realizadas nos comentarios refletirao nos proximos arquivos salvos, entretanto o      */
/* numero de oito linhas de comentarios iniciais deve ser mantido.                       */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void AteamParam::defaultParams()
{
    char *model_path = IOParam::getExecutablePath();
    strncat(model_path, "/etc/ateam_param.model", 22);
    int *params = IOParam::readParams(model_path);

    if (params == NULL || params[0] == IOPARAMS_FAILURE)
    {
        /** ***************************** MENSAGENS DE ERRO *************************************/
        wxMessageBox(wxString(_("COD 001:\nFailed to loaded default parameters\n\n")) + 
        _("For details:\nhttp://www.inf.ufg.br/~diocleciano/ateamscp"),
        _("A-Team Param Error"), wxICON_ERROR|wxOK, this);
    }
    else
    {
        paramsToInterface(params);
        SetStatusText(_("Default parameters loaded with success."), 0);
    }

    /* Libera a area da memoria ocupada pelo vetor params. */
    if (params != NULL) free(params);
}

/* ------------------------------------------------------------------------------------- */
/* Atribui aos campos da interface grafica os valores de um vetor de parametros do       */
/* A-Team caso a posicao 0 desse vetor contenha IOPARAMS_SUCCESS.                        */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* int *params - Ponteiro para um vetor de parametros do A-Team.                         */
/* ------------------------------------------------------------------------------------- */
void AteamParam::paramsToInterface(int *params)
{
    if (params[0] == IOPARAMS_SUCCESS)
    {
        time_sleeping_spin_ctrl->SetValue(params[1]);
        max_len_dual_mem_spin_ctrl->SetValue(params[2]);
        max_len_primal_mem_spin_ctrl->SetValue(params[3]);
        max_len_cut_mem_spin_ctrl->SetValue(params[4]);
        len_hash_tab_spin_ctrl->SetValue(params[5]);
        cut_sof_sol_spin_ctrl->SetValue(params[6]);
        max_cut_gen_spin_ctrl->SetValue(params[7]);
        min_change_spin_ctrl->SetValue(params[8]);
        dual_worst_checkbox->SetValue(params[9]);
        primal_worst_checkbox->SetValue(params[10]);
        max_exe_time_spin_ctrl->SetValue(params[11]);
        reduc_perc_spin_ctrl->SetValue(params[12]);
        random_init_mp_checkbox->SetValue(params[13]);
        random_primal_checkbox->SetValue(params[14]);
        random_consensus_checkbox->SetValue(params[15]);
        random_init_md_checkbox->SetValue(params[16]);
        random_dual_checkbox->SetValue(params[17]);
        restr_value_spin_ctrl->SetValue(params[18]);
        tabu_iterations_spin_ctrl->SetValue(params[19]);
        balas_ng_cut_checkbox->SetValue(params[20]);
        max_sol_dual_ag_spin_ctrl->SetValue(params[21]);
        max_sol_primal_ag_spin_ctrl->SetValue(params[22]);
        equal_cost_sol_checkbox->SetValue(params[23]);
    }
}

/* ------------------------------------------------------------------------------------- */
/* Retorna um vetor de parametros de configuracao do A-Team, com os valores presentes na */
/* interface grafica. Para essa operacao a posicao 0 do vetor recebera IOPARAMS_SUCCESS. */
/* O valor NULL pode ser retornado em caso de erro de alocacao de memoria.               */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
int *AteamParam::interfaceToParams()
{
    int *params = (int *) malloc(24 * sizeof(int));

    if (params != NULL)
    {
        params[0] = IOPARAMS_SUCCESS;

        params[1] = time_sleeping_spin_ctrl->GetValue();
        params[2] = max_len_dual_mem_spin_ctrl->GetValue();
        params[3] = max_len_primal_mem_spin_ctrl->GetValue();
        params[4] = max_len_cut_mem_spin_ctrl->GetValue();
        params[5] = len_hash_tab_spin_ctrl->GetValue();
        params[6] = cut_sof_sol_spin_ctrl->GetValue();
        params[7] = max_cut_gen_spin_ctrl->GetValue();
        params[8] = min_change_spin_ctrl->GetValue();
        params[9] = dual_worst_checkbox->GetValue();
        params[10] = primal_worst_checkbox->GetValue();
        params[11] = max_exe_time_spin_ctrl->GetValue();
        params[12] = reduc_perc_spin_ctrl->GetValue();
        params[13] = random_init_mp_checkbox->GetValue();
        params[14] = random_primal_checkbox->GetValue();
        params[15] = random_consensus_checkbox->GetValue();
        params[16] = random_init_md_checkbox->GetValue();
        params[17] = random_dual_checkbox->GetValue();
        params[18] = restr_value_spin_ctrl->GetValue();
        params[19] = tabu_iterations_spin_ctrl->GetValue();
        params[20] = balas_ng_cut_checkbox->GetValue();
        params[21] = max_sol_dual_ag_spin_ctrl->GetValue();
        params[22] = max_sol_primal_ag_spin_ctrl->GetValue();
        params[23] = equal_cost_sol_checkbox->GetValue();
    }

    return params;
}

/* ------------------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------------------- */
