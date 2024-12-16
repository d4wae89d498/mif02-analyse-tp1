#pragma once

struct Gaussian : Mif02Plugin
{
    wxTextCtrl* kernelCtrl = nullptr;
    wxTextCtrl* alphaCtrl = nullptr;
    wxCheckBox* opencvCheckbox = nullptr;


	fn getName() const -> string_view override
	{
		return "Flou gaussien";
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
		paramLabel = new wxStaticText(panel, wxID_ANY, "Moyenne :");
		alphaCtrl = new wxTextCtrl(panel, wxID_ANY, "0.5");
		hbox3->Add(paramLabel, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 5);
		hbox3->Add(alphaCtrl, 1, wxEXPAND);
		vbox->Add(hbox3, 0, wxEXPAND | wxALL, 10);

 		opencvCheckbox = new wxCheckBox(panel, wxID_ANY, "Utiliser fonction de OpenCV");
		opencvCheckbox->SetValue(false);
		vbox->Add(opencvCheckbox, 0, wxALL, 10);

		return;
	}

	fn onApply(const cv::Mat& loadedImage, cv::Mat &filteredImg) -> void override
	{

		uint kernel = parse_odd_uint(kernelCtrl->GetValue().ToStdString());
		double alpha = stod(alphaCtrl->GetValue().ToStdString());


		if (opencvCheckbox->IsChecked()) {
			filteredImg = testGaussianKernelWithOpenCv(loadedImage,kernel, alpha); 
		} else {
			filteredImg = applyConvolution(loadedImage,createGaussianKernel(kernel,alpha));
		}
	}
};
REGISTER_PLUGIN(Gaussian);
