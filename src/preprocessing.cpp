//
// Created by Piotr Sykulski on 13/01/2018.
//

#include "../headers/preprocessing.hpp"
using namespace cv;

cv::Mat preprocessing(cv::Mat image){
    cv::Mat grayscale = conv2Grey(image);
    matrix<float,3,3> gaussianKernel = gaussian_kernel(1.0f);
    cv::Mat blurred = convolutionSingleChannel<float,3>(grayscale,gaussianKernel);
    cv::Mat sobelX = convolutionSingleChannel<int,3>(blurred, sobelx);
    cv::Mat sobelY = convolutionSingleChannel<int,3>(blurred, sobely);
    cv::Mat magn = magnitude(sobelX,sobelY);
    threshold(magn, 127);

    return magn;
}