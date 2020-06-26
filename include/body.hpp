#ifndef BODY_HPP
#define BODY_HPP

#include <iostream>
#include <vector>

class Body
{
public:

    Body();

    ~Body();

    // get most recent position
    std::pair<int, int> getPosition() { return position; }

    // setter
    void setPosition(int&& x, int&& y);

    // Functions for calculating relative court position
    // Relative to the bottom left corner of court



private:

    // current position
    std::pair<int, int> position;

    // current velocity
    std::pair<float, float> speed;

    // Position Vector
    std::vector<std::pair<int, int>> posList;

    // Velocity vector
    std::vector<std::pair<float, float>> veloList;


};

#endif