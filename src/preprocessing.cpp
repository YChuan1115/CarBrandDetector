//
// Created by Piotr Sykulski on 13/01/2018.
//

#include "../headers/preprocessing.hpp"
using namespace cv;

cv::Mat preprocessing(cv::Mat image){
    cv::Mat grayscale = conv2Grey(image);
    //cv::Mat leveled = levels(grayscale);
    matrix<float,3,3> gaussianKernel = gaussian_kernel(1.0f);
    cv::Mat blurred = convolutionSingleChannel<float,3>(grayscale,gaussianKernel);
    cv::Mat sobelX = convolutionSingleChannel<int,3>(blurred, sobelx);
    cv::Mat sobelY = convolutionSingleChannel<int,3>(blurred, sobely);
    cv::Mat magn = magnitude(sobelX,sobelY);
    cv::Mat binary;
    threshold(magn, binary, 127,255, THRESH_BINARY);
    int dilation_type = MORPH_RECT;
    int dilation_size = 1;
    Mat element = getStructuringElement( dilation_type,
                                         Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                                         Point( dilation_size, dilation_size ) );
    /// Apply the dilation operation
    //   dilate( binary, binary, element );
    //   dilate( binary, binary, element );
    //  dilate( binary, binary, element );
    //   imshow("afterDilation",binary);
    //  erode(binary,binary,element);
    //   imshow("afterClosing",binary);
//      imshow("leveled", leveled);
    // morphologyEx(binary, binary, MORPH_CLOSE);
//    cv::imshow("original", image);
//    cv::imshow("grayscale", grayscale);
//    cv::imshow("blurred", blurred);
//    cv::imshow("sobelX", sobelX);
//    cv::imshow("sobelY",sobelY);
//    cv::imshow("magn",magn);
//    cv::waitKey(-1);
    return binary;
}