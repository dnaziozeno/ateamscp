#include <wx/wx.h>
#include <wx/image.h>

#ifndef INITMEMORIES_H
#define INITMEMORIES_H

class InitMemories: public wxFrame {
public:
    InitMemories(wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_FRAME_STYLE);

private:
    void set_properties();
    void do_layout();

    void onApplyClick(wxCommandEvent &event);
    void onConfigureClick(wxCommandEvent &event);
    void onCancelClick(wxCommandEvent &event);

protected:
    wxButton* apply_button;
    wxButton* configure_button;
    wxButton* cancel_button;
    wxStaticText* new_init_label;
};


#endif // INITMEMORIES_H
