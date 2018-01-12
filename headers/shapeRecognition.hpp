//
// Created by Piotr Sykulski on 06/01/2018.
//

#ifndef CARBRANDDETECTOR_SHAPERECOGNITION_HPP
#define CARBRANDDETECTOR_SHAPERECOGNITION_HPP

#include <opencv2/core/mat.hpp>
#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>

double calcmpq(cv::Mat I, int p, int q);
void calculateParameters(cv::Mat I);
double calculateM1(cv::Mat I);
double calculateM7(cv::Mat I);
double getW3(cv::Mat I);

#endif //CARBRANDDETECTOR_SHAPERECOGNITION_HPP
