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
            if(image.at<uchar>(i,j) > 0) {
                nonZeroPoints.emplace_back(Point(i,j));
            }else{
                //std::cout << static_cast<unsigned long>(image.at<uchar>(i,j)) << " ";
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

std::vector<AxisCandidate> getPairsWithAngleInSpan(const std::vector<AxisCandidate> &axisCandidates, const float middleRotationDeg, const float rotationSpanDeg){
    std::vector<AxisCandidate> filteredAxisCandidates;

    float tanLow = tan(degToRad(middleRotationDeg-rotationSpanDeg));
    float tanHigh = tan(degToRad(middleRotationDeg+rotationSpanDeg));
  //  std::cout << "Deg: " << middleRotationDeg-rotationSpanDeg << " " << middleRotationDeg+rotationSpanDeg << std::endl;
  //  std::cout << "DegtoRad: " << degToRad(middleRotationDeg-rotationSpanDeg) << " " << degToRad(middleRotationDeg+rotationSpanDeg) << std::endl;
  //  std::cout << tanLow << " " << tanHigh << std::endl;

    for(auto && axisCandidate : axisCandidates){
        Point point1 = axisCandidate.point1;
        Point point2 = axisCandidate.point2;
        float tangent = (static_cast<float>(point1.y-point2.y))/(static_cast<float>(point1.x-point2.x));
//        std::cout << "Tangent: " << tangent << std::endl;
        if( tanLow < tanHigh){
            if(tangent > tanLow && tangent < tanHigh)
                filteredAxisCandidates.push_back(axisCandidate);
        }else{
            if(tangent > tanLow || tangent < tanHigh)
                filteredAxisCandidates.push_back(axisCandidate);
        }
    }

    return filteredAxisCandidates;

}

std::vector<AxisCandidate> getPairsWithDistanceInRange(const std::vector<Point> &points, const float minDistance, const float maxDistance){
    std::vector<AxisCandidate> filteredPairsWithDistance;
    for(int i=0; i<points.size(); ++i){
        for( int j=i; j<points.size(); ++j){
            Point point1 = points.at(i);
            Point point2 = points.at(j);
            float distance = calculateDistance(point1.x, point2.x, point1.y, point2.y);
            if(distance >= minDistance && distance <= maxDistance){
                filteredPairsWithDistance.emplace_back(AxisCandidate(point1,point2, distance));
            }
        }
    }
    return filteredPairsWithDistance;
}

std::vector<Point> getPointsWithDistanceSmallerThan(const Point origin, const std::vector<Point> &points, const float maxDistance){
    std::vector<Point> filteredPoints;
    for(auto point : points){
        float distance = calculateDistance(point.x, origin.x, point.y, origin.y);
        if(distance <= maxDistance){
            filteredPoints.push_back(point);
        }
    }
    return filteredPoints;
}

std::vector<AxisCandidate> getRandomSubsetOfPairs(const std::vector<AxisCandidate> &axisCandidates, const unsigned int randomizationFactor , const unsigned int minNumberOfPairs){
    if( minNumberOfPairs > axisCandidates.size() || randomizationFactor == 0)
        return axisCandidates;
    std::srand(unsigned(std::time(0)));
    std::vector<unsigned int> indexes(axisCandidates.size());
    for(unsigned int i=0; i<axisCandidates.size(); ++i)
        indexes.at(i) = i;
    std::random_shuffle(indexes.begin(),indexes.end());
    unsigned int numberOfPairs = std::min(static_cast<unsigned int>(axisCandidates.size()), minNumberOfPairs*randomizationFactor);
    std::vector<AxisCandidate> selectedPairs;
    for(int i=0; i < numberOfPairs; ++i){
        selectedPairs.push_back(axisCandidates.at(indexes.at(i)));
    }
    return selectedPairs;
}


float calculateDistance( const float x1, const float x2, const float y1, const float y2){
    return static_cast<float>(sqrt(pow(x1- x2, 2.0) + pow(y1 - y2, 2.0)));
}

//float calculateDistance( const PairOfPoints pair){
//    return static_cast<float>(sqrt(pow(pair.first.x- pair.second.x, 2.0) + pow(pair.first.y - pair.second.y, 2.0)));
//}

float calculateDistanceSquared( const float x1, const float x2, const float y1, const float y2){
    return static_cast<float>((pow(x1- x2, 2.0) + pow(y1 - y2, 2.0)));
}

//float calculateDistanceSquared( const PairOfPoints pair){
//    return static_cast<float>((pow(pair.first.x- pair.second.x, 2.0) + pow(pair.first.y - pair.second.y, 2.0)));
//}

float degToRad( float degrees){
    return static_cast<float>(degrees* static_cast<float>(M_PI)/180.0f);
}

float radToDeg( float radians){
    return static_cast<float>(radians*180.0f/static_cast<float>(M_PI));
}

inline Point calculateEllipseCenter(AxisCandidate majorAxisVertices){
    long centerX = (majorAxisVertices.point1.x + majorAxisVertices.point2.x)/2;
    long centerY = (majorAxisVertices.point1.y + majorAxisVertices.point2.y)/2;
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
   // std::cout << "Possible major axes: " << nonZeroPoints.size()<<"x"<<nonZeroPoints.size()<<" "<< nonZeroPoints.size()*nonZeroPoints.size() << std::endl;
    std::vector<AxisCandidate> filteredAxisCandidates = getPairsWithDistanceInRange(nonZeroPoints, params.minMajorAxis*2, params.maxMajorAxis*2);
//    std::cout << "After distance constraint: " << filteredAxisCandidates.size()<< std::endl;


    filteredAxisCandidates = getPairsWithAngleInSpan(filteredAxisCandidates, params.rotation, params.rotationSpan);
   // std::cout << "After angle constraint: " << filteredAxisCandidates.size()<< std::endl;

    filteredAxisCandidates = getRandomSubsetOfPairs(filteredAxisCandidates, params.randomize, static_cast<unsigned int>(nonZeroPoints.size()));
  //  std::cout << "After randomization: " << filteredAxisCandidates.size()<< std::endl;



    for( auto && axisCandidate : filteredAxisCandidates){
        Point center = calculateEllipseCenter(axisCandidate);
        float majAxisSquared = static_cast<float>(pow(axisCandidate.distance/2.0,2.0));
        std::vector<Point> thirdPointCandidates = getPointsWithDistanceSmallerThan(center, nonZeroPoints, axisCandidate.distance/2.0f);
        std::vector<unsigned long> accumulator(static_cast<unsigned long>(ceil(axisCandidate.distance/2.0f)),0);

        // Calculate minor axes for all candidates
        for(Point point : thirdPointCandidates){
            float thirdPointDistSq = calculateDistanceSquared(point.x,center.x, point.y, center.y);
            float fSq = calculateDistanceSquared(point.x, axisCandidate.point2.x, point.y, axisCandidate.point2.y);
            float cosTau = (majAxisSquared + thirdPointDistSq - fSq)/(2.0f*sqrtf(thirdPointDistSq*majAxisSquared));
            cosTau = std::min(1.0f,std::max(-1.0f,cosTau));
            float sinTauSq = 1.0f - static_cast<float>(pow(cosTau, 2.0));
            float minAxis = sqrt((majAxisSquared*thirdPointDistSq*sinTauSq)/
                                         (eps+majAxisSquared-thirdPointDistSq*static_cast<float>(pow(cosTau,2.0))));
            accumulator.at(static_cast<unsigned long>(floor(minAxis))) += 1;
        }
        // Discard ellipses with wrong aspect ratio
        auto minMinorAxisLengthFromMinAspectRatio = static_cast<unsigned long>((axisCandidate.distance/2.0f)*params.minAspectRatio);
        if(minMinorAxisLengthFromMinAspectRatio < accumulator.size()){
            for(auto it = accumulator.begin(); it != accumulator.begin()+ minMinorAxisLengthFromMinAspectRatio ; ++it){
                (*it) = 0;
            }
        }

        auto maxMinorAxisLengthFromMaxAspectRatio = static_cast<unsigned long>(floor((axisCandidate.distance/2.0f)*params.maxAspectRatio));
        if(maxMinorAxisLengthFromMaxAspectRatio < accumulator.size()){
            for(auto it = accumulator.begin()+maxMinorAxisLengthFromMaxAspectRatio; it != accumulator.end() ; ++it){
                (*it) = 0;
            }
        }


        auto bestElem = std::max_element(accumulator.begin(),accumulator.end());
        unsigned long score = *bestElem;
        if(score > result.score){
            result.score  = score;
            result.x = static_cast<unsigned long>(center.y);
            result.y = static_cast<unsigned long>(center.x);
            result.majorAxis = axisCandidate.distance/2.0f;
            result.minorAxis = bestElem - accumulator.begin();
            result.angle = radToDeg(static_cast<float>(atan2(axisCandidate.point1.y-axisCandidate.point2.y, axisCandidate.point1.x-axisCandidate.point2.x)));
        }
    }

    results.push_back(result);
    return results;
}

std::pair<bool, Point> find2Ellipses(Mat & image, Mat & color_image){

    std::vector<EllipseDetection> resultsHorizontal, resultsVertical;
    EllipseDetectionParams params;
    params.minAspectRatio = 0.4f;
    params.maxAspectRatio = 0.72f;
    params.randomize = 1;
    params.rotation = 90.0f;
    params.rotationSpan = 20.0f;
    params.maxMajorAxis = 50.0f;
    params.minMajorAxis = 10.0f;
    resultsHorizontal = ellipseDetection(image, params);
    EllipseDetection bestHorizontalEllipse = *(resultsHorizontal.begin());

   //  cv::ellipse(color_image, cv::Point(bestHorizontalEllipse.y,bestHorizontalEllipse.x),cv::Size(bestHorizontalEllipse.minorAxis,bestHorizontalEllipse.majorAxis),bestHorizontalEllipse.angle-45.0,0,360,cv::Scalar(0,0,255));

    params.minAspectRatio = 0.2f;
    params.maxAspectRatio = 0.4f;
    params.rotation = bestHorizontalEllipse.angle + 90.0f;
    params.rotationSpan = 10.0f;
    params.maxMajorAxis = bestHorizontalEllipse.majorAxis*0.7f;
    params.minMajorAxis = bestHorizontalEllipse.majorAxis*0.5f;
    resultsVertical = ellipseDetection(image, params);
    EllipseDetection bestVerticalEllipse = *(resultsVertical.begin());

    float ellipsesDist = calculateDistance(bestHorizontalEllipse.x, bestVerticalEllipse.x, bestHorizontalEllipse.y, bestVerticalEllipse.y);
  // cv::ellipse(color_image, cv::Point(bestVerticalEllipse.y,bestVerticalEllipse.x),cv::Size(bestVerticalEllipse.minorAxis,bestVerticalEllipse.majorAxis),bestVerticalEllipse.angle-45.0,0,360,cv::Scalar(255,0,0));

    std::pair<bool, Point> result(false, Point(0,0));
    if(bestHorizontalEllipse.score > 20.0f && bestVerticalEllipse.score > 20.0f && ellipsesDist < 5.0f && bestHorizontalEllipse.majorAxis > bestVerticalEllipse.majorAxis*1.3f) {
        result.first = true;
        result.second = Point(bestHorizontalEllipse.x, bestHorizontalEllipse.y);
        cv::ellipse(color_image, cv::Point(bestVerticalEllipse.x,bestVerticalEllipse.y),cv::Size(bestVerticalEllipse.minorAxis,bestVerticalEllipse.majorAxis),bestVerticalEllipse.angle,0,360,cv::Scalar(255,255,255));
        cv::ellipse(color_image, cv::Point(bestHorizontalEllipse.x,bestHorizontalEllipse.y),cv::Size(bestHorizontalEllipse.minorAxis,bestHorizontalEllipse.majorAxis),bestHorizontalEllipse.angle,0,360,cv::Scalar(255,255,255));
        std::cout << "x: " << bestHorizontalEllipse.x << " y: " << bestHorizontalEllipse.y << " majAxis: "
                  << bestHorizontalEllipse.majorAxis << " minAxis: " << bestHorizontalEllipse.minorAxis
                  << " angle: " << bestHorizontalEllipse.angle << " score " << bestHorizontalEllipse.score << std::endl;
        std::cout << "x: " << bestVerticalEllipse.x << " y: " << bestVerticalEllipse.y << " majAxis: "
                  << bestVerticalEllipse.majorAxis << " minAxis: " << bestVerticalEllipse.minorAxis
                  << " angle: " << bestVerticalEllipse.angle << " score " << bestVerticalEllipse.score << std::endl;

    }

    return result;
}