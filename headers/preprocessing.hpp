//
// Created by Piotr Sykulski on 13/01/2018.
//

#ifndef CARBRANDDETECTORTEST_PREPROCESSING_HPP
#define CARBRANDDETECTORTEST_PREPROCESSING_HPP

#include <vector>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "utils.hpp"
#include "shapeRecognition.hpp"
#include "kernels.hpp"

cv::Mat preprocessing(cv::Mat image);

#endif //CARBRANDDETECTORTEST_PREPROCESSING_HPP
