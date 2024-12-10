#include <wx/wx.h>
#include <wx/filedlg.h>
#include <wx/choice.h>
#include <wx/spinctrl.h>

class Mif02Filters : public wxApp {
public:
    virtual bool OnInit();
};

class Mif02FiltersFrame : public wxFrame {
public:
    Mif02FiltersFrame();

private:
    void OnOpen(wxCommandEvent &event);
    void OnSelectFilter(wxCommandEvent &event);
    void OnApplyFilter(wxCommandEvent &event);

 wxBoxSizer* mainSizer;
    wxChoice* filterChoice;
    wxSpinCtrl* kernelSize;
    wxTextCtrl* additionalParam;
	wxStaticText* kernelLabel;
 	wxStaticText* paramLabel;

    wxStaticBitmap* beforeImage;
    wxStaticBitmap* afterImage;

    void UpdateFilterOptions();

    wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(Mif02FiltersFrame, wxFrame)
    EVT_MENU(wxID_OPEN, Mif02FiltersFrame::OnOpen)
    EVT_CHOICE(wxID_ANY, Mif02FiltersFrame::OnSelectFilter)
    EVT_BUTTON(1002, Mif02FiltersFrame::OnApplyFilter)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(Mif02Filters);

bool Mif02Filters::OnInit() {
    Mif02FiltersFrame *frame = new Mif02FiltersFrame();
    frame->Show(true);
    return true;
}

Mif02FiltersFrame::Mif02FiltersFrame()
    : wxFrame(nullptr, wxID_ANY, "Image Filters UI", wxDefaultPosition, wxSize(800, 600)) {
    // Menu Bar
    wxMenu *fileMenu = new wxMenu;
    fileMenu->Append(wxID_OPEN, "&Open Picture\tCtrl-O", "Open a picture file");

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(fileMenu, "&File");
    SetMenuBar(menuBar);

    // Status Bar
    CreateStatusBar();
    SetStatusText("Mif02 - Analyse - TP1");

    // Main Layout
    wxPanel* panel = new wxPanel(this);
   mainSizer = new wxBoxSizer(wxVERTICAL);

    // Image Display
    wxBoxSizer* imageSizer = new wxBoxSizer(wxHORIZONTAL);

    beforeImage = new wxStaticBitmap(panel, wxID_ANY, wxBitmap(200, 200));
    afterImage = new wxStaticBitmap(panel, wxID_ANY, wxBitmap(200, 200));

    imageSizer->Add(new wxStaticText(panel, wxID_ANY, "Before"), 0, wxALIGN_CENTER | wxALL, 5);
    imageSizer->Add(beforeImage, 1, wxEXPAND | wxALL, 5);
    imageSizer->Add(new wxStaticText(panel, wxID_ANY, "After"), 0, wxALIGN_CENTER | wxALL, 5);
    imageSizer->Add(afterImage, 1, wxEXPAND | wxALL, 5);

    mainSizer->Add(imageSizer, 1, wxEXPAND | wxALL, 10);

    // Filter Options
    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

    // Filter Selection
    wxBoxSizer* hbox1 = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* filterLabel = new wxStaticText(panel, wxID_ANY, "Filter:");
    filterChoice = new wxChoice(panel, wxID_ANY);
    filterChoice->Append("------ Choose a filter -------");
    filterChoice->Append("Gaussian");
    filterChoice->Append("Median");
    filterChoice->Append("Sharpen");
    filterChoice->SetSelection(0);
    hbox1->Add(filterLabel, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 5);
    hbox1->Add(filterChoice, 1, wxEXPAND);
    vbox->Add(hbox1, 0, wxEXPAND | wxALL, 10);

    // Kernel Size
    wxBoxSizer* hbox2 = new wxBoxSizer(wxHORIZONTAL);
    kernelLabel = new wxStaticText(panel, wxID_ANY, "Kernel Size:");
    kernelSize = new wxSpinCtrl(panel, wxID_ANY);
    kernelSize->SetRange(1, 31);
    kernelSize->SetValue(3);
    hbox2->Add(kernelLabel, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 5);
    hbox2->Add(kernelSize, 1, wxEXPAND);
    vbox->Add(hbox2, 0, wxEXPAND | wxALL, 10);

    // Additional Parameter
    wxBoxSizer* hbox3 = new wxBoxSizer(wxHORIZONTAL);
    paramLabel = new wxStaticText(panel, wxID_ANY, "Additional Parameter:");
    additionalParam = new wxTextCtrl(panel, wxID_ANY, "0.5");
    hbox3->Add(paramLabel, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 5);
    hbox3->Add(additionalParam, 1, wxEXPAND);
    vbox->Add(hbox3, 0, wxEXPAND | wxALL, 10);

    // Apply Button
    wxButton* applyButton = new wxButton(panel, 1002, "Apply Filter");
    vbox->Add(applyButton, 0, wxALIGN_CENTER | wxALL, 10);

    mainSizer->Add(vbox, 0, wxEXPAND | wxALL, 10);

    panel->SetSizer(mainSizer);

    // Initially hide parameter controls
 	kernelSize->Hide();
	kernelLabel->Hide();

    additionalParam->Hide();
	paramLabel->Hide();
}

void Mif02FiltersFrame::OnOpen(wxCommandEvent &event) {
    wxFileDialog openFileDialog(this, _("Open Image file"), "", "", "Image files (*.png;*.jpg;*.bmp)|*.png;*.jpg;*.bmp", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;

    wxMessageBox("Image file selected: " + openFileDialog.GetPath(), "Info", wxOK | wxICON_INFORMATION, this);
    // Load image into "beforeImage"
}

void Mif02FiltersFrame::OnSelectFilter(wxCommandEvent &event) {
    UpdateFilterOptions();
}

void Mif02FiltersFrame::OnApplyFilter(wxCommandEvent &event) {
    wxString filter = filterChoice->GetStringSelection();
    int kernel = kernelSize->GetValue();
    wxString param = additionalParam->GetValue();

    wxString message = wxString::Format("Applying filter: %s\nKernel Size: %d\nAdditional Param: %s", filter, kernel, param);
    wxMessageBox(message, "Filter Application", wxOK | wxICON_INFORMATION, this);

    // Process and display "afterImage"
}

void Mif02FiltersFrame::UpdateFilterOptions() {
    wxString selectedFilter = filterChoice->GetStringSelection();

    if (selectedFilter == "Median") {
        kernelSize->Show();
		kernelLabel->Show();

        additionalParam->Hide();
		paramLabel->Hide();
    } else if (selectedFilter == "Gaussian") {
        kernelSize->Show();
		kernelLabel->Show();

        additionalParam->Show();
		paramLabel->Show();
    } else { //if (selectedFilter == "Sharpen") {
        kernelSize->Hide();
		kernelLabel->Hide();

        additionalParam->Hide();
		paramLabel->Hide();
    }


    Layout(); // Force layout refresh
	mainSizer->Layout();
}
