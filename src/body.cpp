#include <body.hpp>

// def functions
void Body::setPosition(int&& x, int&& y)
{
    position.first = x;
    position.second = y;

    posList.push_back(position);
}

/* Get relative court position of player
float Body::courtPosition(const vector<Point>& bounds)
{
    auto width = bounds[1].x - bounds[0].x;
    float pixelsPerMetric = width / courtWidth;

    cout << "PPM Metric = " << pixelsPerMetric << endl;

    float pixelDist =  sqrt(pow(position.first - bounds[0].x, 2) +  
                            pow(position.second - bounds[0].y, 2) * 1.0);

    cout << "Actual distance = " << pixelDist / pixelsPerMetric << endl;

    // courtPosition.first = sqrt(pow(position.first - bounds[0].x, 2)) / pixelsPerMetric;
    // courtPosition.second = sqrt(pow(position.second - bounds[0].y, 2)) / pixelsPerMetric;

    return pixelDist / pixelsPerMetric;
}
*/


// Calculate current speed
float currentSpeed()
{

}

