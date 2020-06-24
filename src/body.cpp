#include <body.h>

// def functions
void Body::setPosition(int&& x, int&& y)
{
    position.first = x;
    position.second = y;

    posList.push_back(position);
}