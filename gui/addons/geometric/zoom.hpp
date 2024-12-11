#pragma once

struct Zoom : Mif02Plugin
{
    wxTextCtrl*	scaleCtrl = nullptr;
    wxCheckBox* opencvCheckbox = nullptr;


	fn getName() const -> string_view override
	{
		return "Zoom";
	}

	fn setupUi(wxBoxSizer* vbox, wxPanel* panel) -> void override
	{
		wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);
        wxStaticText* paramLabel = new wxStaticText(panel, wxID_ANY, "Facteur d'échelle :");
        scaleCtrl = new wxTextCtrl(panel, wxID_ANY, "1.0");
        hbox->Add(paramLabel, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 5);
        hbox->Add(scaleCtrl, 1, wxEXPAND);
        vbox->Add(hbox, 0, wxEXPAND | wxALL, 10);

 		opencvCheckbox = new wxCheckBox(panel, wxID_ANY, "Utiliser fonction de OpenCV");
		opencvCheckbox->SetValue(true);
		vbox->Add(opencvCheckbox, 0, wxALL, 10);

		return;
	}

	fn onApply(const cv::Mat& loadedImage, cv::Mat &filteredImg) -> void override
	{
        double scale = stod(scaleCtrl->GetValue().ToStdString());

        if (scale <= 0.0) {
            throw invalid_argument("Le facteur d'échelle doit être strictement positif.");
        }

        cv::Mat resized;

        if (opencvCheckbox->IsChecked()) {
            cv::resize(loadedImage, resized, cv::Size(), scale, scale, cv::INTER_LINEAR);
        } else {
            resized = applyResize(loadedImage, scale);
        }

 		int offsetX = (resized.cols - loadedImage.cols) / 2;
        int offsetY = (resized.rows - loadedImage.rows) / 2;

        cv::Rect roi(
            max(0, offsetX),
            max(0, offsetY),
            min(loadedImage.cols, resized.cols),
            min(loadedImage.rows, resized.rows)
        );

        filteredImg = resized(roi);
	}

};
REGISTER_PLUGIN(Zoom);
