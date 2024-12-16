#include <wx/wx.h>
#include <string_view>

using namespace std;

struct OddIntegerInput {
	wxSlider* slider = nullptr;

	string_view label;
	int default_value;
	int min_value;
	int max_value;

	OddIntegerInput(string_view label, int default_value, int min_value, int max_value)
		: label(label), default_value(default_value), min_value(min_value), max_value(max_value) {

		if (default_value % 2 == 0) {
			default_value += 1;
		}
	}

	void setupUi(wxBoxSizer* vbox, wxPanel* panel) {
		wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);

		wxStaticText* paramLabel = new wxStaticText(panel, wxID_ANY, wxString(label.data(), label.size()));
		hbox->Add(paramLabel, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 5);

		slider = new wxSlider(panel, wxID_ANY, default_value, min_value, max_value, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_LABELS);
		hbox->Add(slider, 1, wxEXPAND);
		vbox->Add(hbox, 0, wxEXPAND | wxALL, 10);

		slider->Bind(wxEVT_SLIDER, &OddIntegerInput::onSliderChange, this);
	}

	void onSliderChange(wxCommandEvent& event) {
		int value = slider->GetValue();
		if (value % 2 == 0) {
			value += (value < max_value) ? 1 : -1;
			slider->SetValue(value);
		}
	}
};
