#pragma once

struct HistogramEq : Mif02Plugin
{
	Components::TextInput			factorComponent;

	fn getName() const -> string_view override
	{
		return "Egaliser Histogram";
	}

	fn setupUi(wxBoxSizer* vbox, wxPanel* panel) -> void override
	{
		factorComponent.setupUi(vbox, panel, "Facteur :", "0.2");
	}


	fn onApply(const cv::Mat& loadedImage, cv::Mat& filteredImg) -> void override
	{
		double factor = stod(factorComponent.ctrl->GetValue().ToStdString());

		filteredImg = equalizeHist(loadedImage, factor);
	}
};

REGISTER_PLUGIN(HistogramEq);
