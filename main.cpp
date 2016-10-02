#ifdef __APPLE__
  #include <GLUT/glut.h>
  #include <OpenGL/gl.h>
  #include <OpenGL/glu.h>
#else
  #include <GL/glut.h>
  #include <GL/gl.h>
  #include <GL/glu.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "terrain.h"

// Constants.

#define W 0
#define S 1
#define A 2
#define D 3

// Global variables.

size_t window_width = 640;
size_t window_height = 480;

GLuint envDL;

Terrain t;

bool keys_down[4] = {false, false, false, false};

// Environment setup functions.

void generate_env_dl() {
  glMatrixMode(GL_MODELVIEW);
  envDL = glGenLists(1);
  glNewList(envDL, GL_COMPILE);
    glPushMatrix();
      t.draw();
    glPopMatrix();
  glEndList();
}

// Drawing callback functions.

void resize(int w, int h) {
  window_width = w;
  window_height = h;

  glViewport(0, 0, w, h);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, (float) w / h, 0.1, 100000);
}

void render() {
  glDrawBuffer(GL_BACK);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glViewport(0, 0, window_width, window_height);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  gluLookAt(0, 40, 100,
            0, 0, 0,
            0, 1, 0);

  glCallList(envDL);

  glutSwapBuffers();
}

// Input callback functions.

void normal_keys_down(unsigned char key, int x, int y) {
  if (key == 'q' || key == 'Q' || key == 27) {
    exit(0);
  }

  if (key == 'w' || key == 'W') {
    keys_down[W] = true;
  } else if (key == 's' || key == 'S') {
    keys_down[S] = true;
  } else if (key == 'a' || key == 'A') {
    keys_down[A] = true;
  } else if (key == 'd' || key == 'D') {
    keys_down[D] = true;
  }
}

void normal_keys_up(unsigned char key, int x, int y) {
  if (key == 'w' || key == 'W') {
    keys_down[W] = false;
  } else if (key == 's' || key == 'S') {
    keys_down[S] = false;
  } else if (key == 'a' || key == 'A') {
    keys_down[A] = false;
  } else if (key == 'd' || key == 'D') {
    keys_down[D] = false;
  }
}

void menu_callback(int option) {
  if (option == 0) {
    exit(0);
  }
}

// Timer functions.

void render_timer(int value) {
  glutPostRedisplay();

  glutTimerFunc(1000.0 / 24.0, render_timer, 0);
}

void anim_timer(int value) {
  // animate here

  glutTimerFunc(1000.0 / 10.0, anim_timer, 0);
}

// Misc setup functions.

void create_menu() {
  glutCreateMenu(menu_callback);
  glutAddMenuEntry("Quit", 0);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void init_scene() {
  glEnable(GL_DEPTH_TEST);

  float lightCol[4] = { 1, 1, 1, 1};
  float ambientCol[4] = { 0.0, 0.0, 0.0, 1.0 };
  float lPosition[4] = { 10, 10, 10, 1 };
  glLightfv( GL_LIGHT0, GL_POSITION,lPosition );
  glLightfv( GL_LIGHT0, GL_DIFFUSE,lightCol );
  glLightfv( GL_LIGHT0, GL_AMBIENT, ambientCol );
  glEnable( GL_LIGHTING );
  glEnable( GL_LIGHT0 );
      
  glEnable( GL_COLOR_MATERIAL );
  glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );

  glShadeModel(GL_FLAT);

  generate_env_dl();
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(window_width, window_height);
  glutCreateWindow("Guild Wars");

  glutSetKeyRepeat(GLUT_KEY_REPEAT_ON);
  glutDisplayFunc(render);
  glutReshapeFunc(resize);
  glutKeyboardFunc(normal_keys_down);
  glutKeyboardUpFunc(normal_keys_up);
  anim_timer(0);
  render_timer(0);

  init_scene();
  create_menu();
  glutMainLoop();

  return 0;
}
