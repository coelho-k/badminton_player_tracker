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
    void setPosition(int&& x, int&& y);

    // Functions for calculating relative court position
    // Relative to the bottom left corner of court
    float courtPosition(const vector<Point>& bounds);

    // Calculate speed
    float currentSpeed();


private:

    // current frame position
    std::pair<int, int> position;

    // current court position
    std::pair<float, float> courtPos;

    // current velocity
    std::pair<float, float> speed;

    // Position Vector
    std::vector<std::pair<int, int>> posList;

    // Velocity vector
    std::vector<std::pair<float, float>> veloList;


};

#endif