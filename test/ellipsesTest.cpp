//
// Created by Piotr Sykulski on 12/01/2018.
//


#include <iostream>
#include "catch.hpp"

#include "../headers/ellipses.hpp"
#include "../headers/preprocessing.hpp"




TEST_CASE( "Find ones in Mat of zeros") {
    cv::Mat testEmpty = cv::Mat::zeros(10,10, CV_8UC1);
    std::vector<Point> nonZeroPoints = getNonZeroPoints(testEmpty);
    REQUIRE( nonZeroPoints.size() == 0);
}

TEST_CASE( "Find ones in Mat of ones") {
    cv::Mat testFull = cv::Mat::ones(10,10, CV_8UC1);
    std::vector<Point>  nonZeroPoints = getNonZeroPoints(testFull);
    REQUIRE( nonZeroPoints.size() == 10*10);
}

TEST_CASE( "Calculate distance between 2 points") {
    REQUIRE( calculateDistance( -2.0f, 2.0f, 1.0f,1.0f) == 4.0f);
}

TEST_CASE( "Calculate pairwise distances between 3 points (including distances with themselves)") {
    std::vector<Point> points;
    points.emplace_back(Point(0,0));
    points.emplace_back(Point(2,0));
    points.emplace_back(Point(3,0));

    REQUIRE( computePairwiseDistances(points).size() == 6);
}

/*TEST_CASE( "Calculate pairwise distances between 3 points (including distances with themselves) and get only those with range between 1.0 and 2.0") {
    std::vector<Point> points;
    points.emplace_back(Point(0,0));
    points.emplace_back(Point(2,0));
    points.emplace_back(Point(3,0));
    std::vector<PairOfPoints> expectedPairs;
    expectedPairs.emplace_back(std::make_pair(Point(0,0),Point(2,0)));
    expectedPairs.emplace_back(std::make_pair(Point(2,0),Point(3,0)));
    std::vector<PairOfPoints> pairs = getPairsWithDistanceInRange(points, 1.0, 2.0);

    REQUIRE( pairs == expectedPairs);
}

TEST_CASE("Filter out all pairs with angle in range of 40-90 deegrees") {
    std::vector<PairOfPoints> pairs;
    pairs.emplace_back(std::make_pair(Point(0,0),Point(2,2)));
    pairs.emplace_back(std::make_pair(Point(0,0),Point(0,2)));
    pairs.emplace_back(std::make_pair(Point(0,0),Point(1,0)));
    std::vector<PairOfPoints> filteringResult = getPairsWithAngleInSpan(pairs, 65.0f,25.0f);

    std::vector<PairOfPoints> expected;
    expected.emplace_back(std::make_pair(Point(0,0),Point(2,2)));
    expected.emplace_back(std::make_pair(Point(0,0),Point(0,2)));

    REQUIRE( filteringResult == expected );
}*/

/*TEST_CASE("For randomization parameter equal to 0 return all pairs") {
    std::vector<PairOfPoints> pairs;
    pairs.emplace_back(std::make_pair(Point(0,0),Point(2,2)));
    pairs.emplace_back(std::make_pair(Point(0,0),Point(0,2)));
    pairs.emplace_back(std::make_pair(Point(0,0),Point(1,0)));
    REQUIRE(getRandomSubsetOfPairs(pairs, 0, 2).size() == pairs.size());
}

TEST_CASE("Return random subset, but not smaller than minimal number of pairs argument") {
    std::vector<PairOfPoints> pairs;
    pairs.emplace_back(std::make_pair(Point(0,0),Point(2,2)));
    pairs.emplace_back(std::make_pair(Point(0,0),Point(0,2)));
    pairs.emplace_back(std::make_pair(Point(0,0),Point(1,0)));
    pairs.emplace_back(std::make_pair(Point(0,0),Point(2,2)));
    pairs.emplace_back(std::make_pair(Point(0,0),Point(0,2)));
    pairs.emplace_back(std::make_pair(Point(0,0),Point(1,0)));
    pairs.emplace_back(std::make_pair(Point(0,0),Point(2,2)));
    pairs.emplace_back(std::make_pair(Point(0,0),Point(0,2)));
    pairs.emplace_back(std::make_pair(Point(0,0),Point(1,0)));
    REQUIRE(getRandomSubsetOfPairs(pairs, 2, 2).size() <= 2*2);
}*/

/*TEST_CASE("Detect ellipse in toyota logo"){
    cv::Mat color_image = cv::imread("../Photos/segments/toyseg32.png");
    cv::Mat preprocessed = cv::imread("../Photos/segments/toyseg32.png",CV_LOAD_IMAGE_GRAYSCALE);
//    cv::Mat preprocessed = preprocessing(color_image);
    std::vector<EllipseDetection> results;
    EllipseDetectionParams params;
    params.minAspectRatio = 0.5f;
    params.randomize = 2;
    params.rotation = 90.0f;
    params.rotationSpan = 10.0f;
    params.maxMajorAxis = 90.0f;
    params.minMajorAxis = 20.0f;
    results = ellipseDetection(preprocessed, params);
    EllipseDetection bestEllipse = *(results.begin());
    std::cout << "x: " << bestEllipse.x << " y: " << bestEllipse.y << " majAxis: "
              << bestEllipse.majorAxis << " minAxis: " << bestEllipse.minorAxis
              << " angle: " << bestEllipse.angle << " score " << bestEllipse.score << std::endl;

    cv::ellipse(color_image, cv::Point(bestEllipse.y,bestEllipse.x),cv::Size(bestEllipse.minorAxis,bestEllipse.majorAxis),bestEllipse.angle-45.0,0,360,cv::Scalar(0,0,255));

    params.minAspectRatio = 0.2f;
    params.randomize = 2;
    params.rotation = 0.0f;
    params.rotationSpan = 5.0f;
    params.maxMajorAxis = 60.0f;
    params.minMajorAxis = 20.0f;
    results = ellipseDetection(preprocessed, params);
    bestEllipse = *(results.begin());
    std::cout << "x: " << bestEllipse.x << " y: " << bestEllipse.y << " majAxis: "
              << bestEllipse.majorAxis << " minAxis: " << bestEllipse.minorAxis
              << " angle: " << bestEllipse.angle << " score " << bestEllipse.score << std::endl;

    cv::ellipse(color_image, cv::Point(bestEllipse.y,bestEllipse.x),cv::Size(bestEllipse.minorAxis,bestEllipse.majorAxis),bestEllipse.angle-45.0,0,360,cv::Scalar(255,0,0));


    cv::imshow("Detected ellipse", color_image);
}*/
/*

TEST_CASE("Detect ellipse in toyota logo with preprocessing"){
    cv::Mat color_image = cv::imread("../Photos/toy4cropped.jpg");
//    cv::Mat preprocessed = cv::imread("../Photos/segments/toyseg32.png",CV_LOAD_IMAGE_GRAYSCALE);
    cv::Mat preprocessed = preprocessing(color_image);
    std::vector<EllipseDetection> results;
    EllipseDetectionParams params;
    params.minAspectRatio = 0.5f;
    params.randomize = 2;
    params.rotation = 90.0f;
    params.rotationSpan = 10.0f;
    params.maxMajorAxis = 90.0f;
    params.minMajorAxis = 20.0f;
    results = ellipseDetection(preprocessed, params);
    EllipseDetection bestEllipse = *(results.begin());
    std::cout << "x: " << bestEllipse.x << " y: " << bestEllipse.y << " majAxis: "
              << bestEllipse.majorAxis << " minAxis: " << bestEllipse.minorAxis
              << " angle: " << bestEllipse.angle << " score " << bestEllipse.score << std::endl;

    cv::ellipse(color_image, cv::Point(bestEllipse.y,bestEllipse.x),cv::Size(bestEllipse.minorAxis,bestEllipse.majorAxis),bestEllipse.angle-45.0,0,360,cv::Scalar(0,0,255));

    params.minAspectRatio = 0.2f;
    params.randomize = 2;
    params.rotation = 0.0f;
    params.rotationSpan = 5.0f;
    params.maxMajorAxis = 60.0f;
    params.minMajorAxis = 20.0f;
    results = ellipseDetection(preprocessed, params);
    bestEllipse = *(results.begin());
    std::cout << "x: " << bestEllipse.x << " y: " << bestEllipse.y << " majAxis: "
              << bestEllipse.majorAxis << " minAxis: " << bestEllipse.minorAxis
              << " angle: " << bestEllipse.angle << " score " << bestEllipse.score << std::endl;

    cv::ellipse(color_image, cv::Point(bestEllipse.y,bestEllipse.x),cv::Size(bestEllipse.minorAxis,bestEllipse.majorAxis),bestEllipse.angle-45.0,0,360,cv::Scalar(255,0,0));


    cv::imshow("Detected ellipse 2", color_image);
    cvWaitKey(-1);
}*/
/*
TEST_CASE("Detect ellipse in toyota logo with preprocessing") {

    cv::Mat color_image = cv::imread("../Photos/simpletoy.jpg");
    cv::Mat bw_image = cv::imread("../Photos/simpletoy.jpg", CV_8UC1);
  //  cv::Mat preprocessed = preprocessing(color_image);

    std::vector<EllipseDetection> resultsHorizontal, resultsVertical;
    EllipseDetectionParams params;
    params.minAspectRatio = 0.1f;
    params.randomize = 1;
    params.rotation = 0.0f;
    params.rotationSpan = 5.0f;
    params.maxMajorAxis = 200.0f;
    params.minMajorAxis = 1.0f;
    resultsHorizontal = ellipseDetection(bw_image, params);
    EllipseDetection bestHorizontalEllipse = *(resultsHorizontal.begin());
    std::cout << "x: " << bestHorizontalEllipse.x << " y: " << bestHorizontalEllipse.y << " majAxis: "
              << bestHorizontalEllipse.majorAxis << " minAxis: " << bestHorizontalEllipse.minorAxis
              << " angle: " << bestHorizontalEllipse.angle << " score " << bestHorizontalEllipse.score << std::endl;

    cv::ellipse(color_image, cv::Point(bestHorizontalEllipse.x,bestHorizontalEllipse.y),cv::Size(bestHorizontalEllipse.minorAxis,bestHorizontalEllipse.majorAxis),bestHorizontalEllipse.angle,0,360,cv::Scalar(0,0,255));


    cv::Rect border(cv::Point(bestHorizontalEllipse.x,bestHorizontalEllipse.y), cv::Size(5,5));
    cv::Scalar color(0,0,255);
    int thickness = 1;
    cv::rectangle(color_image, border, color, 5);

   // cv::ellipse(color_image, cv::Point(38,58),cv::Size(24,28),180,0,360,cv::Scalar(0,255,0));
    cv::imshow("Detected ellipse", color_image);
   // cv::imshow("bw", bw_image);
    cvWaitKey(-1);
}*/
