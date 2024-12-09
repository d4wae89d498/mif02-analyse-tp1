#include "filter.h"


///////////////////////////////////////// MEDIAN FILTER /////////////////////////////////////////


cv::Mat medianFilterGray(const cv::Mat& image) {

    if (image.channels() != 1) {
        std::cerr << "Only gray images can be traited." << std::endl;
        return cv::Mat();
    }

    cv::Mat result = image.clone();

    for (int i = 1; i < image.rows - 1; ++i) {
        for (int j = 1; j < image.cols - 1; ++j) {
            std::vector<uchar> pixels; // for storing the values of the 8 neighbours 
            for (int y = -1; y <= 1; ++y) {
                for (int x = -1; x <= 1; ++x) {
                    if (y == 0 && x == 0) continue; // pix we want to set the value 
                    pixels.push_back(image.at<uchar>(i + y, j + x));
                }
            }

            std::sort(pixels.begin(), pixels.end()); // sort the values of pixels 
            uchar medianValue = pixels[pixels.size() / 2]; // caculate the median 
            result.at<uchar>(i, j) = medianValue;
        }
    }

    return result;
}


cv::Mat medianFilterColor(const cv::Mat& image) {

    if (image.channels() != 3) {
        std::cerr << "Only image in color can be traited!" << std::endl;
        return cv::Mat();
    }

    std::vector<cv::Mat> channels;
    cv::split(image, channels); // get the three channels 

    for (int i = 0; i < 3; ++i) {
        channels[i] = medianFilterGray(channels[i]); 
    }

    cv::Mat filteredImage;
    cv::merge(channels, filteredImage); 
    return filteredImage;
}


cv::Mat applyFilterMedian(const cv::Mat &img) {
    return img.channels()==1 ? medianFilterGray(img) : medianFilterColor(img) ;
}



///////////////////////////////////////// CONVOLUTION FILTER /////////////////////////////////////////


cv::Mat genericConvolutionGray(const cv::Mat &image, const cv::Mat &kernel) {
    if (image.channels() != 1) { // function only for gray images
        std::cerr << "Only grayscale images are supported in genericConvolutionGray." << std::endl;
        return cv::Mat();
    }

    if (kernel.rows % 2 == 0 || kernel.cols % 2 == 0) { // convolution only works if the kernel is odd
        std::cerr << "The size of the kernel must be odd." << std::endl;
        return cv::Mat();
    }

    cv::Mat resultConvolution(image.rows, image.cols, image.type()); // output image

    // we need that to look over the kernel symmetrically around the central pixel
    const int dx = kernel.cols / 2;  // half-width of the kernel 
    const int dy = kernel.rows / 2; // half-height of the kernel

    
    for (int i = dy; i < image.rows - dy; i++) {
        for (int j = dx; j < image.cols - dx; j++) {
            float sum = 0.0f;

            // we apply the convlution for each pixel 
            for (int k = 0; k < kernel.rows; k++) {
                for (int l = 0; l < kernel.cols; l++) {
                    int x = j - dx + l;
                    int y = i - dy + k;
                    sum += image.at<uchar>(y, x) * kernel.at<float>(k, l);
                }
            }

            // we update the value og the pixel
            resultConvolution.at<uchar>(i, j) = cv::saturate_cast<uchar>(sum); // we ensure that the interval is respected in [0,255]
        }
    }

    return resultConvolution;
}


cv::Mat genericConvolutionColor(const cv::Mat& image, const cv::Mat& kernel) {
    if (image.channels() != 3) { // function only for color images
        std::cerr << "Only color images are supported for in genericConvolutionColor" << std::endl;
        return cv::Mat();
    }

    std::vector<cv::Mat> channels; // we get the three channels of the color image 
    cv::split(image, channels); // extraction of the channels RGB 

    for (unsigned int i = 0; i < 3; ++i) { // we calculate the convolution of the three channels separately
        channels[i] = genericConvolutionGray(channels[i], kernel);
    }

    cv::Mat filteredImage; 
    cv::merge(channels, filteredImage); // we merge the channels into a single image
    return filteredImage;
}

cv::Mat applyConvolution(const cv::Mat & img, const cv::Mat& kernel) {
    return img.channels()==1 ? genericConvolutionGray(img,kernel) : genericConvolutionColor(img,kernel) ;
}


cv::Mat createAveragingKernel(int size) {
    return cv::Mat::ones(size, size, CV_32F) / (float)(size * size);
}
