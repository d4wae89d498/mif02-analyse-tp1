#include "base.h"
#include "filter.h"
#include <iostream>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

// to store teacher test images 
const std::vector<std::pair<std::string, std::string>> imageStoring = {
    {"../data/data1/Autre.png", "Autre"},
    {"../data/data1/lena.png", "Lena"},
    {"../data/data1/lena2.png", "Lena 2"},
    {"../data/data1/lena3.png", "Lena 3"}
};


// dupliquer les pixels sur la bordur d une image 
// mettre à 0 
// mettre à 255 
// ne pas prendre en compte 


void testFiltreMedian(cv::Mat & img) {
    cv::Mat imgMedianFilter = medianFilterColor(img); 
    showImage(imgMedianFilter, "with Median Filter"); 
}

void testAveragingKernel(cv::Mat & img) {
    int choice = 0; 
    std::cout<<"Wich size for ur avecraging kernel do u yant"<<std::endl; 
    std::cin>>choice; 
    cv::Mat kernel = createAveragingKernel(choice); 
    cv::Mat imResult = applyConvolution(img,kernel); 
    showImage(imResult, "with Averaging Kernel"); 
}

void testGaussianKernel(cv::Mat & img) {
    int choiceSizeKernel = 0; 
    double choiceAlpha = 0; 
    std::cout<<"Wich size for ur gaussian kernel do u yant"<<std::endl; 
    std::cin>>choiceSizeKernel; 
    std::cout<<"Wich alpha do u yant"<<std::endl; 
    std::cin>>choiceAlpha; 
    cv::Mat imgResult = applyConvolution(img,createGaussianKernel(choiceSizeKernel,choiceAlpha)); 
    showImage(imgResult, "with Gaussian Filter"); 
    
    // with opencv
    cv::Mat imgConvOpenCV;
    cv::GaussianBlur(img, imgConvOpenCV, cv::Size(choiceSizeKernel, choiceSizeKernel), choiceAlpha);  
    showImage(imgConvOpenCV, "Compate with Gauss Convolution (OpenCV)");
}

void testLaplacienKernel(cv::Mat & img) {
    int choiceSizeKernel = 0; 
    std::cout<<"Wich size for ur laplacien kernel do u yant"<<std::endl; 
    std::cin>>choiceSizeKernel;    
    cv::Mat imgResult = applyConvolution(img,createLaplacianKernel(choiceSizeKernel)); 
    showImage(imgResult, "with Laplacien Filter"); 
    
    //with opencv
    cv::Mat laplacianImage;
    cv::Laplacian(img, laplacianImage, CV_32F, choiceSizeKernel);
    cv::Mat laplacianImageDisplay;
    laplacianImage.convertTo(laplacianImageDisplay, CV_8U);
    showImage(laplacianImageDisplay, "Laplacian Filter");

    // cv::Mat laplacianImage;
    // cv::Laplacian(img, laplacianImage, CV_32F, choiceSizeKernel);
    // cv::Mat normalizedLaplacian;
    // cv::normalize(laplacianImage, normalizedLaplacian, 0, 255, cv::NORM_MINMAX);
    // cv::Mat laplacianImageDisplay;
    // normalizedLaplacian.convertTo(laplacianImageDisplay, CV_8U);
    // showImage(laplacianImageDisplay, "Laplacian Filter");
}


void testSobelKernel(cv::Mat & img) {
    int choiceSizeKernel = 0; 
    bool choiceDirection = false; // false : vertical and true : horizontal
    std::cout << "Which size for your Sobel kernel do u want (msut be odd number)?" << std::endl;
    std::cin >> choiceSizeKernel;
    std::cout << "Which Sobel direction do you want => 0 for vertical and 1 for horizontal)?" << std::endl;
    std::cin >> choiceDirection;   

    cv::Mat imgResult = applyConvolution(img, createSobelKernel(choiceSizeKernel, choiceDirection));
    showImage(imgResult, "With Sobel Filter");

    // with opencv
    cv::Mat imgConvOpenCV;
    cv::Sobel(img, imgConvOpenCV, CV_8U, choiceDirection ? 1 : 0, choiceDirection ? 0 : 1, choiceSizeKernel);
    showImage(imgConvOpenCV, "Sobel with OpenCV");
}

void testHighPassKernel(cv::Mat & img) {
    int choice = 0; 
    std::cout<<"Wich size for ur hight pass kernel do u yant"<<std::endl; 
    std::cin>>choice; 
    cv::Mat kernel = createHighPassKernel(choice); 
    cv::Mat imResult = applyConvolution(img,kernel); 
    showImage(imResult, "with Averaging Kernel"); 

    //with open cv 
    cv::Mat blurred, highPassOpenCV;
    cv::GaussianBlur(img, blurred, cv::Size(choice, choice), 0); 
    highPassOpenCV = img - blurred; 
    showImage(highPassOpenCV, "High-Pass Filter (OpenCV)");
}


int main() {
    std::cout << "11111" << std::endl; 

    std::string imagePath = "../data/data1/lena.png";
    cv::Mat img = loadImage(imagePath); 
    showImage(img, "Normal Image");

    //testFiltreMedian(img);
    //testAveragingKernel(img); 
    //testGaussianKernel(img); 
    testLaplacienKernel(img); 
    testSobelKernel(img); 
    //testHighPassKernel(img); 

    return 0;
}
