#ifndef BODY_H
#define BODY_H

#include <iostream>
#include <vector>

class Body
{
public:

// getter
// setter
void setPosition(int&& x, int&& y);
// Functions for calculating angles 



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