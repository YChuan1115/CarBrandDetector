//
// Created by Piotr Sykulski on 12/01/2018.
//

#include <iostream>
#include "../headers/ellipses.hpp"

using cv::Mat;


std::vector<Point> getNonZeroPoints(const Mat image){
    std::vector<Point> nonZeroPoints;
    for(int i=0; i < image.rows; ++i){
        for(int j=0; j<image.cols; ++j){
            if(image.at<uchar>(i,j) == 255) {
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
    std::cout << "Deg: " << middleRotationDeg-rotationSpanDeg << " " << middleRotationDeg+rotationSpanDeg << std::endl;
    std::cout << "DegtoRad: " << degToRad(middleRotationDeg-rotationSpanDeg) << " " << degToRad(middleRotationDeg+rotationSpanDeg) << std::endl;
    std::cout << tanLow << " " << tanHigh << std::endl;

    for(auto && pair : pairs){
        Point point1 = pair.first;
        Point point2 = pair.second;
        float tangent = (static_cast<float>(point1.y-point2.y))/(static_cast<float>(point1.x-point2.x));
//        std::cout << "Tangent: " << tangent << std::endl;
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

std::vector<Point> getPointsWithDistanceSmallerThan(const Point origin, const std::vector<Point> points, const float maxDistance){
    std::vector<Point> filteredPoints;
    for(auto point : points){
        float distance = calculateDistance(point.x, origin.x, point.y, origin.y);
        if(distance <= maxDistance){
            filteredPoints.push_back(point);
        }
    }
    return filteredPoints;
}

std::vector<PairOfPoints> getRandomSubsetOfPairs(const std::vector<PairOfPoints> &pairs, const unsigned int randomizationFactor , const unsigned int minNumberOfPairs){
    if( minNumberOfPairs > pairs.size() || randomizationFactor == 0)
        return pairs;
    std::srand(unsigned(std::time(0)));
    std::vector<unsigned int> indexes(pairs.size());
    for(unsigned int i=0; i<pairs.size(); ++i)
        indexes.at(i) = i;
    std::random_shuffle(indexes.begin(),indexes.end());
    unsigned int numberOfPairs = std::min(static_cast<unsigned int>(pairs.size()), minNumberOfPairs*randomizationFactor);
    std::vector<PairOfPoints> selectedPairs;
    for(int i=0; i < numberOfPairs; ++i){
        selectedPairs.push_back(pairs.at(indexes.at(i)));
    }
    return selectedPairs;
}


float calculateDistance( const float x1, const float x2, const float y1, const float y2){
    return static_cast<float>(sqrt(pow(x1- x2, 2.0) + pow(y1 - y2, 2.0)));
}

float calculateDistance( const PairOfPoints pair){
    return static_cast<float>(sqrt(pow(pair.first.x- pair.second.x, 2.0) + pow(pair.first.y - pair.second.y, 2.0)));
}

float calculateDistanceSquared( const float x1, const float x2, const float y1, const float y2){
    return static_cast<float>((pow(x1- x2, 2.0) + pow(y1 - y2, 2.0)));
}

float calculateDistanceSquared( const PairOfPoints pair){
    return static_cast<float>((pow(pair.first.x- pair.second.x, 2.0) + pow(pair.first.y - pair.second.y, 2.0)));
}

float degToRad( float degrees){
    return static_cast<float>(degrees* static_cast<float>(M_PI)/180.0f);
}

float radToDeg( float radians){
    return static_cast<float>(radians*180.0f/static_cast<float>(M_PI));
}

inline Point calculateEllipseCenter(PairOfPoints majorAxisVertices){
    long centerX = (majorAxisVertices.first.x + majorAxisVertices.second.x)/2;
    long centerY = (majorAxisVertices.first.y + majorAxisVertices.second.y)/2;
    return {centerX,centerY};
}

std::vector<EllipseDetection> ellipseDetection(const Mat image, const EllipseDetectionParams params){
    std::vector<EllipseDetection> results;
    float eps = 0.001;
    EllipseDetection result;
    result.score = 0.0f;
    Mat I = Mat(image);
    //cv::imshow("Detected ellipse", I);
    std::vector<Point> nonZeroPoints = getNonZeroPoints(I);
    std::cout << "Possible major axes: " << nonZeroPoints.size()<<"x"<<nonZeroPoints.size()<<" "<< nonZeroPoints.size()*nonZeroPoints.size() << std::endl;

    std::vector<PairOfPoints> filteredPairs = getPairsWithDistanceInRange(nonZeroPoints, params.minMajorAxis, params.maxMajorAxis);
    std::cout << "After distance constraint: " << filteredPairs.size()<< std::endl;

    filteredPairs = getRandomSubsetOfPairs(filteredPairs, params.randomize, static_cast<unsigned int>(nonZeroPoints.size()));
    std::cout << "After randomization: " << filteredPairs.size()<< std::endl;

    filteredPairs = getPairsWithAngleInSpan(filteredPairs, params.rotation, params.rotationSpan);
    std::cout << "After angle constraint: " << filteredPairs.size()<< std::endl;


    for( auto && pair : filteredPairs){
        Point center = calculateEllipseCenter(pair);
        float majorAxis = calculateDistance(pair)/2;
        std::vector<Point> thirdPointCandidates = getPointsWithDistanceSmallerThan(center, nonZeroPoints, majorAxis);
        float majAxisSquared = calculateDistanceSquared(pair)/4;
        std::vector<unsigned long> accumulator(static_cast<unsigned long>(ceil(majorAxis)),0);

        // Calculate minor axes for all candidates
        for(Point point : thirdPointCandidates){
            float thirdPointDistSq = calculateDistanceSquared(point.x,center.x, point.y, center.y);
            float fSq = calculateDistanceSquared(point.x, pair.second.x, point.y, pair.second.y);
            float cosTau = (majAxisSquared + thirdPointDistSq - fSq)/(2.0f*sqrtf(majAxisSquared*thirdPointDistSq));
            cosTau = std::min(1.0f,std::max(-1.0f,cosTau));
            float sinTauSq = 1.0f - static_cast<float>(pow(cosTau, 2.0));
            float minAxis = sqrt((majAxisSquared*thirdPointDistSq*sinTauSq)/
                                         (eps+majAxisSquared-thirdPointDistSq*static_cast<float>(pow(cosTau,2.0))));
            accumulator.at(static_cast<unsigned long>(floor(minAxis))) += 1;
        }
        // Discard ellipses with wrong aspect ratio
        for(auto it = accumulator.begin(); it != accumulator.begin()+ static_cast<unsigned long>(majorAxis*params.minAspectRatio); ++it){
            (*it) = 0;
        }
        auto bestElem = std::max_element(accumulator.begin(),accumulator.end());
        unsigned long score = *bestElem;
        if(score > result.score){
            result.score  = score;
            result.x = static_cast<unsigned long>(center.x);
            result.y = static_cast<unsigned long>(center.y);
            result.majorAxis = majorAxis;
            result.minorAxis = bestElem - accumulator.begin();
            result.angle = radToDeg(static_cast<float>(atan2(pair.first.y-pair.second.y, pair.first.x-pair.first.y)));
        }
    }

    results.push_back(result);
    return results;
}

std::pair<bool, Point> find2Ellipses(const Mat image){

    std::vector<EllipseDetection> resultsHorizontal, resultsVertical;
    EllipseDetectionParams params;
    params.minAspectRatio = 0.4f;
    params.randomize = 6;
    params.rotation = 90.0f;
    params.rotationSpan = 10.0f;
    params.maxMajorAxis = 120.0f;
    params.minMajorAxis = 20.0f;
    resultsHorizontal = ellipseDetection(image, params);
    EllipseDetection bestHorizontalEllipse = *(resultsHorizontal.begin());
    std::cout << "x: " << bestHorizontalEllipse.x << " y: " << bestHorizontalEllipse.y << " majAxis: "
              << bestHorizontalEllipse.majorAxis << " minAxis: " << bestHorizontalEllipse.minorAxis
              << " angle: " << bestHorizontalEllipse.angle << " score " << bestHorizontalEllipse.score << std::endl;

  //  cv::ellipse(image, cv::Point(bestEllipse.y,bestEllipse.x),cv::Size(bestEllipse.minorAxis,bestEllipse.majorAxis),bestEllipse.angle-45.0,0,360,cv::Scalar(0,0,255));

    params.minAspectRatio = 0.2f;
    params.rotation = 0.0f;
    params.rotationSpan = 10.0f;
    params.maxMajorAxis = 80.0f;
    params.minMajorAxis = 10.0f;
    resultsVertical = ellipseDetection(image, params);
    EllipseDetection bestVerticalEllipse = *(resultsVertical.begin());
    std::cout << "x: " << bestVerticalEllipse.x << " y: " << bestVerticalEllipse.y << " majAxis: "
              << bestVerticalEllipse.majorAxis << " minAxis: " << bestVerticalEllipse.minorAxis
              << " angle: " << bestVerticalEllipse.angle << " score " << bestVerticalEllipse.score << std::endl;

    float ellipsesDist = calculateDistance(bestHorizontalEllipse.x, bestVerticalEllipse.x, bestHorizontalEllipse.y, bestVerticalEllipse.y);
    //  cv::ellipse(color_image, cv::Point(bestEllipse.y,bestEllipse.x),cv::Size(bestEllipse.minorAxis,bestEllipse.majorAxis),bestEllipse.angle-45.0,0,360,cv::Scalar(255,0,0));

    std::pair<bool, Point> result(false, Point(0,0));
    if(bestHorizontalEllipse.score > 30.0f && bestVerticalEllipse.score > 30.0f && ellipsesDist < 15.0f && bestHorizontalEllipse.majorAxis > bestVerticalEllipse.majorAxis*1.3f) {
        result.first = true;
        result.second = Point(bestHorizontalEllipse.y, bestHorizontalEllipse.x);
    }

    return result;
}