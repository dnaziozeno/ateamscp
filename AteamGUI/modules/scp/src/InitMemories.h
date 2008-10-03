#include <wx/wx.h>
#include <wx/image.h>

#include <mpi.h>

#ifndef INITMEMORIES_H
#define INITMEMORIES_H

#include <wx/statline.h>

class InitMemories: public wxFrame {
public:
    InitMemories(wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_FRAME_STYLE);

private:
    void set_properties();
    void do_layout();

    void onTimerMDEvent(wxCommandEvent &event);
    void onTimerMPEvent(wxCommandEvent &event);
    void onApplyClick(wxCommandEvent &event);
    void onConfigureClick(wxCommandEvent &event);
    void onCancelClick(wxCommandEvent &event);
    void onInitMDClick(wxCommandEvent &event);
    void onThreeOPTClick(wxCommandEvent &event);
    void onSubGClick(wxCommandEvent &event);
    void onInitMPClick(wxCommandEvent &event);
    void onLSClick(wxCommandEvent &event);
    void onPertClick(wxCommandEvent &event);
    void onConsClick(wxCommandEvent &event);

protected:
    wxStatusBar* init_memories_statusbar;
    wxButton* apply_button;
    wxButton* configure_button;
    wxButton* cancel_button;
    wxStaticLine* top_static_line;
    wxButton* init_md_button;
    wxComboBox* init_md_combo_box;
    wxButton* three_opt_button;
    wxComboBox* three_opt_combo_box;
    wxButton* subg_button;
    wxComboBox* subg_combo_box;
    wxStaticLine* mid_static_line;
    wxButton* init_mp_button;
    wxComboBox* init_mp_combo_box;
    wxButton* ls_button;
    wxComboBox* ls_combo_box;
    wxButton* pert_button;
    wxComboBox* pert_combo_box;
    wxButton* cons_button;
    wxComboBox* cons_combo_box;
};

#endif // INITMEMORIES_H
