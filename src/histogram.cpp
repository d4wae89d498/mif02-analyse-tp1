#include "histogram.h"

int Round(float value)
{
	if (value > 0) {
		return (int)(value + 0.5f);
	} else {
		return (int)(value - 0.5f);
	}
}


std::array<ull, 256> CalcHist(const cv::Mat& img, int channel)
{
	std::array<ull, 256> output = {0};

	if (img.channels() != 3) {
		throw std::runtime_error("L'image doit avoir trois canneaux (BGR)");
	}

	for (int y = 0; y < img.rows; y++) {
		for (int x = 0; x < img.cols; x++) {
			cv::Vec3b pixel = img.at<cv::Vec3b>(y, x);
			int value = pixel[channel];
			output[value]++;
		}
	}

	return output;
}

void NormalizeHist(std::array<ull, 256> &hist, int maxVal)
{
	int maxHistValue = *std::max_element(hist.begin(), hist.end());
	if (maxHistValue == 0) return;
	float scale = maxVal / static_cast<float>(maxHistValue);
	for (int i = 0; i < hist.size(); i++) {
		hist[i] = Round(hist[i] * scale);
	}
}

cv::Mat generateHistogram(const cv::Mat& img)
{
	auto bHist = CalcHist(img, 0);  // B
	auto gHist = CalcHist(img, 1);  // G
	auto rHist = CalcHist(img, 2);  // R

	NormalizeHist(bHist);
	NormalizeHist(gHist);
	NormalizeHist(rHist);

	int histHeight = 256;
	int histWidth = 512;
	cv::Mat histImage(histHeight, histWidth, CV_8UC3, cv::Scalar(0, 0, 0));

	int histSize = 256;
	int binWidth = histWidth / histSize;

	for (int i = 1; i < histSize; i++)
	{
		cv::line(histImage,
				 cv::Point((i - 1) * binWidth, histHeight - Round(bHist[i - 1])),
				 cv::Point(i * binWidth, histHeight - Round(bHist[i])),
				 cv::Scalar(255, 0, 0), 2, 8, 0);

		cv::line(histImage,
				 cv::Point((i - 1) * binWidth, histHeight - Round(gHist[i - 1])),
				 cv::Point(i * binWidth, histHeight - Round(gHist[i])),
				 cv::Scalar(0, 255, 0), 2, 8, 0);

		cv::line(histImage,
				 cv::Point((i - 1) * binWidth, histHeight - Round(rHist[i - 1])),
				 cv::Point(i * binWidth, histHeight - Round(rHist[i])),
				 cv::Scalar(0, 0, 255), 2, 8, 0);
	}

	for (int i = 0; i < histSize; i += 32)
	{
		std::string label = std::to_string(i);
		cv::putText(histImage, label, cv::Point(i * binWidth, histHeight - 10),
					cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(255, 255, 255), 1, 8, false);
	}

	int maxValue = 1024;
	int step = maxValue / 5;
	for (int i = 0; i <= 5; i++)
	{
		std::string label = std::to_string(i * step);
		cv::putText(histImage, label, cv::Point(5, histHeight - (i * histHeight / 5)),
					cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(255, 255, 255), 1, 8, false);
	}

	return histImage;
}
