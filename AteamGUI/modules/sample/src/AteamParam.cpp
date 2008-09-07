#include "AteamParam.h"

AteamParam::AteamParam(wxWindow* parent, int id, const wxString& title, const wxPoint& pos, const wxSize& size, long style):
    wxFrame(parent, id, title, pos, size, wxDEFAULT_FRAME_STYLE)
{
    open_button = new wxButton(this, wxID_ANY, _("Open"));
    save_button = new wxButton(this, wxID_ANY, _("Save"));
    reset_button = new wxButton(this, wxID_ANY, _("Reset"));
    apply_button = new wxButton(this, wxID_ANY, _("Apply"));
    hide_button = new wxButton(this, wxID_ANY, _("Hide"));
    new_params_label = new wxStaticText(this, wxID_ANY, _("NEW ATEAM PARAMS"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);

    set_properties();
    do_layout();
}


void AteamParam::set_properties()
{
    SetTitle(_("New A-Team Param"));
    open_button->SetMinSize(wxSize(88, 27));
    save_button->SetMinSize(wxSize(88, 27));
    reset_button->SetMinSize(wxSize(88, 27));
    apply_button->SetMinSize(wxSize(88, 27));
    hide_button->SetMinSize(wxSize(88, 27));
    new_params_label->SetMinSize(wxSize(428, 32));
    new_params_label->SetFont(wxFont(19, wxDEFAULT, wxSLANT, wxNORMAL, 0, wxT("")));
}


void AteamParam::do_layout()
{
    wxFlexGridSizer* grid_sizer_1 = new wxFlexGridSizer(2, 1, 2, 2);
    wxFlexGridSizer* button_grid_sizer = new wxFlexGridSizer(1, 5, 4, 4);
    button_grid_sizer->Add(open_button, 0, 0, 0);
    button_grid_sizer->Add(save_button, 0, 0, 0);
    button_grid_sizer->Add(reset_button, 0, 0, 0);
    button_grid_sizer->Add(apply_button, 0, 0, 0);
    button_grid_sizer->Add(hide_button, 0, 0, 0);
    grid_sizer_1->Add(button_grid_sizer, 1, wxALL|wxEXPAND, 4);
    grid_sizer_1->Add(new_params_label, 0, wxALL, 18);
    SetSizer(grid_sizer_1);
    grid_sizer_1->Fit(this);
    Layout();
}

