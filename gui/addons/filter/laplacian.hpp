#pragma once

struct Laplacian : Mif02Plugin
{
    wxTextCtrl* kernelCtrl = nullptr;
    wxCheckBox* opencvCheckbox = nullptr;


	fn getName() const -> string_view override
	{
		return "Laplacien";
	}

	fn setupUi(wxBoxSizer* vbox, wxPanel* panel) -> void override
	{
		wxBoxSizer* hbox3 = new wxBoxSizer(wxHORIZONTAL);
		wxStaticText* paramLabel = new wxStaticText(panel, wxID_ANY, "Taille du kernel :");
		kernelCtrl = new wxTextCtrl(panel, wxID_ANY, "1");
		hbox3->Add(paramLabel, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 5);
		hbox3->Add(kernelCtrl, 1, wxEXPAND);
		vbox->Add(hbox3, 0, wxEXPAND | wxALL, 10);


 		opencvCheckbox = new wxCheckBox(panel, wxID_ANY, "Utiliser fonction de OpenCV");
		opencvCheckbox->SetValue(true);
		vbox->Add(opencvCheckbox, 0, wxALL, 10);

		return;
	}

	fn onApply(const cv::Mat& loadedImage, cv::Mat &filteredImg) -> void override
	{

		uint kernel = parse_odd_uint(kernelCtrl->GetValue().ToStdString());

		if (opencvCheckbox->IsChecked()) {
			cv::Laplacian(loadedImage, filteredImg, CV_32F,  kernel);
		} else {
			filteredImg = applyConvolution(loadedImage,createLaplacianKernel(kernel));
		}
	}
};
REGISTER_PLUGIN(Laplacian);
