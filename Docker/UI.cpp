//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: UI.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#include "UI.h"

// Declare the bitmap loading function
extern void wxCrafterOTgKroInitBitmapResources();

static bool bBitmapLoaded = false;

NewDockerWorkspaceDlgBase::NewDockerWorkspaceDlgBase(wxWindow* parent, wxWindowID id, const wxString& title,
                                                     const wxPoint& pos, const wxSize& size, long style)
    : wxDialog(parent, id, title, pos, size, style)
{
    if(!bBitmapLoaded) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxCrafterOTgKroInitBitmapResources();
        bBitmapLoaded = true;
    }

    wxBoxSizer* boxSizer2 = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(boxSizer2);

    wxFlexGridSizer* flexGridSizer10 = new wxFlexGridSizer(0, 2, 0, 0);
    flexGridSizer10->SetFlexibleDirection(wxBOTH);
    flexGridSizer10->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);
    flexGridSizer10->AddGrowableCol(1);

    boxSizer2->Add(flexGridSizer10, 1, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    m_staticText16 =
        new wxStaticText(this, wxID_ANY, _("Name:"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);

    flexGridSizer10->Add(m_staticText16, 0, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP(5));

    m_textCtrlName = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(300, -1)), 0);
    m_textCtrlName->SetFocus();
#if wxVERSION_NUMBER >= 3000
    m_textCtrlName->SetHint(wxT(""));
#endif

    flexGridSizer10->Add(m_textCtrlName, 0, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    m_workspacePath =
        new wxStaticText(this, wxID_ANY, _("Workspace Path:"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);

    flexGridSizer10->Add(m_workspacePath, 0, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP(5));

    m_dirPicker14 = new wxDirPickerCtrl(this, wxID_ANY, wxEmptyString, _("Select a folder"), wxDefaultPosition,
                                        wxDLG_UNIT(this, wxSize(-1, -1)),
                                        wxDIRP_SMALL | wxDIRP_DEFAULT_STYLE | wxDIRP_USE_TEXTCTRL);

    flexGridSizer10->Add(m_dirPicker14, 0, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    m_stdBtnSizer4 = new wxStdDialogButtonSizer();

    boxSizer2->Add(m_stdBtnSizer4, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, WXC_FROM_DIP(5));

    m_button6 = new wxButton(this, wxID_OK, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_button6->SetDefault();
    m_stdBtnSizer4->AddButton(m_button6);

    m_button8 = new wxButton(this, wxID_CANCEL, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_stdBtnSizer4->AddButton(m_button8);
    m_stdBtnSizer4->Realize();

    SetName(wxT("NewDockerWorkspaceDlgBase"));
    SetSize(wxDLG_UNIT(this, wxSize(-1, -1)));
    if(GetSizer()) { GetSizer()->Fit(this); }
    if(GetParent()) {
        CentreOnParent(wxBOTH);
    } else {
        CentreOnScreen(wxBOTH);
    }
#if wxVERSION_NUMBER >= 2900
    if(!wxPersistenceManager::Get().Find(this)) {
        wxPersistenceManager::Get().RegisterAndRestore(this);
    } else {
        wxPersistenceManager::Get().Restore(this);
    }
#endif
    // Connect events
    m_button6->Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(NewDockerWorkspaceDlgBase::OnOkUI), NULL, this);
}

NewDockerWorkspaceDlgBase::~NewDockerWorkspaceDlgBase()
{
    m_button6->Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(NewDockerWorkspaceDlgBase::OnOkUI), NULL, this);
}

DockerSettingsBaseDlg::DockerSettingsBaseDlg(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos,
                                             const wxSize& size, long style)
    : wxDialog(parent, id, title, pos, size, style)
{
    if(!bBitmapLoaded) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxCrafterOTgKroInitBitmapResources();
        bBitmapLoaded = true;
    }

    wxBoxSizer* boxSizer22 = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(boxSizer22);

    wxFlexGridSizer* flexGridSizer30 = new wxFlexGridSizer(0, 2, 0, 0);
    flexGridSizer30->SetFlexibleDirection(wxBOTH);
    flexGridSizer30->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);
    flexGridSizer30->AddGrowableCol(1);

    boxSizer22->Add(flexGridSizer30, 1, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    m_staticText32 =
        new wxStaticText(this, wxID_ANY, _("Docker"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);

    flexGridSizer30->Add(m_staticText32, 0, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP(5));

    m_filePickerDocker =
        new wxFilePickerCtrl(this, wxID_ANY, wxEmptyString, _("Select a file"), wxT("*"), wxDefaultPosition,
                             wxDLG_UNIT(this, wxSize(-1, -1)), wxFLP_DEFAULT_STYLE | wxFLP_USE_TEXTCTRL | wxFLP_SMALL);
    m_filePickerDocker->SetFocus();

    flexGridSizer30->Add(m_filePickerDocker, 0, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    m_staticText36 =
        new wxStaticText(this, wxID_ANY, _("Docker compose"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);

    flexGridSizer30->Add(m_staticText36, 0, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP(5));

    m_filePickerDockerCompose =
        new wxFilePickerCtrl(this, wxID_ANY, wxEmptyString, _("Select a file"), wxT("*"), wxDefaultPosition,
                             wxDLG_UNIT(this, wxSize(-1, -1)), wxFLP_DEFAULT_STYLE | wxFLP_USE_TEXTCTRL | wxFLP_SMALL);

    flexGridSizer30->Add(m_filePickerDockerCompose, 0, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    m_stdBtnSizer24 = new wxStdDialogButtonSizer();

    boxSizer22->Add(m_stdBtnSizer24, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, WXC_FROM_DIP(5));

    m_button26 = new wxButton(this, wxID_OK, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_button26->SetDefault();
    m_stdBtnSizer24->AddButton(m_button26);

    m_button28 = new wxButton(this, wxID_CANCEL, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_stdBtnSizer24->AddButton(m_button28);
    m_stdBtnSizer24->Realize();

    SetName(wxT("DockerSettingsBaseDlg"));
    SetSize(wxDLG_UNIT(this, wxSize(-1, -1)));
    if(GetSizer()) { GetSizer()->Fit(this); }
    if(GetParent()) {
        CentreOnParent(wxBOTH);
    } else {
        CentreOnScreen(wxBOTH);
    }
#if wxVERSION_NUMBER >= 2900
    if(!wxPersistenceManager::Get().Find(this)) {
        wxPersistenceManager::Get().RegisterAndRestore(this);
    } else {
        wxPersistenceManager::Get().Restore(this);
    }
#endif
    // Connect events
    m_button26->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DockerSettingsBaseDlg::OnOK), NULL, this);
}

DockerSettingsBaseDlg::~DockerSettingsBaseDlg()
{
    m_button26->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DockerSettingsBaseDlg::OnOK), NULL,
                           this);
}

DockerfileSettingsDlgBase::DockerfileSettingsDlgBase(wxWindow* parent, wxWindowID id, const wxString& title,
                                                     const wxPoint& pos, const wxSize& size, long style)
    : wxDialog(parent, id, title, pos, size, style)
{
    if(!bBitmapLoaded) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxCrafterOTgKroInitBitmapResources();
        bBitmapLoaded = true;
    }

    wxBoxSizer* boxSizer42 = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(boxSizer42);

    m_panel114 = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(300, 200)), wxTAB_TRAVERSAL);

    boxSizer42->Add(m_panel114, 1, wxEXPAND, WXC_FROM_DIP(5));

    wxBoxSizer* boxSizer116 = new wxBoxSizer(wxVERTICAL);
    m_panel114->SetSizer(boxSizer116);

    m_notebook96 =
        new Notebook(m_panel114, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_panel114, wxSize(-1, -1)), wxBK_DEFAULT);
    m_notebook96->SetName(wxT("m_notebook96"));

    boxSizer116->Add(m_notebook96, 1, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    m_panel98 = new wxPanel(m_notebook96, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_notebook96, wxSize(-1, -1)),
                            wxTAB_TRAVERSAL);
    m_notebook96->AddPage(m_panel98, _("Build"), true);

    wxBoxSizer* boxSizer102 = new wxBoxSizer(wxVERTICAL);
    m_panel98->SetSizer(boxSizer102);

    m_staticText106 = new wxStaticText(m_panel98, wxID_ANY, _("Build options:"), wxDefaultPosition,
                                       wxDLG_UNIT(m_panel98, wxSize(-1, -1)), 0);

    boxSizer102->Add(m_staticText106, 0, wxALL, WXC_FROM_DIP(5));

    m_stcBuild = new wxStyledTextCtrl(m_panel98, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_panel98, wxSize(-1, -1)), 0);
    m_stcBuild->SetToolTip(
        _("Set here the build options to pass to the\n'docker build' command.\nFor example:\n'-t my_container .'"));
    m_stcBuild->SetFocus();
    // Configure the fold margin
    m_stcBuild->SetMarginType(4, wxSTC_MARGIN_SYMBOL);
    m_stcBuild->SetMarginMask(4, wxSTC_MASK_FOLDERS);
    m_stcBuild->SetMarginSensitive(4, true);
    m_stcBuild->SetMarginWidth(4, 0);

    // Configure the tracker margin
    m_stcBuild->SetMarginWidth(1, 0);

    // Configure the symbol margin
    m_stcBuild->SetMarginType(2, wxSTC_MARGIN_SYMBOL);
    m_stcBuild->SetMarginMask(2, ~(wxSTC_MASK_FOLDERS));
    m_stcBuild->SetMarginWidth(2, 0);
    m_stcBuild->SetMarginSensitive(2, true);

    // Configure the line numbers margin
    m_stcBuild->SetMarginType(0, wxSTC_MARGIN_NUMBER);
    m_stcBuild->SetMarginWidth(0, 0);

    // Configure the line symbol margin
    m_stcBuild->SetMarginType(3, wxSTC_MARGIN_FORE);
    m_stcBuild->SetMarginMask(3, 0);
    m_stcBuild->SetMarginWidth(3, 0);
    // Select the lexer
    m_stcBuild->SetLexer(wxSTC_LEX_NULL);
    // Set default font / styles
    m_stcBuild->StyleClearAll();
    m_stcBuild->SetWrapMode(1);
    m_stcBuild->SetIndentationGuides(0);
    m_stcBuild->SetKeyWords(0, wxT(""));
    m_stcBuild->SetKeyWords(1, wxT(""));
    m_stcBuild->SetKeyWords(2, wxT(""));
    m_stcBuild->SetKeyWords(3, wxT(""));
    m_stcBuild->SetKeyWords(4, wxT(""));

    boxSizer102->Add(m_stcBuild, 1, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    m_panel100 = new wxPanel(m_notebook96, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_notebook96, wxSize(-1, -1)),
                             wxTAB_TRAVERSAL);
    m_notebook96->AddPage(m_panel100, _("Run"), false);

    wxBoxSizer* boxSizer104 = new wxBoxSizer(wxVERTICAL);
    m_panel100->SetSizer(boxSizer104);

    m_staticText110 = new wxStaticText(m_panel100, wxID_ANY, _("Run options:"), wxDefaultPosition,
                                       wxDLG_UNIT(m_panel100, wxSize(-1, -1)), 0);

    boxSizer104->Add(m_staticText110, 0, wxALL, WXC_FROM_DIP(5));

    m_stcRun = new wxStyledTextCtrl(m_panel100, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_panel100, wxSize(-1, -1)), 0);
    m_stcRun->SetToolTip(
        _("Set here the build options to pass to the\n'docker run' command.\nFor example:\n'-i my_container .'"));
    // Configure the fold margin
    m_stcRun->SetMarginType(4, wxSTC_MARGIN_SYMBOL);
    m_stcRun->SetMarginMask(4, wxSTC_MASK_FOLDERS);
    m_stcRun->SetMarginSensitive(4, true);
    m_stcRun->SetMarginWidth(4, 0);

    // Configure the tracker margin
    m_stcRun->SetMarginWidth(1, 0);

    // Configure the symbol margin
    m_stcRun->SetMarginType(2, wxSTC_MARGIN_SYMBOL);
    m_stcRun->SetMarginMask(2, ~(wxSTC_MASK_FOLDERS));
    m_stcRun->SetMarginWidth(2, 0);
    m_stcRun->SetMarginSensitive(2, true);

    // Configure the line numbers margin
    m_stcRun->SetMarginType(0, wxSTC_MARGIN_NUMBER);
    m_stcRun->SetMarginWidth(0, 0);

    // Configure the line symbol margin
    m_stcRun->SetMarginType(3, wxSTC_MARGIN_FORE);
    m_stcRun->SetMarginMask(3, 0);
    m_stcRun->SetMarginWidth(3, 0);
    // Select the lexer
    m_stcRun->SetLexer(wxSTC_LEX_NULL);
    // Set default font / styles
    m_stcRun->StyleClearAll();
    m_stcRun->SetWrapMode(1);
    m_stcRun->SetIndentationGuides(0);
    m_stcRun->SetKeyWords(0, wxT(""));
    m_stcRun->SetKeyWords(1, wxT(""));
    m_stcRun->SetKeyWords(2, wxT(""));
    m_stcRun->SetKeyWords(3, wxT(""));
    m_stcRun->SetKeyWords(4, wxT(""));

    boxSizer104->Add(m_stcRun, 1, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    m_stdBtnSizer44 = new wxStdDialogButtonSizer();

    boxSizer42->Add(m_stdBtnSizer44, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, WXC_FROM_DIP(5));

    m_button46 = new wxButton(this, wxID_OK, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_button46->SetDefault();
    m_stdBtnSizer44->AddButton(m_button46);

    m_button48 = new wxButton(this, wxID_CANCEL, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_stdBtnSizer44->AddButton(m_button48);
    m_stdBtnSizer44->Realize();

    SetName(wxT("DockerfileSettingsDlgBase"));
    SetSize(wxDLG_UNIT(this, wxSize(-1, -1)));
    if(GetSizer()) { GetSizer()->Fit(this); }
    if(GetParent()) {
        CentreOnParent(wxBOTH);
    } else {
        CentreOnScreen(wxBOTH);
    }
    // Connect events
    m_button46->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DockerfileSettingsDlgBase::OnOK), NULL,
                        this);
}

DockerfileSettingsDlgBase::~DockerfileSettingsDlgBase()
{
    m_button46->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DockerfileSettingsDlgBase::OnOK), NULL,
                           this);
}

DockerOutputPaneBase::DockerOutputPaneBase(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size,
                                           long style)
    : wxPanel(parent, id, pos, size, style)
{
    if(!bBitmapLoaded) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxCrafterOTgKroInitBitmapResources();
        bBitmapLoaded = true;
    }

    wxBoxSizer* boxSizer64 = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(boxSizer64);

    m_notebook = new Notebook(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), wxBK_DEFAULT);
    m_notebook->SetName(wxT("m_notebook"));

    boxSizer64->Add(m_notebook, 1, wxEXPAND, WXC_FROM_DIP(5));

    m_outputPage =
        new wxPanel(m_notebook, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_notebook, wxSize(-1, -1)), wxTAB_TRAVERSAL);
    m_notebook->AddPage(m_outputPage, _("Output"), true);

    wxBoxSizer* boxSizer76 = new wxBoxSizer(wxVERTICAL);
    m_outputPage->SetSizer(boxSizer76);

    m_toolbar = new clToolBar(m_outputPage, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_outputPage, wxSize(-1, -1)),
                              wxTB_NODIVIDER | wxTB_FLAT);
    m_toolbar->SetToolBitmapSize(wxSize(16, 16));

    boxSizer76->Add(m_toolbar, 0, wxEXPAND, WXC_FROM_DIP(5));

    m_stc =
        new wxStyledTextCtrl(m_outputPage, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_outputPage, wxSize(-1, -1)), 0);
    // Configure the fold margin
    m_stc->SetMarginType(4, wxSTC_MARGIN_SYMBOL);
    m_stc->SetMarginMask(4, wxSTC_MASK_FOLDERS);
    m_stc->SetMarginSensitive(4, true);
    m_stc->SetMarginWidth(4, 0);

    // Configure the tracker margin
    m_stc->SetMarginWidth(1, 0);

    // Configure the symbol margin
    m_stc->SetMarginType(2, wxSTC_MARGIN_SYMBOL);
    m_stc->SetMarginMask(2, ~(wxSTC_MASK_FOLDERS));
    m_stc->SetMarginWidth(2, 0);
    m_stc->SetMarginSensitive(2, true);

    // Configure the line numbers margin
    m_stc->SetMarginType(0, wxSTC_MARGIN_NUMBER);
    m_stc->SetMarginWidth(0, 0);

    // Configure the line symbol margin
    m_stc->SetMarginType(3, wxSTC_MARGIN_FORE);
    m_stc->SetMarginMask(3, 0);
    m_stc->SetMarginWidth(3, 0);
    // Select the lexer
    m_stc->SetLexer(wxSTC_LEX_NULL);
    // Set default font / styles
    m_stc->StyleClearAll();
    m_stc->SetWrapMode(1);
    m_stc->SetIndentationGuides(0);
    m_stc->SetKeyWords(0, wxT(""));
    m_stc->SetKeyWords(1, wxT(""));
    m_stc->SetKeyWords(2, wxT(""));
    m_stc->SetKeyWords(3, wxT(""));
    m_stc->SetKeyWords(4, wxT(""));

    boxSizer76->Add(m_stc, 1, wxEXPAND, WXC_FROM_DIP(5));

    m_containersPage =
        new wxPanel(m_notebook, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_notebook, wxSize(-1, -1)), wxTAB_TRAVERSAL);
    m_notebook->AddPage(m_containersPage, _("Containers"), false);

    wxBoxSizer* boxSizer78 = new wxBoxSizer(wxVERTICAL);
    m_containersPage->SetSizer(boxSizer78);

    m_toolbarContainers = new clToolBar(m_containersPage, wxID_ANY, wxDefaultPosition,
                                        wxDLG_UNIT(m_containersPage, wxSize(-1, -1)), wxTB_FLAT);
    m_toolbarContainers->SetToolBitmapSize(wxSize(16, 16));

    boxSizer78->Add(m_toolbarContainers, 0, wxEXPAND, WXC_FROM_DIP(5));

    m_dvListCtrlContainers = new wxDataViewListCtrl(m_containersPage, wxID_ANY, wxDefaultPosition,
                                                    wxDLG_UNIT(m_containersPage, wxSize(-1, -1)),
                                                    wxDV_VERT_RULES | wxDV_ROW_LINES | wxDV_MULTIPLE | wxDV_SINGLE);

    boxSizer78->Add(m_dvListCtrlContainers, 1, wxEXPAND, WXC_FROM_DIP(5));

    m_dvListCtrlContainers->AppendTextColumn(_("ID"), wxDATAVIEW_CELL_INERT, WXC_FROM_DIP(-2), wxALIGN_LEFT,
                                             wxDATAVIEW_COL_RESIZABLE);
    m_dvListCtrlContainers->AppendTextColumn(_("Image"), wxDATAVIEW_CELL_INERT, WXC_FROM_DIP(-2), wxALIGN_LEFT,
                                             wxDATAVIEW_COL_RESIZABLE);
    m_dvListCtrlContainers->AppendTextColumn(_("Command"), wxDATAVIEW_CELL_INERT, WXC_FROM_DIP(-2), wxALIGN_LEFT,
                                             wxDATAVIEW_COL_RESIZABLE);
    m_dvListCtrlContainers->AppendTextColumn(_("Created"), wxDATAVIEW_CELL_INERT, WXC_FROM_DIP(-2), wxALIGN_LEFT,
                                             wxDATAVIEW_COL_RESIZABLE);
    m_dvListCtrlContainers->AppendTextColumn(_("Status"), wxDATAVIEW_CELL_INERT, WXC_FROM_DIP(-2), wxALIGN_LEFT,
                                             wxDATAVIEW_COL_RESIZABLE);
    m_dvListCtrlContainers->AppendTextColumn(_("Ports"), wxDATAVIEW_CELL_INERT, WXC_FROM_DIP(-2), wxALIGN_LEFT,
                                             wxDATAVIEW_COL_RESIZABLE);
    m_dvListCtrlContainers->AppendTextColumn(_("Names"), wxDATAVIEW_CELL_INERT, WXC_FROM_DIP(-2), wxALIGN_LEFT,
                                             wxDATAVIEW_COL_RESIZABLE);
    m_imagesPage =
        new wxPanel(m_notebook, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_notebook, wxSize(-1, -1)), wxTAB_TRAVERSAL);
    m_notebook->AddPage(m_imagesPage, _("Images"), false);

    wxBoxSizer* boxSizer90 = new wxBoxSizer(wxVERTICAL);
    m_imagesPage->SetSizer(boxSizer90);

    m_toolbarImages =
        new clToolBar(m_imagesPage, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_imagesPage, wxSize(-1, -1)), wxTB_FLAT);
    m_toolbarImages->SetToolBitmapSize(wxSize(16, 16));

    boxSizer90->Add(m_toolbarImages, 0, wxEXPAND, WXC_FROM_DIP(5));

    m_dvListCtrlImages =
        new wxDataViewListCtrl(m_imagesPage, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_imagesPage, wxSize(-1, -1)),
                               wxDV_VERT_RULES | wxDV_ROW_LINES | wxDV_MULTIPLE | wxDV_SINGLE);

    boxSizer90->Add(m_dvListCtrlImages, 1, wxEXPAND, WXC_FROM_DIP(5));

    m_dvListCtrlImages->AppendTextColumn(_("ID"), wxDATAVIEW_CELL_INERT, WXC_FROM_DIP(-2), wxALIGN_LEFT,
                                         wxDATAVIEW_COL_RESIZABLE);
    m_dvListCtrlImages->AppendTextColumn(_("Repository"), wxDATAVIEW_CELL_INERT, WXC_FROM_DIP(-2), wxALIGN_LEFT,
                                         wxDATAVIEW_COL_RESIZABLE);
    m_dvListCtrlImages->AppendTextColumn(_("Tag"), wxDATAVIEW_CELL_INERT, WXC_FROM_DIP(-2), wxALIGN_LEFT,
                                         wxDATAVIEW_COL_RESIZABLE);
    m_dvListCtrlImages->AppendTextColumn(_("CreatedAt"), wxDATAVIEW_CELL_INERT, WXC_FROM_DIP(-2), wxALIGN_LEFT,
                                         wxDATAVIEW_COL_RESIZABLE);
    m_dvListCtrlImages->AppendTextColumn(_("Size"), wxDATAVIEW_CELL_INERT, WXC_FROM_DIP(-2), wxALIGN_LEFT,
                                         wxDATAVIEW_COL_RESIZABLE);

    SetName(wxT("DockerOutputPaneBase"));
    SetSize(wxDLG_UNIT(this, wxSize(500, 300)));
    if(GetSizer()) { GetSizer()->Fit(this); }
    // Connect events
    m_dvListCtrlContainers->Connect(wxEVT_COMMAND_DATAVIEW_ITEM_CONTEXT_MENU,
                                    wxDataViewEventHandler(DockerOutputPaneBase::OnContainerContextMenu), NULL, this);
}

DockerOutputPaneBase::~DockerOutputPaneBase()
{
    m_dvListCtrlContainers->Disconnect(wxEVT_COMMAND_DATAVIEW_ITEM_CONTEXT_MENU,
                                       wxDataViewEventHandler(DockerOutputPaneBase::OnContainerContextMenu), NULL,
                                       this);
}
