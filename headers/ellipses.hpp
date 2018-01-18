//
// Created by Piotr Sykulski on 12/01/2018.
//

#ifndef CARBRANDDETECTOR_ELLIPSES_HPP
#define CARBRANDDETECTOR_ELLIPSES_HPP

#include <vector>
#include <tuple>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <cmath>
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand
#include <algorithm>

struct EllipseDetection {
    unsigned long x;
    unsigned long y;
    float majorAxis;
    float minorAxis;
    float angle;
    float score;
};
struct EllipseDetectionParams {
    float minMajorAxis;
    float maxMajorAxis;
    float rotation;
    float rotationSpan;
    float minAspectRatio;
    float maxAspectRatio;
    unsigned int randomize;
};

struct Point {
    long x;
    long y;
    Point(long x , long y): x(x),  y(y){}
    bool operator==(const Point point) const {
        return x == point.x && y == point.y;
    }
};

struct AxisCandidate {
    Point point1;
    Point point2;
    float distance;
    AxisCandidate(Point point1, Point point2, float dist): point1(point1), point2(point2), distance(dist){};
};


std::pair<bool, Point> find2Ellipses(cv::Mat & image, cv::Mat & color_image);

std::vector<EllipseDetection> ellipseDetection(const cv::Mat image, const EllipseDetectionParams params);

std::vector<Point> getNonZeroPoints(const cv::Mat &image);

std::vector<float> computePairwiseDistances(const std::vector<Point> &points);

float calculateDistance( const float x1, const float x2, const float y1, const float y2);

std::vector<AxisCandidate> getPairsWithDistanceInRange(const std::vector<Point> &points, const float minDistance, const float maxDistance);

std::vector<AxisCandidate> getPairsWithAngleInSpan(const std::vector<AxisCandidate> &axisCandidates, const float middleRotationDeg, const float rotationSpanDeg);

std::vector<AxisCandidate> getRandomSubsetOfPairs(const std::vector<AxisCandidate> &axisCandidates, const unsigned int randomizationFactor , const unsigned int minNumberOfPairs);

float degToRad( float degrees);



#endif //CARBRANDDETECTOR_ELLIPSES_HPP
