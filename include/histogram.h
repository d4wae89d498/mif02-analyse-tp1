#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <opencv2/opencv.hpp>
#include <unordered_map>
#include <iostream>

using histogram = std::array<unsigned long long, 256>;

// eveything about histogram transformations ....

/*
 *		Arondi a l'entier au plus proche
 */
int Round(float value);


/*
 *	Compte le nombre de pixel par couleur (entre 0 et 255) pour le channel demande
 */
histogram calcHist(const cv::Mat& img, int channel);


/*
 *  Met l'histogram a l'echelle  demandee
 */
void normalizeHist(histogram &hist, int maxVal = 255);


/*
 *  Calcule l'histogramme d'une image en utilisant des tableaux histogram pour chaque canal (BGR),
 *	puis le normalise avant de le retourner sous forme d'image ocv avec des labels d'échelle pour les axes.
 */
cv::Mat generateHist(const cv::Mat& img);


/*
 *  Ajustement du contraste
 */
cv::Mat equalizeHist(const cv::Mat& img, double contrast_factor = 1.0);


#endif
