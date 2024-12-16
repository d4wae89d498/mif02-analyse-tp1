#include "histogram.h"

int Round(float value)
{
	if (value > 0) {
		return (int)(value + 0.5f);
	} else {
		return (int)(value - 0.5f);
	}
}


histogram calcHist(const cv::Mat& img, int channel)
{
	histogram output = {0};

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

void normalizeHist(histogram &hist, int maxVal)
{
	int maxHistValue = *std::max_element(hist.begin(), hist.end());
	if (maxHistValue == 0) return;
	float scale = maxVal / static_cast<float>(maxHistValue);
	for (int i = 0; i < hist.size(); i++) {
		hist[i] = Round(hist[i] * scale);
	}
}

cv::Mat generateHist(const cv::Mat& img)
{
	auto bHist = calcHist(img, 0);  // B
	auto gHist = calcHist(img, 1);  // G
	auto rHist = calcHist(img, 2);  // R

	normalizeHist(bHist);
	normalizeHist(gHist);
	normalizeHist(rHist);

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



cv::Mat equalizeHist(const cv::Mat& img, double contrast_factor)
{
	if (img.channels() != 3) {
		throw std::runtime_error("L'image doit avoir trois canals (BGR)");
	}

	cv::Mat result = img.clone();

	for (int c = 0; c < 3; c++) {
		histogram hist = calcHist(img, c);

		histogram cdf = {0};
		cdf[0] = hist[0];
		for (int i = 1; i < 256; i++) {
			cdf[i] = cdf[i - 1] + hist[i];
		}

		auto min_cdf = cdf[0];
		auto max_cdf = cdf[255];
		for (int i = 0; i < 256; i++) {
			cdf[i] = ((cdf[i] - min_cdf) * 255) / (max_cdf - min_cdf);
		}

		for (int y = 0; y < img.rows; y++) {
			for (int x = 0; x < img.cols; x++) {
				cv::Vec3b pixel = img.at<cv::Vec3b>(y, x);
				int new_value = static_cast<int>(cdf[pixel[c]] * contrast_factor);
				new_value = std::min(std::max(new_value, 0), 255);
				result.at<cv::Vec3b>(y, x)[c] = static_cast<uchar>(new_value);
			}
		}
	}

	return result;
}


cv::Mat stretchHist(const cv::Mat& img, double stretch_factor)
{
	if (img.channels() != 3) {
		throw std::runtime_error("L'image doit avoir trois canaux (BGR)");
	}
	cv::Mat result = img.clone();
	for (int c = 0; c < 3; c++) {
		histogram hist = calcHist(img, c);
		int min_val = 255, max_val = 0;
		for (int i = 0; i < 256; i++) {
			if (hist[i] > 0) {
				min_val = std::min(min_val, i);
				max_val = std::max(max_val, i);
			}
		}
		for (int y = 0; y < img.rows; y++) {
			for (int x = 0; x < img.cols; x++) {
				cv::Vec3b pixel = img.at<cv::Vec3b>(y, x);
				int new_value = static_cast<int>((pixel[c] - min_val) * 255.0 / (max_val - min_val) * stretch_factor);
				new_value = std::min(std::max(new_value, 0), 255);
				result.at<cv::Vec3b>(y, x)[c] = static_cast<uchar>(new_value);
			}
		}
	}
	return result;
}
