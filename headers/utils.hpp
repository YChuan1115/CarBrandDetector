//
// Created by Piotr Sykulski on 05/01/2018.
//

#ifndef POBR_PROJEKT_UTILS_HPP
#define POBR_PROJEKT_UTILS_HPP

#define _USE_MATH_DEFINES
#include <cmath>
#include <opencv2/core/core.hpp>

#include "../headers/kernels.hpp"


cv::Mat magnitude(const cv::Mat image_x, const cv::Mat image_y);

matrix<float, 3, 3> gaussian_kernel(float sigma);

cv::Mat conv2Grey(cv::Mat I);

template < class T, size_t filterDimension>
cv::Mat convolutionMultiChannel(cv::Mat I, matrix<T,filterDimension,filterDimension>(filterArray));

template <class T,size_t filterDimension>
cv::Mat convolutionSingleChannel(const cv::Mat I, matrix<T,filterDimension,filterDimension>(filterArray));


#endif //POBR_PROJEKT_UTILS_HPP
