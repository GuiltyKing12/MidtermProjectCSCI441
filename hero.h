#pragma once
#include "point.h"
#include "vector.h"
#include <string>

class Hero {
  public:
    Point position;
    Vector direction;
    float heading;
    virtual ~Hero() {};
    //virtual void drawHero() = 0;
};
