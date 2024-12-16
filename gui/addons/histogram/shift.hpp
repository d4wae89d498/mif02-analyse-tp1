#pragma once

struct HistogramShift : Mif02Plugin
{
	Components::IntegerInput			redComponent;
	Components::IntegerInput			greenComponent;
	Components::IntegerInput			blueComponent;


	fn getName() const -> string_view override
	{
		return "Decaler Histogram";
	}

	fn setupUi(wxBoxSizer* vbox, wxPanel* panel) -> void override
	{
		redComponent.setupUi(vbox, panel, "Décalage du rouge:", 0, -255, 255);
		greenComponent.setupUi(vbox, panel, "Décalage du vert:", 0, -255, 255);
		blueComponent.setupUi(vbox, panel, "Décalage du bleu:", 0, -255, 255);
	}

	fn onApply(const cv::Mat& loadedImage, cv::Mat& filteredImg) -> void override
	{

		new CVImageWindow(nullptr, "Histograme avant traitement", generateHist(loadedImage));


		filteredImg = shiftColors(loadedImage, redComponent.getValue(), greenComponent.getValue(), blueComponent.getValue());


		new CVImageWindow(nullptr, "Histograme apres traitement", generateHist(filteredImg));

	}
};

REGISTER_PLUGIN(HistogramShift);
