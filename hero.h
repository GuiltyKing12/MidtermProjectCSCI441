#pragma once
#include "point.h"
#include "vector.h"

class Hero {
  public:
    Point position;
    Vector direction;
    virtual ~Hero() {};
    virtual void drawHero() = 0;
};
