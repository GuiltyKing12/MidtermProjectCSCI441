#ifndef OBJECTS_H
#define OBJECTS_H

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
#include "vector.h"

using namespace std;

class Objects {
  public:
    Objects();
    Objects(std::string fn);

    void load_and_draw();

  private:
    string file;
    int num_objects;
  
    void drawTree();
	void drawRock();
};

#endif