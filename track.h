#ifndef TRACK_H
#define TRACK_H

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

class Track {
  public:
    Track();
    Track(std::string fn);

    void draw();

    Point get_point(float t);
    int get_num_segments() { return segments; }

  private:
    int segments;
    Point** control_points;

    void load_points(std::string fn);
    void vertex(Point p);
    Point bez_curve(float t, Point p0, Point p1, Point p2, Point p3);
};

#endif
