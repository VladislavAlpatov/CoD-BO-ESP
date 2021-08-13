#pragma once
#include <math.h>

class ImVec2
{
public:
    float x, y;
    ImVec2(float x, float y)
    {
        this->x = x;
        this->y = y;
    }
    ImVec2()
    {

    }
};
class ImVec3 : public ImVec2
{
public:
    float z;

    ImVec3(float x, float y, float z)  : ImVec2(x, y)
    {
        this->z = z;
    }

};
