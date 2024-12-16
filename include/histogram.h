#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <opencv2/opencv.hpp>
#include <unordered_map>
#include <iostream>
#ifndef ull
# define ull unsigned long long
#endif
// eveything about histogram transformations ....

/*
 *		Arondi a l'entier au plus proche
 */
int Round(float value);


/*
 *	Compte le nombre de pixel par couleur (entre 0 et 255) pour le channel demande
 */
std::array<ull, 256> CalcHist(const cv::Mat& img, int channel);


/*
 *  Met l'histogram a l'echelle  demandee
 */
void NormalizeHist(std::array<ull, 256> &hist, int maxVal = 255);


/*
 *  Calcule l'histogramme d'une image en utilisant des tableaux std::array<ull, 256> pour chaque canal (BGR),
 *	puis le normalise avant de le retourner sous forme d'image ocv avec des labels d'échelle pour les axes.
 */
cv::Mat generateHistogram(const cv::Mat& img);


#endif
