#include <wx/wx.h>
#include <wx/image.h>

#ifndef ATEAMPARAM_H
#define ATEAMPARAM_H

class AteamParam: public wxFrame {
public:
    AteamParam(wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_FRAME_STYLE);

private:
    void set_properties();
    void do_layout();

protected:
    wxButton* open_button;
    wxButton* save_button;
    wxButton* reset_button;
    wxButton* apply_button;
    wxButton* hide_button;
    wxStaticText* new_params_label;
};

#endif // ATEAMPARAM_H
