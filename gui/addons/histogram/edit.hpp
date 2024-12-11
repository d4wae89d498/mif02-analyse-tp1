#pragma once
#include <opencv2/core.hpp>

struct HistogramLUT : Mif02Plugin
{
    wxTextCtrl* lowerBoundCtrl = nullptr;
    wxTextCtrl* upperBoundCtrl = nullptr;
    wxButton* histButton = nullptr;

	void displayHistogram(const cv::Mat& img)
	{
		// Calculate histogram
		std::vector<cv::Mat> bgrPlanes;
		cv::split(img, bgrPlanes);

		int histSize = 256; // Number of bins for histogram
		float range[] = {0, 256}; // Range from 0 to 255
		const float* histRange = {range};

		cv::Mat bHist, gHist, rHist;
		cv::calcHist(&bgrPlanes[0], 1, 0, cv::Mat(), bHist, 1, &histSize, &histRange);
		cv::calcHist(&bgrPlanes[1], 1, 0, cv::Mat(), gHist, 1, &histSize, &histRange);
		cv::calcHist(&bgrPlanes[2], 1, 0, cv::Mat(), rHist, 1, &histSize, &histRange);

		// Normalize histograms
		cv::normalize(bHist, bHist, 0, 255, cv::NORM_MINMAX);
		cv::normalize(gHist, gHist, 0, 255, cv::NORM_MINMAX);
		cv::normalize(rHist, rHist, 0, 255, cv::NORM_MINMAX);

		// Create a black image to draw the histogram
		int histHeight = 256;
		int histWidth = 512;
		cv::Mat histImage(histHeight, histWidth, CV_8UC3, cv::Scalar(0, 0, 0));

		int binWidth = cvRound((double)histWidth / histSize); // Set width of each bin

		// Draw the histograms
		for (int i = 1; i < histSize; i++)
		{
			// Blue
			cv::line(histImage,
					cv::Point((i - 1) * binWidth, histHeight - cvRound(bHist.at<float>(i - 1))),
					cv::Point(i * binWidth, histHeight - cvRound(bHist.at<float>(i))),
					cv::Scalar(255, 0, 0), 2, 8, 0);
			// Green
			cv::line(histImage,
					cv::Point((i - 1) * binWidth, histHeight - cvRound(gHist.at<float>(i - 1))),
					cv::Point(i * binWidth, histHeight - cvRound(gHist.at<float>(i))),
					cv::Scalar(0, 255, 0), 2, 8, 0);
			// Red
			cv::line(histImage,
					cv::Point((i - 1) * binWidth, histHeight - cvRound(rHist.at<float>(i - 1))),
					cv::Point(i * binWidth, histHeight - cvRound(rHist.at<float>(i))),
					cv::Scalar(0, 0, 255), 2, 8, 0);
		}

		// Add labels on the X-axis (color intensities from 0 to 255)
		for (int i = 0; i < histSize; i += 32) // Add labels for every 32 steps
		{
			std::string label = std::to_string(i);
			cv::putText(histImage, label, cv::Point(i * binWidth, histHeight - 10),
						cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(255, 255, 255), 1, 8, false);
		}

		// Add labels on the Y-axis (frequency count values)
		int maxValue = 1024;// std::max({cvRound(cv::minMaxLoc(bHist).maxVal), cvRound(cv::minMaxLoc(gHist).maxVal), cvRound(cv::minMaxLoc(rHist).maxVal)});
		int step = maxValue / 5;  // Divide the max value by 5 to create intervals for labeling
		for (int i = 0; i <= 5; i++)
		{
			std::string label = std::to_string(i * step);
			cv::putText(histImage, label, cv::Point(5, histHeight - (i * histHeight / 5)),
						cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(255, 255, 255), 1, 8, false);
		}

		// Display the histogram with color intensities and frequency labels
		cv::imshow("Histogram", histImage);
		cv::waitKey(0);  // Wait for a key press before closing
	}


    fn getName() const -> string_view override
    {
        return "Histogram / LUT";
    }

    fn setupUi(wxBoxSizer* vbox, wxPanel* panel) -> void override
    {
        wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);
        wxStaticText* paramLabel = new wxStaticText(panel, wxID_ANY, "Plage inférieure :");
        lowerBoundCtrl = new wxTextCtrl(panel, wxID_ANY, "0");
        hbox->Add(paramLabel, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 5);
        hbox->Add(lowerBoundCtrl, 1, wxEXPAND);
        vbox->Add(hbox, 0, wxEXPAND | wxALL, 10);

        hbox = new wxBoxSizer(wxHORIZONTAL);
        paramLabel = new wxStaticText(panel, wxID_ANY, "Plage supérieure :");
        upperBoundCtrl = new wxTextCtrl(panel, wxID_ANY, "255");
        hbox->Add(paramLabel, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 5);
        hbox->Add(upperBoundCtrl, 1, wxEXPAND);
        vbox->Add(hbox, 0, wxEXPAND | wxALL, 10);

/*
        histButton = new wxButton(panel, wxID_ANY, "Afficher Histogramme");
        vbox->Add(histButton, 0, wxALL, 10);

        histButton->Bind(wxEVT_BUTTON, &HistogramLUT::onShowHistogram, this);*/
    }


    fn onApply(const cv::Mat& loadedImage, cv::Mat& filteredImg) -> void override
    {
        int lowerBound = std::stoi(lowerBoundCtrl->GetValue().ToStdString());
        int upperBound = std::stoi(upperBoundCtrl->GetValue().ToStdString());

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
