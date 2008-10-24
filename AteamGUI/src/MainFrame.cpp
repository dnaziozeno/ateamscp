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
/* Esta classe é responsável pela construção e funcionalidades da janela principal do    */
/* sistema. Sua principal funcionalidade e gerenciar os agentes em execução e permitir   */
/* alterar seus parametros dinamicamente.                                                */
/* ------------------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------------------- */

#include "MainFrame.h"
#include "Graph.h"
#include "IOParam.h"

#include "../include/InitMemories.h"
#include "../include/AteamParam.h"

/* ------------------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------------------- */

Graph *frame = (Graph *) NULL;
wxTreeItemId root_id;

wxTreeItemId agents_tree_id[8];

wxChartCtrl *statistics_chart;
wxChartPoints *statistics_3D_bar;

wxFlexGridSizer* main_grid_sizer;

AteamParam *ateam_param_main_frame;

InitMemories *init_memories;

int nagents[8];
float pagents[8];

extern MPI_Comm comm_three_opt;

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
MainFrame::MainFrame(
    wxWindow* parent, int id, const wxString& title, const wxPoint& pos, const wxSize& size, long style
):wxFrame(
    parent, id, title, pos, size, wxICONIZE|wxCAPTION|wxMINIMIZE|wxCLOSE_BOX|wxMINIMIZE_BOX
)
{
    main_frame_statusbar = CreateStatusBar(1, 0);

    start_button = new wxButton(this, 11, _("Start"));
    stop_button = new wxButton(this, 12, _("Stop"));
    plot_button = new wxButton(this, 13, _("Plot"));
    add_button = new wxButton(this, 14, _("Add"));
    remove_button = new wxButton(this, 15, _("Remove"));
    edit_button = new wxButton(this, 16, _("Edit"));

    Connect(11, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::onStartClick));
    Connect(12, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::onStopClick));
    Connect(13, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::onPlotClick));
    Connect(14, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::onAddClick));
    Connect(15, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::onRemoveClick));
    Connect(16, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::onEditClick));

    button_static_line = new wxStaticLine(this, wxID_ANY);

    team_tree_ctrl = new wxTreeCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER);
    team_tree_ctrl->SetWindowStyle(wxTR_HAS_BUTTONS|wxSUNKEN_BORDER);

    nagents[0] = nagents[1] = nagents[2] = nagents[3] = nagents[4] = nagents[5] = nagents[6] = nagents[7] = 0;
    pagents[0] = pagents[1] = pagents[2] = pagents[3] = pagents[4] = pagents[5] = pagents[6] = pagents[7] = 0.0;

    set_properties();
    do_layout();

    ateam_param_main_frame = new AteamParam(this, wxID_ANY, wxEmptyString, wxPoint(420, 200));
    init_memories = new InitMemories(this, wxID_ANY, wxEmptyString, wxPoint(100, 200));
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void MainFrame::set_properties()
{
    SetTitle(_("A-Team SCP"));
    int main_frame_statusbar_widths[] = { -1 };
    main_frame_statusbar->SetStatusWidths(1, main_frame_statusbar_widths);
    const wxString main_frame_statusbar_fields[] = {
        _("")
    };
    for(int i = 0; i < main_frame_statusbar->GetFieldsCount(); ++i) {
        main_frame_statusbar->SetStatusText(main_frame_statusbar_fields[i], i);
    }
    start_button->SetMinSize(wxSize(105, 27));
    stop_button->SetMinSize(wxSize(105, 27));
    plot_button->SetMinSize(wxSize(105, 27));
    add_button->SetMinSize(wxSize(105, 27));
    remove_button->SetMinSize(wxSize(105, 27));
    edit_button->SetMinSize(wxSize(105, 27));
    team_tree_ctrl->SetMinSize(wxSize(291, 383));

    stop_button->Disable();
    plot_button->Disable();
    add_button->Disable();
    remove_button->Disable();
    edit_button->Disable();

    statistics_3D_bar = wxBar3DChartPoints::CreateWxBar3DChartPoints(wxT("Agents"), wxCHART_GRAY, true);

    statistics_3D_bar->Add( wxT("3OPT"), 1, 0);
    statistics_3D_bar->Add( wxT("SUBG"), 2, 0);
    statistics_3D_bar->Add( wxT("LS"), 3, 0);
    statistics_3D_bar->Add( wxT("PERT"), 4, 0);
    statistics_3D_bar->Add( wxT("CONS"), 5, 0);
    statistics_3D_bar->Add( wxT("PRIMAL"), 6, 0);
    statistics_3D_bar->Add( wxT("DUAL"), 7, 0);

    statistics_chart = new wxChartCtrl(
        this, -1, (STYLE)(USE_AXIS_X|USE_AXIS_Y|USE_GRID),
        wxDefaultPosition, wxSize(200,230), wxSUNKEN_BORDER 
    );

    statistics_chart->Add(statistics_3D_bar);
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void MainFrame::do_layout()
{
    main_grid_sizer = new wxFlexGridSizer(4, 1, 0, 0);
    wxFlexGridSizer* button_grid_sizer = new wxFlexGridSizer(2, 3, 4, 4);

    button_grid_sizer->Add(start_button, 0, 0, 0);
    button_grid_sizer->Add(stop_button, 0, 0, 0);
    button_grid_sizer->Add(plot_button, 0, 0, 0);
    button_grid_sizer->Add(add_button, 0, 0, 0);
    button_grid_sizer->Add(remove_button, 0, 0, 0);
    button_grid_sizer->Add(edit_button, 0, 0, 0);

    main_grid_sizer->Add(button_grid_sizer, 1, wxALL|wxEXPAND, 3);
    main_grid_sizer->Add(button_static_line, 0, wxALL|wxEXPAND, 4);
    main_grid_sizer->Add(team_tree_ctrl, 1, wxALL|wxEXPAND, 4);
    main_grid_sizer->Add(statistics_chart, 1, wxALL|wxEXPAND, 4);

    SetSizer(main_grid_sizer);
    main_grid_sizer->Fit(this);
    Layout();
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void MainFrame::onStartClick(wxCommandEvent &event)
{
    init_memories->Show();
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void MainFrame::onStopClick(wxCommandEvent &event)
{
    MPI_Status status;

    stop_button->Disable();
    plot_button->Disable();
    add_button->Disable();
    remove_button->Disable();
    edit_button->Disable();
    start_button->Enable();

    //team_tree_ctrl->DeleteAllItems();

    wxTreeItemId current_agent_item_id;
    MyTreeItemData *current_agent_item_data;

    for (int agent = THREE_OPT; agent <= DUAL; agent++)
    {
        while (team_tree_ctrl->ItemHasChildren(agents_tree_id[agent]))
        {
            current_agent_item_id = team_tree_ctrl->GetLastChild(agents_tree_id[agent]);
            current_agent_item_data = (MyTreeItemData *) team_tree_ctrl->GetItemData(current_agent_item_id);
            onRemoveAgent(current_agent_item_data->GetType(), current_agent_item_data->GetComm());
            team_tree_ctrl->Delete(current_agent_item_id);
        }
    }

    MPI_Comm interComSpawn1, interComSpawn2, interComSpawn3;
    int errcodes[1];

    MPI_Comm_spawn(
        "/home/naziozeno/Documents/projeto-final/AteamSCP/AteamMPI/stopATEAM",
        MPI_ARGV_NULL, 1, MPI_INFO_NULL, 0,  MPI_COMM_SELF, &interComSpawn1, errcodes
    );

    sleep(1);

    //int teste = 0;
    //MPI_Recv(&teste, 1, MPI_INT, 0, MPI_ANY_TAG, comm_three_opt, &status);

    MPI_Comm_spawn(
        "/home/naziozeno/Documents/projeto-final/AteamSCP/AteamMPI/stopSERVER",
        MPI_ARGV_NULL, 1, MPI_INFO_NULL, 0,  MPI_COMM_SELF, &interComSpawn2, errcodes
    );

    sleep(1);

    MPI_Comm_spawn(
        "/home/naziozeno/Documents/projeto-final/AteamSCP/AteamMPI/stopFINAL",
        MPI_ARGV_NULL, 1, MPI_INFO_NULL, 0,  MPI_COMM_SELF, &interComSpawn3, errcodes
    );

    sleep(1);

    MPI_Finalize();
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void MainFrame::onPlotClick(wxCommandEvent &event)
{
    if (frame == NULL)
    {
        frame = new Graph(this, -1, _T("wxPlot"), wxPoint(420, 80), wxSize(800, 400));
        frame->Show(true);
    }
    else
    {
        frame->Show(true);
    }
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void MainFrame::onAddClick(wxCommandEvent &event)
{
    init_memories->Show();
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void MainFrame::onRemoveClick(wxCommandEvent &event)
{
    wxTreeItemId agent_id = team_tree_ctrl->GetSelection();

    if (agent_id != root_id && agent_id != agents_tree_id[THREE_OPT] && 
        agent_id != agents_tree_id[SUBG] && agent_id != agents_tree_id[LS] &&
        agent_id != agents_tree_id[PERT] && agent_id != agents_tree_id[CONS])
    {
        MyTreeItemData *agent_data = (MyTreeItemData *) team_tree_ctrl->GetItemData(agent_id);
        onRemoveAgent(agent_data->GetType(), agent_data->GetComm());
        team_tree_ctrl->Delete(agent_id);
    }
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void MainFrame::onEditClick(wxCommandEvent &event)
{
    wxTreeItemId agent_id = team_tree_ctrl->GetSelection();

    if (agent_id != root_id)
    {
        MyTreeItemData *data = (MyTreeItemData *) team_tree_ctrl->GetItemData(agent_id);
        ateam_param_main_frame->editParams(data);
        ateam_param_main_frame->Show();
    }
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void MainFrame::onApplyClick()
{
    root_id = team_tree_ctrl->AddRoot(_T("Set Covering Problem - ") + getTime(), -1, -1, NULL);
    team_tree_ctrl->SetItemBold(root_id);

    agents_tree_id[THREE_OPT] = team_tree_ctrl->AppendItem(root_id, _T("1-3OPT (0.00%)"), -1, -1, NULL);
    agents_tree_id[SUBG] = team_tree_ctrl->AppendItem(root_id, _T("2-SUBG (0.00%)"), -1, -1, NULL);
    agents_tree_id[LS] = team_tree_ctrl->AppendItem(root_id, _T("3-LS (0.00%)"), -1, -1, NULL);
    agents_tree_id[PERT] = team_tree_ctrl->AppendItem(root_id, _T("4-PERT (0.00%)"), -1, -1, NULL);
    agents_tree_id[CONS] = team_tree_ctrl->AppendItem(root_id, _T("5-CONS (0.00%)"), -1, -1, NULL);
    agents_tree_id[PRIMAL] = team_tree_ctrl->AppendItem(root_id, _T("6-PRIMAL (0.00%)"), -1, -1, NULL);
    agents_tree_id[DUAL] = team_tree_ctrl->AppendItem(root_id, _T("7-DUAL (0.00%)"), -1, -1, NULL);

    team_tree_ctrl->ExpandAll();
    team_tree_ctrl->SortChildren(root_id);

    stop_button->Enable();
    plot_button->Enable();
    add_button->Enable();
    remove_button->Enable();
    edit_button->Enable();
    start_button->Disable();
}

/* ------------------------------------------------------------------------------------- */
/* Cria uma nova entrada para a arvore de agentes com seus respectivos parametros e      */
/* inicia uma copia desse agente em algum computador da rede computacional.              */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* int type - Tipo do agente adicionado (3OPT, SUBG, LS, PERT, CONS).                    */
/* wxString ip - Endereco IP do computador o qual o agente deve ser executado.           */
/* int mpi_ref - xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx.                      */
/* int *params - Ponteiro para o vetor de parametros do agente.                          */
/* ------------------------------------------------------------------------------------- */
void MainFrame::onAddAgent(int type, wxString ip, MPI_Comm *mpi_ref, int *params)
{
    MyTreeItemData *data = new MyTreeItemData(type, mpi_ref, params);

    switch (type)
    {
        case THREE_OPT:
            team_tree_ctrl->AppendItem(agents_tree_id[THREE_OPT], ip + _("-") + getTime(), -1, -1, data);
            nagents[THREE_OPT]++;
        break;

        case SUBG:
            team_tree_ctrl->AppendItem(agents_tree_id[SUBG], ip + _("-") + getTime(), -1, -1, data);
            nagents[SUBG]++;
            break;

        case LS:
            team_tree_ctrl->AppendItem(agents_tree_id[LS], ip + _("-") + getTime(), -1, -1, data);
            nagents[LS]++;
        break;

        case PERT:
            team_tree_ctrl->AppendItem(agents_tree_id[PERT], ip + _("-") + getTime(), -1, -1, data);
            nagents[PERT]++;
        break;

        case CONS:
            team_tree_ctrl->AppendItem(agents_tree_id[CONS], ip + _("-") + getTime(), -1, -1, data);
            nagents[CONS]++;
        break;

        case PRIMAL:
            team_tree_ctrl->AppendItem(agents_tree_id[PRIMAL], ip + _("-") + getTime(), -1, -1, data);
            nagents[PRIMAL]++;
        break;

        case DUAL:
            team_tree_ctrl->AppendItem(agents_tree_id[DUAL], ip + _("-") + getTime(), -1, -1, data);
            nagents[DUAL]++;
        break;
    }

    nagents[0]++;
    refresh_statistics();

    team_tree_ctrl->SortChildren(root_id);
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void MainFrame::onRemoveAgent(int type, MPI_Comm *mpi_ref)
{
    char change[1];
    int position = 0;
    int size = sizeof(char);
    char *buffer = (char *) malloc(size);

    change[0] = 'S';
    MPI_Pack(change, 1, MPI_CHAR, buffer, size, &position, *mpi_ref);
    MPI_Send(change, 1, MPI_CHAR, 0, 1, *mpi_ref);
    free(buffer);

    MPI_Comm_disconnect(mpi_ref);

    switch (type)
    {
        case THREE_OPT:
            nagents[THREE_OPT]--;
        break;

        case SUBG:
            nagents[SUBG]--;
            break;

        case LS:
            nagents[LS]--;
        break;

        case PERT:
            nagents[PERT]--;
        break;

        case CONS:
            nagents[CONS]--;
        break;

        case PRIMAL:
            nagents[PRIMAL]--;
        break;

        case DUAL:
            nagents[DUAL]--;
        break;
    }

    nagents[0]--;
    refresh_statistics();
}

/* ------------------------------------------------------------------------------------- */
/* Atualiza os dados estatisticos e o grafico das porcentagens de cada agente presente   */
/* no time. Essa funcao deve ser executada toda vez que um agente for adicionado ou      */
/* removido do time.                                                                     */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void MainFrame::refresh_statistics()
{
    /* Calcula a porcentagem de cada agente presente no time. */
    if (nagents[0] == 0)
    {
        pagents[1] = pagents[2] = pagents[3] = pagents[4] = pagents[5] = pagents[6] = pagents[7] = 0.0;
    }
    else
    {
        pagents[1] = ((float)nagents[1]/(float)nagents[0]) * 100;
        pagents[2] = ((float)nagents[2]/(float)nagents[0]) * 100;
        pagents[3] = ((float)nagents[3]/(float)nagents[0]) * 100;
        pagents[4] = ((float)nagents[4]/(float)nagents[0]) * 100;
        pagents[5] = ((float)nagents[5]/(float)nagents[0]) * 100;
        pagents[6] = ((float)nagents[6]/(float)nagents[0]) * 100;
        pagents[7] = ((float)nagents[7]/(float)nagents[0]) * 100;
    }

    /* Cria o objeto que gerencia o grafico de barras. */
    statistics_3D_bar = wxBar3DChartPoints::CreateWxBar3DChartPoints(wxT("Agents"), wxCHART_GRAY, true);

    /* Adiciona a barra correspondente a cada agente com sua respectiva porcentagem. */
    statistics_3D_bar->Add( wxT("3OPT"), 1, pagents[1]);
    statistics_3D_bar->Add( wxT("SUBG"), 2, pagents[2]);
    statistics_3D_bar->Add( wxT("LS"), 3, pagents[3]);
    statistics_3D_bar->Add( wxT("PERT"), 4, pagents[4]);
    statistics_3D_bar->Add( wxT("CONS"), 5, pagents[5]);
    statistics_3D_bar->Add( wxT("PRIMAL"), 6, pagents[6]);
    statistics_3D_bar->Add( wxT("DUAL"), 7, pagents[7]);

    /* Cria o objeto que apresenta visualmente o gráfico */
    statistics_chart = new wxChartCtrl(
        this, -1, (STYLE)(USE_AXIS_X|USE_AXIS_Y|USE_GRID),
        wxDefaultPosition, wxSize(200,230), wxSUNKEN_BORDER 
    );

    statistics_chart->Add(statistics_3D_bar);

    /* Substitui o gráfico atual pelo novo gráfico gerado. */
    main_grid_sizer->Remove(3);
    main_grid_sizer->Insert(3,statistics_chart, 1, wxALL|wxEXPAND, 4);
    Layout();

    /* Atualiza o texto da porcentagem de cada agente presente no time. */
    team_tree_ctrl->SetItemText(agents_tree_id[THREE_OPT], _T("1-3OPT (") + wxString::Format(wxT("%.2f%)"), pagents[THREE_OPT]));
    team_tree_ctrl->SetItemText(agents_tree_id[SUBG], _T("2-SUBG (") + wxString::Format(wxT("%.2f%)"), pagents[SUBG]));
    team_tree_ctrl->SetItemText(agents_tree_id[LS], _T("3-LS (") + wxString::Format(wxT("%.2f%)"), pagents[LS]));
    team_tree_ctrl->SetItemText(agents_tree_id[PERT], _T("4-PERT (") + wxString::Format(wxT("%.2f%)"), pagents[PERT]));
    team_tree_ctrl->SetItemText(agents_tree_id[CONS], _T("5-CONS (") + wxString::Format(wxT("%.2f%)"), pagents[CONS]));
    team_tree_ctrl->SetItemText(agents_tree_id[PRIMAL], _T("6-PRIMAL (") + wxString::Format(wxT("%.2f%)"), pagents[PRIMAL]));
    team_tree_ctrl->SetItemText(agents_tree_id[DUAL], _T("7-DUAL (") + wxString::Format(wxT("%.2f%)"), pagents[DUAL]));
}

/* ------------------------------------------------------------------------------------- */
/*                                                                                       */
/* PARAMETROS:                                                                           */
/* ------------------------------------------------------------------------------------- */
void MainFrame::initMD()
{
/*
    printf("%s\n", IOParam::getExecutablePath());

    char *args[4];

    args[0] = (char *) malloc (512 * sizeof(char));
    args[1] = (char *) malloc (512 * sizeof(char));
    args[2] = (char *) malloc (512 * sizeof(char));
    args[3] = (char *) malloc (512 * sizeof(char));

    strcpy(args[0], IOParam::getExecutablePath());
    strcat(args[0], "/../AteamMPI/initMD");

    strcpy(args[1], "1");

    strcpy(args[2], IOParam::getExecutablePath());
    strcat(args[2], "/../AteamMPI/20_40_20");

    strcpy(args[3], IOParam::getExecutablePath());
    strcat(args[3], "/../AteamMPI/teste");

    //strcpy(args[0], "/home/naziozeno/Documents/projeto-final/AteamSCP/AteamMPI/initMD");
    //strcpy(args[1], "1");
    //strcpy(args[2], "/home/naziozeno/Documents/projeto-final/AteamSCP/AteamMPI/20_40_20");
    //strcpy(args[3], "/home/naziozeno/Documents/projeto-final/AteamSCP/AteamMPI/teste");

    int com = InitMD(4, args);
    MPI_Comm com1 = (MPI_Comm) com;

    //MODIFICAR DEPOIS...
    onApplyClick();
*/
}

/* ------------------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------------------- */

wxString MainFrame::getTime()
{
    wxString str_hour, str_minute, str_second;
    wxDateTime now = wxDateTime::Now();

    int int_hour = now.GetHour();
    int int_minute = now.GetMinute();
    int int_second = now.GetSecond();

    if (int_hour < 10)
    {
        if (int_hour > 0) str_hour = wxString::Format(wxT("0%i"), wxDateTime::ConvertYearToBC(int_hour));
        else str_hour = wxString::Format(wxT("00"));
    }
    else str_hour = wxString::Format(wxT("%i"), wxDateTime::ConvertYearToBC(int_hour));

    if (int_minute < 10)
    {
        if (int_minute > 0) str_minute = wxString::Format(wxT("0%i"), wxDateTime::ConvertYearToBC(int_minute));
        else str_minute = wxString::Format(wxT("00"));
    }
    else str_minute = wxString::Format(wxT("%i"), wxDateTime::ConvertYearToBC(int_minute));

    if (int_second < 10)
    {
        if (int_second > 0) str_second = wxString::Format(wxT("0%i"), wxDateTime::ConvertYearToBC(int_second));
        else str_second = wxString::Format(wxT("00"));
    }
    else str_second = wxString::Format(wxT("%i"), wxDateTime::ConvertYearToBC(int_second));

    return str_hour + _(":") + str_minute + _(":") + str_second;
}



MyTreeItemData::MyTreeItemData(int type, MPI_Comm *comm, int *params):wxTreeItemData()
{
    type_t = type;
    comm_t = comm;
    params_t = params;
}

void MyTreeItemData::GetId(){}
void MyTreeItemData::SetId(){}

void MyTreeItemData::SetComm(MPI_Comm *comm)
{
    if (comm_t != NULL) free(comm_t);
    comm_t = comm;
}

MPI_Comm *MyTreeItemData::GetComm()
{
    return comm_t;
}

void MyTreeItemData::SetType(int type)
{
    type_t = type;
}

int MyTreeItemData::GetType()
{
    return type_t;
}

void MyTreeItemData::SetParams(int *params)
{
    if (params_t != NULL) free(params_t);
    params_t = params;
}

int *MyTreeItemData::GetParams()
{
    return params_t;
}

