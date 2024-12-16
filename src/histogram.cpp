#include "histogram.h"

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
