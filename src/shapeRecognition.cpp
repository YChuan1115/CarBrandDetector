//
// Created by Piotr Sykulski on 06/01/2018.
//

#include "../headers/shapeRecognition.hpp"



double calcmpq(const cv::Mat I, int p, int q){
    if(I.channels() != 1)
        throw "Not grayscale image";
    uchar thresh = 255;
    double sum = 0.0;

    for (int i = 0; i < I.rows; ++i){
        for (int j = 0; j < I.cols; ++j){
            uchar pixel = I.at<uchar>(i,j);
            double x;
            if (pixel == thresh){
                x = 1.0;
            }
            else
                x = 0.0;
            sum += cv::pow((double)i, p)*cv::pow((double)j, q)*x;
        }
    }
    return sum;
}


double calcW3(double S, double L){

    return L / (2.0 * sqrt(CV_PI*S)) - 1.0;

}

int calcArea(cv::Mat I){

    int area = 0;
    int thresh = 255;

    for (int i = 0; i < I.rows; ++i){
        for (int j = 0; j < I.cols; ++j){
            uchar pixel = I.at<uchar>(i, j);

            if (pixel == thresh ){
                ++area;
            }
        }
    }
    return area;
}

bool hasWhiteNeighbour(cv::Mat I, int i, int j){

    int thresh = 255;
    int c = 0;
    int xarr[3] = { -1,0, 1 };
    int yarr[3] = { -1,0, 1 };

    for (int x : xarr){
        for (int y : yarr){
            if (x + i>0 && x + i < I.rows && y + j>0 && y + j < I.cols ){
                uchar pixel = I.at<uchar>(i + x, j + y);
                if (pixel == thresh){
                    return true;
                }
                ++c;
            }
        }
    }


    return false;

}

int calcCircumference(cv::Mat I){
    int circumference = 0;
    int thresh = 255;

    for (int i = 0; i < I.rows; ++i){
        for (int j = 0; j < I.cols; ++j){
            uchar pixel = I.at<uchar>(i, j);

            if (pixel == thresh){
                if (hasWhiteNeighbour(I, i, j))
                    ++circumference;
            }
        }
    }
    return circumference;
}

void calculateParameters(cv::Mat I) {
    int S = calcArea(I);
    int L = calcCircumference(I);
    double W3 = calcW3((double)S, (double)L);
    double m00 = calcmpq(I, 0, 0);
    double m10 = calcmpq(I, 1, 0);
    double m01 = calcmpq(I, 0, 1);
    double m11 = calcmpq(I, 1, 1);
    double m02 = calcmpq(I, 0, 2);
    double m20 = calcmpq(I, 2, 0);
    double M11 = m11 - m10*m01 / m00;
    double M20 = m20 - cv::pow(m10, 2) / m00;
    double M02 = m02 - cv::pow(m01, 2) / m00;
    double M1 = (M20 + M02) / cv::pow(m00, 2);
    double M7 = (M20*M02 - cv::pow(M11, 2)) / cv::pow(m00, 4);

    std::cout << " S=" << S;
    std::cout << ", L=" << L;
    std::cout << ", W=" << W3;
    std::cout << ", M1=" << M1;
    std::cout << ", M7=" << M7;
    std::cout << std::endl;
}

double calculateM1(cv::Mat I){
    int S = calcArea(I);
    int L = calcCircumference(I);
    double W3 = calcW3((double)S, (double)L);
    double m00 = calcmpq(I, 0, 0);
    double m10 = calcmpq(I, 1, 0);
    double m01 = calcmpq(I, 0, 1);
    double m11 = calcmpq(I, 1, 1);
    double m02 = calcmpq(I, 0, 2);
    double m20 = calcmpq(I, 2, 0);
    double M11 = m11 - m10*m01 / m00;
    double M20 = m20 - cv::pow(m10, 2) / m00;
    double M02 = m02 - cv::pow(m01, 2) / m00;
    double M1 = (M20 + M02) / cv::pow(m00, 2);
    return M1;
}

double calculateM7(cv::Mat I){
    int S = calcArea(I);
    int L = calcCircumference(I);
    double W3 = calcW3((double)S, (double)L);
    double m00 = calcmpq(I, 0, 0);
    double m10 = calcmpq(I, 1, 0);
    double m01 = calcmpq(I, 0, 1);
    double m11 = calcmpq(I, 1, 1);
    double m02 = calcmpq(I, 0, 2);
    double m20 = calcmpq(I, 2, 0);
    double M11 = m11 - m10*m01 / m00;
    double M20 = m20 - cv::pow(m10, 2) / m00;
    double M02 = m02 - cv::pow(m01, 2) / m00;
    double M7 = (M20*M02 - cv::pow(M11, 2)) / cv::pow(m00, 4);
    return M7;
}

double getW3(cv::Mat I){
    int S = calcArea(I);
    int L = calcCircumference(I);
    return calcW3((double)S, (double)L);
}