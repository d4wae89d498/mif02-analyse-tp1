#pragma once

struct HistogramEq : Mif02Plugin
{
	Components::TextInput			factorComponent;

	fn getName() const -> string_view override
	{
		return "Etirer Histogram";
	}

	fn setupUi(wxBoxSizer* vbox, wxPanel* panel) -> void override
	{
		factorComponent.setupUi(vbox, panel, "Facteur :", "0.2");
	}

	fn onApply(const cv::Mat& loadedImage, cv::Mat& filteredImg) -> void override
	{
		double factor = stod(factorComponent.ctrl->GetValue().ToStdString());

		new CVImageWindow(nullptr, "Histograme avant traitement", generateHist(loadedImage), 300);

		filteredImg = stretchHist(loadedImage, factor);

		new CVImageWindow(nullptr, "Histograme apres traitement", generateHist(filteredImg), -300);
	}
};

REGISTER_PLUGIN(HistogramEq);
