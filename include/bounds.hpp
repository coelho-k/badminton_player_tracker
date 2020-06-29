#ifndef BOUNDS_HPP
#define BOUNDS_HPP

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/video.hpp>

using namespace cv;
using namespace std;

const int thresh = 200;
const int max_thresh = 255;
const int blockSize = 5;
const int apertureSize = 3;
const double k = 0.01;
const float courtWidth = 6.10; // court width
const float courtHeight = 13.4; // court height

// Uses masks to isolate the court and the boundary
Mat courtMask(Mat& frame);

// Gets court boundary lines from mask
vector<Vec4i> getLines(Mat& masked);

// Get reference / edges of court position
void sortLines(vector<Vec4i>& lines);

// Returns the transformation matrix mapping
Mat calibrate(const Mat& frame, const vector<Vec4i>& lines);

#endif