#include <iostream>
#include <iomanip>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "ellipses.hpp"
#include "preprocessing.hpp"




cv::Mat naiveScan(cv::Mat image , cv::Mat original_image){
    int imageHeight = image.rows;
    int imageWidth = image.cols;
    int scanHeight = 80;
    int scanWidth = 180;
    int stepX = 50;
    int stepY = 30;
    int scanPosX = 0;
    int scanPosY = 0;
    cv::Rect scanRect = cv::Rect(scanPosX,scanPosY, scanWidth, scanHeight);
    int counter = 0;
    int nrOfCols = image.cols/stepX;
    int nrOfRows = image.rows/stepY;
    int nrOfScanAreas = nrOfCols*nrOfRows;

    cv::Scalar color(0,0,255);
    int thickness = 2;
    int radius = 4;

    while(scanPosY < imageHeight-scanHeight-1){
        cv::Mat scanArea = image(scanRect);
        cv::Mat drawArea = original_image(scanRect);
        std::pair<bool, Point> result = find2Ellipses(scanArea, drawArea);
        if(result.first){
            cv::Point location = cv::Point(static_cast<int>(scanRect.x + result.second.x), static_cast<int>(scanRect.y+result.second.y));
            cv::circle(original_image, location, radius, color,thickness);
            std::cout << "Found logo at x: " << scanPosX << " y: " << scanPosY << std::endl;
        }

        if( scanPosX < imageWidth-scanWidth-1){
            scanRect = cv::Rect(scanPosX,scanPosY, scanWidth, scanHeight);
            scanPosX += stepX;
        }else{
            scanPosX = 0;
            scanPosY += stepY;
            scanRect = cv::Rect(scanPosX,scanPosY, scanWidth, scanHeight);
        }
        ++counter;
        std::cout << '\r';
        std::cout << "Progress: " << counter << "/" << nrOfScanAreas << std::endl;
    }
    return original_image;
}

void toyotaSearch(const std::string &picturePath){
    cv::Mat originalImage = cv::imread(picturePath);
    if(originalImage.data == nullptr){
        std::cerr << picturePath << " is not a proper image file" << std::endl;
        return;
    }
    cv::Mat preprocessedImage = preprocessing(originalImage);
    cv::Mat scannedImage = naiveScan(preprocessedImage, originalImage);
    cv::imshow("scanResult " + picturePath, scannedImage);
    cv::waitKey(1);
    std::cout << "Finished searching logos" << std::endl;
}

int main(int argc, char* argv[]) {

    if ( argc < 2){
        std::cerr << "Usage: " << argv[0] << " PATH_TO_PICTURE" << std::endl;
        return 1;
    }

    for(int i = 1; i < argc; ++i){
        toyotaSearch(argv[i]);
    }

    cv::waitKey(-1);

    return 0;
}
