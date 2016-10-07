#ifndef TERRAIN_H
#define TERRAIN_H

#ifdef __APPLE__
  #include <GLUT/glut.h>
  #include <OpenGL/gl.h>
  #include <OpenGL/glu.h>
#else
  #include <GL/glut.h>
  #include <GL/gl.h>
  #include <GL/glu.h>
#endif
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include "point.h"

class Terrain {
  public:
    Terrain();
    Terrain(std::string fn);

    void draw();
    
    Point bez_patch(float x, float z);

  private:
    int side_length;
    Point**** control_points;

    void load_points(std::string fn);
    void vertex(Point p);
    Point bez_curve(float t, Point p0, Point p1, Point p2, Point p3);
};

#endif
