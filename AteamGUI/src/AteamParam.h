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

#ifndef ATEAMPARAM_H
#define ATEAMPARAM_H

#include <wx/spinctrl.h>
#include <wx/statline.h>

class AteamParam: public wxFrame {
public:
    AteamParam(
        wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition,
        const wxSize& size=wxDefaultSize, long style=wxDEFAULT_FRAME_STYLE
    );

private:
    void set_properties();
    void do_layout();

    void onOpen(wxCommandEvent &event);
    void onSave(wxCommandEvent &event);
    void onReset(wxCommandEvent &event);
    void onApply(wxCommandEvent &event);
    void onHide(wxCommandEvent &event);

    void defaultParams();
    void paramsToInterface(int *params);
    int *interfaceToParams();

protected:
    wxStatusBar* ateam_param_statusbar;

    wxButton* open_button;
    wxButton* save_button;
    wxButton* reset_button;
    wxButton* apply_button;
    wxButton* hide_button;

    wxStaticLine* button_static_line_01;

    wxCheckBox* dual_worst_checkbox;
    wxCheckBox* primal_worst_checkbox;
    wxCheckBox* random_init_mp_checkbox;

    wxStaticLine* check_static_line_01;

    wxCheckBox* random_primal_checkbox;
    wxCheckBox* random_consensus_checkbox;
    wxCheckBox* random_init_md_checkbox;

    wxStaticLine* check_static_line_02;

    wxCheckBox* random_dual_checkbox;
    wxCheckBox* balas_ng_cut_checkbox;
    wxCheckBox* equal_cost_sol_checkbox;

    wxStaticLine* check_static_line_03;

    wxStaticText* time_sleeping_label;
    wxSpinCtrl* time_sleeping_spin_ctrl;

    wxStaticText* max_len_dual_mem_label;
    wxSpinCtrl* max_len_dual_mem_spin_ctrl;

    wxStaticText* max_len_primal_mem_label;
    wxSpinCtrl* max_len_primal_mem_spin_ctrl;

    wxStaticText* max_len_cut_mem_label;
    wxSpinCtrl* max_len_cut_mem_spin_ctrl;

    wxStaticText* len_hash_tab_label;
    wxSpinCtrl* len_hash_tab_spin_ctrl;

    wxStaticText* cut_sof_sol_label;
    wxSpinCtrl* cut_sof_sol_spin_ctrl;

    wxStaticText* max_cut_gen_label;
    wxSpinCtrl* max_cut_gen_spin_ctrl;

    wxStaticLine* spin_static_line_01;

    wxStaticText* min_change_label;
    wxSpinCtrl* min_change_spin_ctrl;

    wxStaticText* max_exe_time_label;
    wxSpinCtrl* max_exe_time_spin_ctrl;

    wxStaticText* reduc_perc_label;
    wxSpinCtrl* reduc_perc_spin_ctrl;

    wxStaticText* restr_value_label;
    wxSpinCtrl* restr_value_spin_ctrl;

    wxStaticText* tabu_iterations_label;
    wxSpinCtrl* tabu_iterations_spin_ctrl;

    wxStaticText* max_sol_dual_ag_label;
    wxSpinCtrl* max_sol_dual_ag_spin_ctrl;

    wxStaticText* max_sol_primal_ag_label;
    wxSpinCtrl* max_sol_primal_ag_spin_ctrl;
};

#endif // ATEAMPARAM_H

/* ------------------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------------------- */
