//
// Created by Piotr Sykulski on 12/01/2018.
//

#ifndef CARBRANDDETECTOR_ELLIPSES_HPP
#define CARBRANDDETECTOR_ELLIPSES_HPP

#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <cmath>
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand
#include <algorithm>

struct EllipseDetection {
    unsigned int x;
    unsigned int y;
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

typedef std::pair<Point,Point> PairOfPoints;

std::vector<EllipseDetection> ellipseDetection(const cv::Mat image, const EllipseDetectionParams params);

std::vector<Point> getNonZeroPoints(const cv::Mat image);

std::vector<float> computePairwiseDistances(const std::vector<Point> points);

float calculateDistance( const float x1, const float x2, const float y1, const float y2);

std::vector<PairOfPoints> getPairsWithDistanceInRange(const std::vector<Point> points, const float minDistance, const float maxDistance);

std::vector<PairOfPoints> getPairsWithAngleInSpan(const std::vector<PairOfPoints> pairs, const float middleRotationDeg, const float rotationSpanDeg);

std::vector<PairOfPoints> getRandomSubsetOfPairs(const std::vector<PairOfPoints> &pairs, const unsigned int randomizationFactor , const unsigned int minNumberOfPairs);

float degToRad( float degrees);



#endif //CARBRANDDETECTOR_ELLIPSES_HPP
