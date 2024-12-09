#include "base.h"
#include "filter.h"
#include <iostream>
#include <string>
#include <vector>

// to store teacher test images 
const std::vector<std::pair<std::string, std::string>> imageStoring = {
    {"../data/data1/Autre.png", "Autre"},
    {"../data/data1/lena.png", "Lena"},
    {"../data/data1/lena2.png", "Lena 2"},
    {"../data/data1/lena3.png", "Lena 3"}
};

// to display the main menu
void displayMainMenu() {
    std::cout << "\nMain menu:\n";
    std::cout << "1 - Choose an image\n";
    std::cout << "2 - Quit\n";
    std::cout << "Selection: ";
}

// to display the image selection menu
void displayImageMenu() {
    std::cout << "\nSelect an image:\n";
    for (unsigned int i = 0; i < imageStoring.size(); ++i) {
        std::cout << i + 1 << " - " << imageStoring[i].second << "\n"; 
    }
    std::cout << imageStoring.size() + 1 << " - Otherwise enter your file path thaht u want.\n";
    std::cout << "Wich one do u prefer ? ";
}

// to display the processing menu
void displayProcessingMenu() {
    std::cout << "\nProcessing image menu:\n";
    std::cout << "1 - Apply median filter\n";
    std::cout << "2 - Apply convolution filter\n";
    std::cout << "3 - Apply histogram\n";
    std::cout << "4 - Back to main menu\n";
    std::cout << "Selection: ";
}

int main() {
    cv::Mat image;
    int mainChoice = 0;

    while (true) {
        displayMainMenu();
        std::cin >> mainChoice;

        switch (mainChoice) {
            case 1: { // choose an image
                int imageChoice = 0;
                displayImageMenu(); 
                std::cin >> imageChoice;

                if (imageChoice >= 1 && imageChoice <= (int)imageStoring.size()) { // load a predefined image
                    image = loadImage(imageStoring[imageChoice - 1].first); // the image path 
                    if (!image.empty()) {
                        showImage(image, imageStoring[imageChoice - 1].second); 
                    } else {
                        continue;  // return to the  menu 
                    }
                } else if (imageChoice == (int)imageStoring.size() + 1) { // load a custom image
                    std::string filePath;
                    std::cout << "Enter ur file path: ";
                    std::cin >> filePath;
                    image = loadImage(filePath);
                    if (!image.empty()) {
                        showImage(image, "Ur image");
                    } else {
                        continue;  
                    }
                } else {
                    std::cerr << "Invalid selection.\n";
                    continue;  
                }
                break;
            }


            case 2: { // quit the app
                std::cout << "I hope you enjoying it.\n";
                return 0;
            }

            default:
                std::cerr << "Invalid choice.\n";
        }
 
        if (!image.empty()) { // Part two on image processing
            int processChoice = 0;
            while (true) {
                displayProcessingMenu();
                std::cin >> processChoice;

                switch (processChoice) {
                    case 1: { // for the part about median filter 
                        cv::Mat filteredImage = applyFilterMedian(image);
                        showImage(filteredImage, "Median filtered image");
                        break;
                    }

                    case 2: { // for the part about convolution
                        cv::Mat averagingKernel = createAveragingKernel(3);
                        cv::Mat smoothedImage = applyConvolution(image, averagingKernel);
                        showImage(smoothedImage, "Convolution Filtered Image");
                        break;
                    }

                    case 3: { // for the part about histogram
                        std::cout << "Not implemented yet.\n";
                        break;
                    }

                    case 4: { // return to the main menu
                        std::cout << "Returning to main menu.\n";
                        image.release(); // we nedd to delete the image here to choose another one in the main menu
                        break; 
                    }

                    default:
                        std::cerr << "Invalid selection.\n";
                }

                if (processChoice == 4) break; // exit from the processing menu and return to the main menu
            }
        }
    }

    return 0;
}
