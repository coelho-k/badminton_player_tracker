#include <body.hpp>

// def functions
void Body::setPosition(float&& x, float&& y)
{
    position.first = x;
    position.second = y;

    posList.push_back(position);
}

// Get relative court position of player
float Body::courtPosition(const Mat& mapping, const vector<Vec4i>& lines)
{
    // maps to real world
    // TODO: Change to a more efficient way. This should be temp
    vector<Point2f> input; input.push_back( Point2f(position.first, position.second) );
    vector<Point2f> output;
    perspectiveTransform(input, output, mapping);

    float pixelDist =  sqrt(pow(output[0].x - lines[0][0], 2) +  
                            pow(output[0].y - lines[0][1], 2) * 1.0);

    // cout << "Actual distance = " << pixelDist / pixelsPerMetric << endl;

    courtPos.first = sqrt(pow(output[0].x - lines[0][0], 2)) / 60.49;
    courtPos.second = sqrt(pow(output[0].y - lines[0][1], 2)) / 60.49;

    cout << "X = " << courtPos.first << "\t" << "Y = " << courtPos.second << endl;

    return pixelDist / 60.49;
}



// Calculate current speed
float currentSpeed()
{

}

