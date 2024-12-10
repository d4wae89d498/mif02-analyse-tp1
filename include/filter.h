#ifndef FILTER_H
#define FILTER_H

#include <opencv2/opencv.hpp>
#include <iostream>

// file for everything that is filtering



// return an image from another where a median filter has been applied
cv::Mat medianFilterGray(const cv::Mat & img); 
cv::Mat medianFilterColor(const cv::Mat& img); 
cv::Mat applyFilterMedian(const cv::Mat & img); 

// return an image from another where a convolotion has been applied
cv::Mat genericConvolutionGray(const cv::Mat& image, const cv::Mat& kernel);
cv::Mat genericConvolutionColor(const cv::Mat& image, const cv::Mat& kernel);
cv::Mat applyConvolution(const cv::Mat & img, const cv::Mat& kernel); 
cv::Mat createAveragingKernel(int size); 
cv::Mat createGaussianKernel(int size, float sigma); 
cv::Mat createLaplacianKernel(int size); 
cv::Mat createSobelKernel(int size, bool horizontal); 
cv::Mat createHighPassKernel(int size); 


#endif