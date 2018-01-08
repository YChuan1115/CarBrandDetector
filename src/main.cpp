#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "../headers/utils.hpp"
#include "../headers/shapeRecognition.hpp"
#include "../headers/kernels.hpp"

using namespace cv;


Mat preprocessing(cv::Mat image){
    cv::Mat grayscale = conv2Grey(image);
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
    dilate( binary, binary, element );
   // imshow("afterDilation",binary);
    erode(binary,binary,element);
 //   imshow("afterClosing",binary);

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
    paths.push_back("../Photos/toyota_wzor.jpg");
    paths.push_back("../Photos/toyota_logo_zdjecie.jpg");
    paths.push_back("../Photos/toyota_logo_zdjecie2.jpg");
    paths.push_back("../Photos/toyota_logo_zdjecie3.jpg");
    paths.push_back("../Photos/toyota_logo_zdjecie4.jpg");
    paths.push_back("../Photos/toyota_logo_zdjecie5.jpg");
    paths.push_back("../Photos/toytest1.png");
    paths.push_back("../Photos/toytest2.png");
    paths.push_back("../Photos/toy2cropped.jpg");

    for (int i=0; i < paths.size();++i){
        std::string path = paths.at(i);
        cv::Mat newShape = cv::imread(path);
        if (!newShape.empty()){
            newShape = conv2Grey(newShape);
            shapes.push_back(newShape);
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

//    double M1 = calculateM1(area);
//    double M7 = calculateM7(area);

    Moments moments1 = moments(area, true);
    double huMoments[7];
    HuMoments(moments1,huMoments);
    std::cout << "Scan area" << std::endl;
    for( int j=0; j<7; ++j){
        std::cout << std::setw(30) << " HuMoment #" << j << " : " << huMoments[j];
        if(!(huMoments[j] <= maxHumMoments[j] && huMoments[j] >= minHumMoments[j]))
            value = false;
    }

    return value;
}

Mat naiveScan(Mat image){
    int imageHeight = image.rows;
    int imageWidth = image.cols;
    int scanHeight = 50;
    int scanWidth = 90;
    int scanOffsetX = scanWidth/2;
    int scanOffsetY = scanHeight/2;
    int stepX = 10;
    int stepY = 10;
    int scanPosX = 0;
    int scanPosY = 0;
    Rect scanRect = Rect(scanPosX,scanPosY, scanWidth, scanHeight);
    int counter = 0;

    int top = (int)(0.1*scanHeight); int bottom = top;
    int left = (int)(0.1*scanWidth); int right = left;

    while(scanPosY < imageHeight-scanHeight-1){
        Mat scanArea = image;


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


int main() {
    Mat image = imread("../Photos/toy3cropped.jpg");

    shapeMomentsAnalysis();

    image = preprocessing(image);
    image = naiveScan(image);

    imshow("scanResult", image);


    waitKey(-1);



    return 0;
}
