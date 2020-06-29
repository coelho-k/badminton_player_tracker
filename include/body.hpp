#ifndef BODY_HPP
#define BODY_HPP

#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;

// const float courtWidth = 6.10;

class Body
{
public:

    // get most recent position
    std::pair<int, int> getPosition() { return position; }

    // setter
    void setPosition(float&& x, float&& y);

    // Functions for calculating relative court position
    // Relative to the bottom left corner of court
    float courtPosition(const Mat& mapping, const vector<Vec4i>& lines);

    // Calculate speed
    float currentVelocity();


private:

    // current frame position
    std::pair<float, float> position;

    // current court position
    std::pair<float, float> courtPos;

    // current velocity
    std::pair<float, float> speed;

    // Position Vector
    std::vector<std::pair<float, float>> posList;

    // Velocity vector
    std::vector<std::pair<float, float>> veloList;


};

#endif