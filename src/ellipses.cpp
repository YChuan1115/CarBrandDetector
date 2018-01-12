//
// Created by Piotr Sykulski on 12/01/2018.
//

#include "../headers/ellipses.hpp"

using cv::Mat;


std::vector<Point> getNonZeroPoints(const Mat image){
    std::vector<Point> nonZeroPoints;
    for(int i=0; i < image.rows; ++i){
        for(int j=0; j<image.cols; ++j){
            if(image.at<uchar>(i,j) > 0) {
                nonZeroPoints.emplace_back(Point(i,j));
            }
        }
    }
    return nonZeroPoints;
}

std::vector<float> computePairwiseDistances(const std::vector<Point> points){
    std::vector<float> distances;
    for(int i=0; i<points.size(); ++i){
        for( int j=i; j<points.size(); ++j){
            Point point1 = points.at(i);
            Point point2 = points.at(j);
            distances.push_back(calculateDistance(point1.x, point2.x, point1.y, point2.y));
        }
    }
    return distances;
}

std::vector<PairOfPoints> getPairsWithAngleInSpan(const std::vector<PairOfPoints> pairs, const float middleRotationDeg, const float rotationSpanDeg){
    std::vector<PairOfPoints> filteredPairs;

    float tanLow = tan(degToRad(middleRotationDeg-rotationSpanDeg));
    float tanHigh = tan(degToRad(middleRotationDeg+rotationSpanDeg));

    for(auto && pair : pairs){
        Point point1 = pair.first;
        Point point2 = pair.second;
        float tangent = static_cast<float>(point1.y-point2.y)/ static_cast<float>(point1.x-point2.x);
        if( tanLow < tanHigh){
            if(tangent > tanLow && tangent < tanHigh)
                filteredPairs.push_back(pair);
        }else{
            if(tangent > tanLow || tangent < tanHigh)
                filteredPairs.push_back(pair);
        }
    }

    return filteredPairs;

}

std::vector<PairOfPoints> getPairsWithDistanceInRange(const std::vector<Point> points, const float minDistance, const float maxDistance){
    std::vector<PairOfPoints> filteredPairs;
    for(int i=0; i<points.size(); ++i){
        for( int j=i; j<points.size(); ++j){
            Point point1 = points.at(i);
            Point point2 = points.at(j);
            float distance = calculateDistance(point1.x, point2.x, point1.y, point2.y);
            if(distance >= minDistance && distance <= maxDistance){
                filteredPairs.emplace_back(std::make_pair(point1,point2));
            }
        }
    }
    return filteredPairs;
}

float calculateDistance( const float x1, const float x2, const float y1, const float y2){
    return static_cast<float>(sqrt(pow(x1- x2, 2.0) + pow(y1 - y2, 2.0)));
}

float degToRad( float degrees){
    return static_cast<float>(degrees*M_PI/180.0f);
}

std::vector<EllipseDetection> ellipseDetection(const Mat image, const EllipseDetectionParams params){
    Mat I = Mat(image);
    std::vector<Point> nonZeroPoints = getNonZeroPoints(I);
    std::vector<PairOfPoints> filteredPairs = getPairsWithDistanceInRange(nonZeroPoints, params.minMajorAxis, params.maxMajorAxis);
    filteredPairs = getPairsWithAngleInSpan(filteredPairs, params.rotation, params.rotationSpan);

}