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

#ifndef ATEAMPARAM_H
#define ATEAMPARAM_H

/* ------------------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------------------- */

#include <wx/wx.h>
#include <wx/image.h>

#include <wx/spinctrl.h>
#include <wx/statline.h>

#define STATUS_BAR_LIVE 5000

/* ------------------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------------------- */

class AteamParam: public wxFrame {
public:
    AteamParam(
        wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition,
        const wxSize& size=wxDefaultSize, long style=wxDEFAULT_FRAME_STYLE
    );

private:
    void set_properties();
    void do_layout();

    void onOpenClick(wxCommandEvent &event);
    void onSaveClick(wxCommandEvent &event);
    void onResetClick(wxCommandEvent &event);
    void onApplyClick(wxCommandEvent &event);
    void onHideClick(wxCommandEvent &event);
    void onTimerEvent(wxCommandEvent &event);

    int defaultParams();
    void paramsToInterface(int *params);
    int *interfaceToParams();

    void onCloseWindow(wxCommandEvent &event);

protected:
    wxStatusBar *ateam_param_statusbar;

    wxButton *open_button, *save_button, *reset_button, *apply_button, *hide_button;

    wxStaticLine *button_static_line_01, *check_static_line_01, *check_static_line_02,
        *check_static_line_03, *spin_static_line_01;

    wxCheckBox *dual_worst_checkbox, *primal_worst_checkbox, *random_init_mp_checkbox,
        *random_primal_checkbox, *random_consensus_checkbox, *random_init_md_checkbox,
        *random_dual_checkbox, *balas_ng_cut_checkbox, *equal_cost_sol_checkbox;

    wxStaticText *time_sleeping_label, *max_len_dual_mem_label, *max_len_primal_mem_label,
        *max_len_cut_mem_label, *len_hash_tab_label, *cut_sof_sol_label, *max_cut_gen_label,
        *min_change_label, *max_exe_time_label, *reduc_perc_label, *restr_value_label,
        *tabu_iterations_label, *max_sol_dual_ag_label, * max_sol_primal_ag_label;

    wxSpinCtrl *time_sleeping_spin_ctrl, *max_len_dual_mem_spin_ctrl,
        *max_len_primal_mem_spin_ctrl, *max_len_cut_mem_spin_ctrl, *len_hash_tab_spin_ctrl,
        *cut_sof_sol_spin_ctrl, *max_cut_gen_spin_ctrl, *min_change_spin_ctrl,
        *max_exe_time_spin_ctrl, *reduc_perc_spin_ctrl, *restr_value_spin_ctrl,
        *tabu_iterations_spin_ctrl, *max_sol_dual_ag_spin_ctrl, *max_sol_primal_ag_spin_ctrl;
};

/* ------------------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------------------- */

#endif /* ATEAMPARAM_H */
