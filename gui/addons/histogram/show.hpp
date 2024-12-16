#pragma once

struct HistogramShow : Mif02Plugin
{
	fn getName() const -> string_view override
	{
		return "Voir Histogram";
	}

	fn setupUi(wxBoxSizer* vbox, wxPanel* panel) -> void override
	{
	}


	fn onApply(const cv::Mat& loadedImage, cv::Mat& filteredImg) -> void override
	{
		new CVImageWindow(nullptr, "Histograme", generateHist(filteredImg));
	}
};

REGISTER_PLUGIN(HistogramShow);
