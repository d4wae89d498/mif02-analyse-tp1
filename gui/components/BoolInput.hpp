#pragma once

struct BoolInput
{
	wxCheckBox* checkbox = nullptr;

	string_view 	label;
	bool 			default_value;

	BoolInput(string_view label = "", bool default_value = "") : label(label), default_value(default_value)
	{
	}

	fn setupUi(wxBoxSizer* vbox, wxPanel* panel, std::optional<string_view> label_opt = std::nullopt, std::optional<bool> default_value_opt = std::nullopt) -> void
	{
		string_view label = label_opt.value_or(this->label);
		bool 		default_value = default_value_opt.value_or(this->default_value);

		checkbox = new wxCheckBox(panel, wxID_ANY, wxString(label.data(), label.size()));
		checkbox->SetValue(default_value);
		vbox->Add(checkbox, 0, wxALL, 10);
	}
};

/*

	wxCheckBox* opencvCheckbox = nullptr;


	fn getName() const -> string_view override
	{
		return "Sobel";
	}

	fn setupUi(wxBoxSizer* vbox, wxPanel* panel) -> void override
	{
		wxBoxSizer* hbox3 = new wxBoxSizer(wxHORIZONTAL);
		wxStaticText* paramLabel = new wxStaticText(panel, wxID_ANY, "Taille du kernel :");
		kernelCtrl = new wxTextCtrl(panel, wxID_ANY, "1");
		hbox3->Add(paramLabel, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 5);
		hbox3->Add(kernelCtrl, 1, wxEXPAND);
		vbox->Add(hbox3, 0, wxEXPAND | wxALL, 10);

		hbox3 = new wxBoxSizer(wxHORIZONTAL);
		paramLabel = new wxStaticText(panel, wxID_ANY, "Direction (0 vert, 1 horizontal) :");
		choiceDirectionCtrl = new wxTextCtrl(panel, wxID_ANY, "0");
		hbox3->Add(paramLabel, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 5);
		hbox3->Add(choiceDirectionCtrl, 1, wxEXPAND);
		vbox->Add(hbox3, 0, wxEXPAND | wxALL, 10);

 		opencvCheckbox = new wxCheckBox(panel, wxID_ANY, "Utiliser fonction de OpenCV");
		opencvCheckbox->SetValue(false);
		vbox->Add(opencvCheckbox, 0, wxALL, 10);

*/
