
#include "InitMemories.h"
#include "AteamParam.h"

InitMemories::InitMemories(wxWindow* parent, int id, const wxString& title, const wxPoint& pos, const wxSize& size, long style):
    wxFrame(parent, id, title, pos, size, wxDEFAULT_FRAME_STYLE)
{
    apply_button = new wxButton(this, 120, _("Apply"));
    configure_button = new wxButton(this, 121, _("Configure"));
    cancel_button = new wxButton(this, 122, _("Cancel"));
    new_init_label = new wxStaticText(this, wxID_ANY, _("NEW INIT MEMORIES"));

    set_properties();
    do_layout();

    Connect(120, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(InitMemories::onApplyClick));
    Connect(121, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(InitMemories::onConfigureClick));
    Connect(122, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(InitMemories::onCancelClick));
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

void InitMemories::set_properties()
{
    SetTitle(_("New InitMemories"));
    apply_button->SetMinSize(wxSize(90, 27));
    configure_button->SetMinSize(wxSize(90, 27));
    cancel_button->SetMinSize(wxSize(90, 27));
    new_init_label->SetMinSize(wxSize(260, 32));
    new_init_label->SetFont(wxFont(19, wxDEFAULT, wxSLANT, wxNORMAL, 0, wxT("")));
}


void InitMemories::do_layout()
{
    wxFlexGridSizer* grid_sizer_1 = new wxFlexGridSizer(2, 1, 2, 2);
    wxFlexGridSizer* grid_sizer_2 = new wxFlexGridSizer(1, 3, 2, 2);
    grid_sizer_2->Add(apply_button, 0, wxTOP, 4);
    grid_sizer_2->Add(configure_button, 0, wxTOP, 4);
    grid_sizer_2->Add(cancel_button, 0, wxTOP, 4);
    grid_sizer_1->Add(grid_sizer_2, 1, wxLEFT|wxRIGHT|wxEXPAND, 2);
    grid_sizer_1->Add(new_init_label, 0, wxALL, 8);
    SetSizer(grid_sizer_1);
    grid_sizer_1->Fit(this);
    Layout();
}
