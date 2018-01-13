//
// Created by Piotr Sykulski on 12/01/2018.
//


#include <iostream>
#include "catch.hpp"

#include "../headers/ellipses.hpp"




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

TEST_CASE( "Calculate pairwise distances between 3 points (including distances with themselves) and get only those with range between 1.0 and 2.0") {
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
}

TEST_CASE("For randomization parameter equal to 0 return all pairs") {
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
}