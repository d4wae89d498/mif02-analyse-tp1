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
                    if (y == 0 && x == 0) continue; 
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


// creation of the averaging kernel 
cv::Mat createAveragingKernel(int size) {
    return cv::Mat::ones(size, size, CV_32F) / (float)(size * size);
}

// creation of the gauss kernel   => size of the kernel and sigma for "l ecart type"
cv::Mat createGaussianKernel(int size, float sigma) {
    int half_size = size / 2;
    cv::Mat kernel(size, size, CV_32F);

    float sum = 0.0f;

    for (int i = -half_size; i <= half_size; ++i) {
        for (int j = -half_size; j <= half_size; ++j) {
            float value = exp(-(i*i + j*j) / (2 * sigma * sigma)); // for each pixel we calculate the value of the gaussienne
            kernel.at<float>(i + half_size, j + half_size) = value;

            sum += value;
        }
    }
    kernel /= sum;
    return kernel;
}


cv::Mat createLaplacianKernel(int size) { // problem here 
    int half_size = size / 2;
    //cv::Mat kernel(size, size, CV_32F, cv::Scalar(0.0f)); 
    cv::Mat kernel(size, size, CV_32F, cv::Scalar(0));

    for (int i = -half_size; i <= half_size; ++i) {
        for (int j = -half_size; j <= half_size; ++j) {
            if (i == 0 && j == 0) { // center of the kernel 
                kernel.at<float>(i + half_size, j + half_size) = -4.0f;
            }
            else if (i == 0 || j == 0) { // center neighbours
                kernel.at<float>(i + half_size, j + half_size) = 1.0f;
            }
            else { // corner
                kernel.at<float>(i + half_size, j + half_size) = 0.0f;
            }
        }
    }

    return kernel;
}


cv::Mat createSobelKernel(int size, bool horizontal) {
    int halfSize = size / 2;
    cv::Mat kernel(size, size, CV_32F, cv::Scalar(0));

    for (int i = -halfSize; i <= halfSize; ++i) {
        for (int j = -halfSize; j <= halfSize; ++j) {
            if (horizontal) {
                kernel.at<float>(i + halfSize, j + halfSize) = j;
            } else {
                kernel.at<float>(i + halfSize, j + halfSize) = i;
            }
        }
    }

    return kernel;
}

cv::Mat createHighPassKernel(int size) {
    if (size % 2 == 0 || size < 3) {
        std::cerr << "Kernel size must be an odd number >= 3!" << std::endl;
        return cv::Mat();
    }

    cv::Mat kernel(size, size, CV_32F, cv::Scalar(-1));
    int center = size / 2;
    kernel.at<float>(center, center) = size * size - 1; 

    return kernel;
}



