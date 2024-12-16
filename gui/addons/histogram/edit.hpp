#pragma once

struct HistogramLUT : Mif02Plugin
{
	Components::TextInput			lowerBoundComponent;
	Components::TextInput			upperBoundComponent;


	wxButton* histButton = nullptr;

	fn getName() const -> string_view override
	{
		return "Histogram / LUT";
	}

	fn setupUi(wxBoxSizer* vbox, wxPanel* panel) -> void override
	{
		lowerBoundComponent.setupUi(vbox, panel, "Plage inférieure :", "0");
		upperBoundComponent.setupUi(vbox, panel, "Plage supérieure :", "255");


/*
		histButton = new wxButton(panel, wxID_ANY, "Afficher Histogramme");
		vbox->Add(histButton, 0, wxALL, 10);

		histButton->Bind(wxEVT_BUTTON, &HistogramLUT::onShowHistogram, this);*/
	}


	fn onApply(const cv::Mat& loadedImage, cv::Mat& filteredImg) -> void override
	{
		int lowerBound = std::stoi(lowerBoundComponent.ctrl->GetValue().ToStdString());
		int upperBound = std::stoi(upperBoundComponent.ctrl->GetValue().ToStdString());

		if (lowerBound >= 0 && upperBound <= 255 && lowerBound < upperBound)
		{
			displayHistogram(filteredImg);
		}
		else
		{
			wxMessageBox("Les valeurs de la plage sont invalides.", "Erreur", wxOK | wxICON_ERROR);
		}
	}
};

REGISTER_PLUGIN(HistogramLUT);
