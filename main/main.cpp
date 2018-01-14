#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "utils.hpp"
#include "shapeRecognition.hpp"
#include "kernels.hpp"
#include "ellipses.hpp"
#include "preprocessing.hpp"



const int SCALE = 10;

cv::Mat levels(cv::Mat image){
    cv::Mat levels = cv::Mat(image.rows,image.cols, CV_8UC1);
    for(int i=0; i<image.rows; ++i){
        for(int j=0; j<image.cols; ++j){
            uchar pix = image.at<uchar>(i,j);
            if(pix > 250)
                levels.at<uchar>(i,j) = 180;
            else
                levels.at<uchar>(i,j) = pix;
        }
    }
    return levels;
}




double maxHumMoments[7] = {0.0};
double minHumMoments[7] = {std::numeric_limits<double>::max()};

void shapeMomentsAnalysis(){
    std::vector<std::string> paths;
    std::vector<cv::Mat> shapes;
    /*paths.push_back("../Photos/toyota_wzor.jpg");
    paths.push_back("../Photos/toyota_logo_zdjecie.jpg");
    paths.push_back("../Photos/toyota_logo_zdjecie2.jpg");
    paths.push_back("../Photos/toyota_logo_zdjecie3.jpg");
    paths.push_back("../Photos/toyota_logo_zdjecie4.jpg");
    paths.push_back("../Photos/toyota_logo_zdjecie5.jpg");
    paths.push_back("../Photos/toytest1.png");
    paths.push_back("../Photos/toytest2.png");
    paths.push_back("../Photos/toy2cropped.jpg");
    paths.push_back("../Photos/toyPreProcessed.jpg");*/
    paths.push_back("../Photos/toy4cropped.jpg");
    paths.push_back("../Photos/toy3cropped.jpg");
    paths.push_back("../Photos/toyota5cropped.jpg");
    paths.push_back("../Photos/toyota2cropped.jpg");
    paths.push_back("../Photos/toyota4cropped.jpg");

    for (int i=0; i < paths.size();++i){
        std::string path = paths.at(i);
        cv::Mat newShape = cv::imread(path);
        if (!newShape.empty()){
            newShape = conv2Grey(newShape);
            newShape = preprocessing(newShape);
            cv::Mat largerShape;
            resize(newShape,largerShape, cv::Size(), SCALE,SCALE);
            shapes.push_back(largerShape);
            std::cout << "Dodano plik: " << path << std::endl;
        }else{
            paths.erase(paths.begin()+i);
            --i;
        }
    }

    for(int i=0; i <shapes.size(); ++i){
       // std::cout<< "Calculating parameters for file: " << paths.at(i) << std::endl;
       // calculateParameters(shapes.at(i));

        cv::Moments moments1 = moments(shapes.at(i), true);
        double huMoments[7];
        cv::HuMoments(moments1,huMoments);
        for( int j=0; j<7; ++j){
            if( huMoments[j] > maxHumMoments[j])
                maxHumMoments[j] = huMoments[j];
            if( huMoments[j] < minHumMoments[j])
                minHumMoments[j] = huMoments[j];
            std::cout << std::setw(30) << " HuMoment #" << j << " : " << huMoments[j];
        }
        std::cout << std::endl;
    }
}

bool findLogo(cv::Mat area){
    bool value = true;
    cv:: Mat scaledArea;
    cv::resize(area,scaledArea, cv::Size(), SCALE,SCALE);
//    double M1 = calculateM1(area);
//    double M7 = calculateM7(area);
    double W3 = getW3(scaledArea);
    cv::Moments moments1 = moments(scaledArea, true);
    double huMoments[7];
    cv::HuMoments(moments1,huMoments);
    std::cout << "Scan area" << std::endl;
    for( int j=0; j<3; ++j){
        //std::cout << std::setw(30) << " HuMoment #" << j << " : " << huMoments[j];
        if(!(huMoments[j] <= maxHumMoments[j] && huMoments[j] >= minHumMoments[j]))
            value = false;
    }

    return value;
}

cv::Mat naiveScan(cv::Mat image , cv::Mat original_image){
    int imageHeight = image.rows;
    int imageWidth = image.cols;
    int scanHeight = 100;
    int scanWidth = 200;
    int scanOffsetX = scanWidth/2;
    int scanOffsetY = scanHeight/2;
    int stepX = 40;
    int stepY = 90;
    int scanPosX = 0;
    int scanPosY = 0;
    cv::Rect scanRect = cv::Rect(scanPosX,scanPosY, scanWidth, scanHeight);
    int counter = 0;

    int top = (int)(0.1*scanHeight); int bottom = top;
    int left = (int)(0.1*scanWidth); int right = left;

    while(scanPosY < imageHeight-scanHeight-1){
        cv::Mat scanArea = image(scanRect);
//        Mat scanArea = image;
        std::pair<bool, Point> result = find2Ellipses(scanArea);
        if(result.first){
            cv::Rect border(cv::Point(scanPosX+result.second.x,scanPosY+result.second.y), cv::Size(10,10));
            cv::Scalar color(0,0,255);
            int thickness = 1;
            cv::rectangle(original_image, border, color, 5);
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
    }
    return original_image;
}

void preprocessAndSaveScanArea(){

}

void mainImageSearch(){
    cv::Mat originalImage = cv::imread("../Photos/toyota2.jpg");

   // shapeMomentsAnalysis();

    cv::Mat preprocessedImage = preprocessing(originalImage);
    //  imwrite("../Photos/results/toyPreProcessed.jpg", image);
    cv::Mat scannedImage = naiveScan(preprocessedImage, originalImage);

//    imshow("originalImage", originalImage);
    cv::imshow("preprocessed", preprocessedImage);
    cv::imshow("scanResult", scannedImage);
}

void tests(){
    cv::Mat originalImage = cv::imread("../Photos/toyota_wzor.jpg");

    cv::Mat greyscaleImage = conv2Grey(originalImage);
    cv::Mat binary;
    cv::threshold(greyscaleImage, binary, 127, 255, cv::THRESH_BINARY);
    int dilation_type = cv::MORPH_RECT;
    int dilation_size = 4;
    cv::Mat element = cv::getStructuringElement( dilation_type,
                                                 cv::Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                                                 cv::Point( dilation_size, dilation_size ) );
    cv::Mat dil1,dil2,dil3,dil4;
    cv::dilate(binary,dil1,element);
    cv::dilate(dil1,dil2,element);
    cv::dilate(dil2,dil3,element);
    cv::dilate(dil3,dil4,element);
    cv::imshow("dil1",dil1);
    cv::imshow("dil2",dil2);
    cv::imshow("dil3",dil3);
    cv::imshow("dil4",dil4);

}

void saveSegmets(cv::Mat image, std::string path){
    int imageHeight = image.rows;
    int imageWidth = image.cols;
    int scanHeight = 80;
    int scanWidth = 150;
    int scanOffsetX = scanWidth/2;
    int scanOffsetY = scanHeight/2;
    int stepX = 30;
    int stepY = 30;
    int scanPosX = 0;
    int scanPosY = 0;
    cv::Rect scanRect = cv::Rect(scanPosX,scanPosY, scanWidth, scanHeight);
    int counter = 0;

    int top = (int)(0.1*scanHeight); int bottom = top;
    int left = (int)(0.1*scanWidth); int right = left;

    while(scanPosY < imageHeight-scanHeight-1){
        cv::Mat scanArea = image(scanRect);
        std::string fullpath = path+std::to_string(counter);
        fullpath = fullpath + ".png";
        cv::imwrite(fullpath, scanArea);

        if( scanPosX < imageWidth-scanWidth-1){
            scanRect = cv::Rect(scanPosX,scanPosY, scanWidth, scanHeight);
            scanPosX += stepX;
        }else{
            scanPosX = 0;
            scanPosY += stepY;
            scanRect = cv::Rect(scanPosX,scanPosY, scanWidth, scanHeight);
        }
        ++counter;
    }
}

void test2(){
    cv::Mat originalImage = cv::imread("../Photos/toy2small.jpg");
    cv::Mat preprocessed = preprocessing(originalImage);
    saveSegmets(preprocessed, "../Photos/segments3/toyseg");
}

int main() {

//    tests();
    mainImageSearch();
//    test2();
    cv::waitKey(-1);



    return 0;
}
