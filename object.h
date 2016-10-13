#ifndef OBJECT_H
#define OBJECT_H

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
#include <math.h>
#include "point.h"
#include "vector.h"

using namespace std;

class Object {
  public:
    Object();
	Object(string t, Point p, float s);

    void draw(Point pos, Vector norm);

    string type;
    //float coords [];
	Point loc;
	float size;
};

#endif