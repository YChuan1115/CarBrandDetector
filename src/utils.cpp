//
// Created by Piotr Sykulski on 06/01/2018.
//

#include "../headers/utils.hpp"




void threshold(cv::Mat &image, unsigned int threshold){
    for(int x=0; x < image.rows; ++x){
        for(int y=0; y<image.cols; ++y){
            if( image.at<uchar>(x,y) >= threshold){
                image.at<uchar>(x,y) = 255;
            }else{
                image.at<uchar>(x,y) = 0;
            }
        }
    }
}

cv::Mat magnitude(const cv::Mat image_x, const cv::Mat image_y){
    cv::Mat magnitude = cv::Mat(image_x.rows,image_x.cols, CV_8UC1);
    for(int x=0; x < image_x.rows; ++x){
        for(int y=0; y<image_x.cols; ++y){
            double xsquared = (double)image_x.at<uchar>(x,y)*(double)image_x.at<uchar>(x,y);
            double ysquared = (double)image_y.at<uchar>(x,y)*(double)image_y.at<uchar>(x,y);
            double magn = sqrt(xsquared+ysquared);
            uchar u_magn;
            u_magn = (uchar) magn;
            if(magn>255)
                u_magn = 255;
            magnitude.at<uchar>(x,y) = u_magn;
        }
    }
    return magnitude;
}

matrix<float, 3, 3> gaussian_kernel(float sigma) {
    matrix<float, 3, 3> gauss;
    float sum = 0, s = 2 * sigma * sigma;

    for (int x = -1; x <= 1; x++)
        for (int y = -1; y <= 1; y++)
            sum += (gauss[x + 1][y + 1] = exp(-(x * x + y * y) / s) / s / M_PI);

    for (auto& row : gauss)
        for (auto& x : row)
            x /= sum;

    return gauss;
}

cv::Mat conv2Grey(cv::Mat I){
    if (I.channels() != 3) return I;
    cv::Mat grayscaled = cv::Mat(I.rows, I.cols, CV_8UC1);
    cv::Mat_<cv::Vec3b> _I = I;
    for (int x=0; x< I.cols; ++x){
        for (int y = 0; y < I.rows; ++y) {
            cv::Vec3b pix = _I.at<cv::Vec3b>(cv::Point(x,y));
            uchar grey = (pix[0] + pix[1] + pix[2]) / 3;
            grayscaled.at<uchar>(cv::Point(x,y))= grey;
        }

    }
    return grayscaled;
}


template <class T,size_t filterDimension>
cv::Mat convolutionMultiChannel(cv::Mat I, matrix<T,filterDimension,filterDimension>(filterArray)){
    cv::Mat_<cv::Vec3b> _I = I;
    cv::Mat_<cv::Vec3b> filteredI(I.rows, I.cols);
    for (unsigned int i = filterDimension; i < I.rows - filterDimension; ++i){
        for (unsigned int j = filterDimension; j < I.cols - filterDimension; ++j){
            for (int chan = 0; chan < I.channels(); ++chan){
                //convolution
                float newVal = 0;
                float division = 0;
                for (unsigned int x = 0; x < filterDimension; ++x){
                    for (unsigned int y = 0; y < filterDimension; ++y){
                        newVal += (float)_I.at<cv::Vec3b>(i + x -1  , j + y-1 )[chan] * filterArray[x][y];
                        division += filterArray[x][y];
                    }
                }

                if (division != 0.0)
                    newVal = newVal / division;
                if (newVal > 255.0)
                    newVal = 255.0;
                else if (newVal < 0.0)
                    newVal = 0.0;
                filteredI(i, j)[chan] = (uchar)newVal;
            }

        }
    }
    return filteredI;
}

template <class T,size_t filterDimension>
cv::Mat convolutionSingleChannel(const cv::Mat I, matrix<T,filterDimension,filterDimension>(filterArray)){
    cv::Mat _I = I;
    cv::Mat filteredI = cv::Mat(I.rows, I.cols, CV_8UC1);
    for (int i = 0; i < I.rows ; ++i) {
        for (int j = 0; j < I.cols; ++j) {
            filteredI.at<uchar>(i,j) = I.at<uchar>(i,j);
        }
    }
    for (unsigned int i = filterDimension; i < I.rows - filterDimension; ++i){
        for (unsigned int j = filterDimension; j < I.cols - filterDimension; ++j){

                //convolution
                float newVal = 0;
                float division = 0;
                for (unsigned int x = 0; x < filterDimension; ++x){
                    for (unsigned int y = 0; y < filterDimension; ++y){
                        newVal += (float)_I.at<uchar>(i + x -1  , j + y-1 ) * filterArray[x][y];
                        division += filterArray[x][y];
                    }
                }

                if (division != 0.0)
                    newVal = newVal / division;
                if (newVal > 255.0)
                    newVal = 255.0;
                else if (newVal < 0.0)
                    newVal = 0.0;
                filteredI.at<uchar>(i, j)= (uchar)newVal;


        }
    }
    return filteredI;
}

template cv::Mat convolutionMultiChannel<float, 3>(cv::Mat I, matrix<float,3,3>(filterArray));
template cv::Mat convolutionMultiChannel<int, 3>(cv::Mat I, matrix<int,3,3>(filterArray));

template cv::Mat convolutionSingleChannel<float, 3>(cv::Mat I, matrix<float,3,3>(filterArray));
template cv::Mat convolutionSingleChannel<int, 3>(cv::Mat I, matrix<int,3,3>(filterArray));