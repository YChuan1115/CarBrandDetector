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

using namespace cv;


const int SCALE = 10;

Mat levels(cv::Mat image){
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

Mat preprocessing(cv::Mat image){
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
            resize(newShape,largerShape, Size(), SCALE,SCALE);
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

        Moments moments1 = moments(shapes.at(i), true);
        double huMoments[7];
        HuMoments(moments1,huMoments);
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

bool findLogo(Mat area){
    bool value = true;
    Mat scaledArea;
    resize(area,scaledArea, Size(), SCALE,SCALE);
//    double M1 = calculateM1(area);
//    double M7 = calculateM7(area);
    double W3 = getW3(scaledArea);
    Moments moments1 = moments(scaledArea, true);
    double huMoments[7];
    HuMoments(moments1,huMoments);
    std::cout << "Scan area" << std::endl;
    for( int j=0; j<3; ++j){
        //std::cout << std::setw(30) << " HuMoment #" << j << " : " << huMoments[j];
        if(!(huMoments[j] <= maxHumMoments[j] && huMoments[j] >= minHumMoments[j]))
            value = false;
    }

    return value;
}

Mat naiveScan(Mat image){
    int imageHeight = image.rows;
    int imageWidth = image.cols;
    int scanHeight = 80;
    int scanWidth = 90;
    int scanOffsetX = scanWidth/2;
    int scanOffsetY = scanHeight/2;
    int stepX = 20;
    int stepY = 20;
    int scanPosX = 0;
    int scanPosY = 0;
    Rect scanRect = Rect(scanPosX,scanPosY, scanWidth, scanHeight);
    int counter = 0;

    int top = (int)(0.1*scanHeight); int bottom = top;
    int left = (int)(0.1*scanWidth); int right = left;

    while(scanPosY < imageHeight-scanHeight-1){
        Mat scanArea = image(scanRect);
//        Mat scanArea = image;

        if(findLogo(scanArea)){
            Rect border(cv::Point(0,0), scanArea.size());
            Scalar color(255,255,255);
            int thickness = 1;
            rectangle(scanArea,border,color,thickness);
            std::cout << "Found logo at x: " << scanPosX << " y: " << scanPosY << std::endl;
        }

        if( scanPosX < imageWidth-scanWidth-1){
            scanRect = Rect(scanPosX,scanPosY, scanWidth, scanHeight);
            scanPosX += stepX;
        }else{
            scanPosX = 0;
            scanPosY += stepY;
            scanRect = Rect(scanPosX,scanPosY, scanWidth, scanHeight);
        }
        ++counter;
    }
    return image;
}

void preprocessAndSaveScanArea(){

}

void mainImageSearch(){
    Mat originalImage = imread("../Photos/toyota4.jpg");

    shapeMomentsAnalysis();

    Mat preprocessedImage = preprocessing(originalImage);
    //  imwrite("../Photos/results/toyPreProcessed.jpg", image);
    Mat scannedImage = naiveScan(preprocessedImage);

//    imshow("originalImage", originalImage);
//    imshow("preprocessed", preprocessedImage);
    imshow("scanResult", scannedImage);
}

void tests(){
    Mat originalImage = imread("../Photos/toyota_wzor.jpg");

    Mat greyscaleImage = conv2Grey(originalImage);
    Mat binary;
    threshold(greyscaleImage, binary, 127, 255, THRESH_BINARY);
    int dilation_type = MORPH_RECT;
    int dilation_size = 4;
    Mat element = getStructuringElement( dilation_type,
                                         Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                                         Point( dilation_size, dilation_size ) );
    Mat dil1,dil2,dil3,dil4;
    dilate(binary,dil1,element);
    dilate(dil1,dil2,element);
    dilate(dil2,dil3,element);
    dilate(dil3,dil4,element);
    imshow("dil1",dil1);
    imshow("dil2",dil2);
    imshow("dil3",dil3);
    imshow("dil4",dil4);

}

void saveSegmets(Mat image, std::string path){
    int imageHeight = image.rows;
    int imageWidth = image.cols;
    int scanHeight = 100;
    int scanWidth = 200;
    int scanOffsetX = scanWidth/2;
    int scanOffsetY = scanHeight/2;
    int stepX = 30;
    int stepY = 30;
    int scanPosX = 0;
    int scanPosY = 0;
    Rect scanRect = Rect(scanPosX,scanPosY, scanWidth, scanHeight);
    int counter = 0;

    int top = (int)(0.1*scanHeight); int bottom = top;
    int left = (int)(0.1*scanWidth); int right = left;

    while(scanPosY < imageHeight-scanHeight-1){
        Mat scanArea = image(scanRect);
        std::string fullpath = path+std::to_string(counter);
        fullpath = fullpath + ".png";
        imwrite(fullpath, scanArea);

        if( scanPosX < imageWidth-scanWidth-1){
            scanRect = Rect(scanPosX,scanPosY, scanWidth, scanHeight);
            scanPosX += stepX;
        }else{
            scanPosX = 0;
            scanPosY += stepY;
            scanRect = Rect(scanPosX,scanPosY, scanWidth, scanHeight);
        }
        ++counter;
    }
}

void test2(){
    Mat originalImage = imread("../Photos/toy2small.jpg");
    Mat preprocessed = preprocessing(originalImage);
    saveSegmets(preprocessed, "../Photos/segments3/toyseg");
}

int main() {

//    tests();
 //   mainImageSearch();
    test2();
 //   waitKey(-1);



    return 0;
}
