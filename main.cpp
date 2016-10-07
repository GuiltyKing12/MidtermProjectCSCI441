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
#include "camera.h"
#include "point.h"
#include "Artoria.h"
#include "Finjuh.h"
#include "track.h"

// Constants.

#define W 0
#define S 1
#define A 2
#define D 3

// Global variables.

size_t window_width = 640;
size_t window_height = 480;

GLuint envDL;
GLint leftMouseButton;                      // status of the mouse buttons
int mouseX = 0, mouseY = 0;                 // last known X and Y of the mouse

Terrain t("terrain_pts_2.csv");
Track tr;

// Camera
Camera mainCamera;
Camera fpvCamera;

// Heroes
Hero* currentHero;

Artoria* artoria;
Finjuh* finjuh;

bool keys_down[4] = {false, false, false, false};
float wh_x = 1.0;
float wh_z = 1.0;
bool keys[256];
int a = 0;

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

  float x = 200 * cos((float)a / 180 * 3.14159);
  float z = 200 * sin((float)a / 180 * 3.14159);
  mainCamera.look(currentHero->position);

  // Here is the wandering hero drawing code. I think the final
  // code should look more like this once the Hero class is done:
  // Point p = t.bez_patch(...
  // Vector n = t.normal(...
  // wb.set_location(p);
  // wb.set_normal(n);
  // ... and the rest of this math would be done within the Hero.
  Point p = t.bez_patch(wh_x, wh_z);
  Vector n = t.normal(wh_x, wh_z) * 15;
  Vector y(0, 1, 0);
  Vector axis = n.Cross(y);
  float angle = n.Dot(y) / 3.14159 * 180;
  glPushMatrix();
  glTranslatef(p.x, p.y, p.z);
  glRotatef(angle, axis.x, axis.y, axis.z);
  glTranslatef(0, 8, 0);
  glScalef(4, 16, 4);
    glColor3f(0, 1, 1);
    glutSolidCube(1);
  glPopMatrix();

  // Draw the terrain.
  glCallList(envDL);

  artoria->drawHero();
  finjuh->drawHero();
  artoria->moveHeroForward();
  artoria->recomputeHeroDirection();
    
  glutSwapBuffers();
}

// Input callback functions.

void mouseCallback(int button, int state, int thisX, int thisY) {
    // update the left mouse button states, if applicable for ctrl click
    if(button == GLUT_LEFT_BUTTON && glutGetModifiers() == GLUT_ACTIVE_CTRL) {
        leftMouseButton = 100;
        mouseX = thisX;
        mouseY = thisY;
    }
    else if(button == GLUT_LEFT_BUTTON) {
        leftMouseButton = state;
        mouseX = thisX;
        mouseY = thisY;
    }
}

void mouseMotion(int x, int y) {
    // if ctrl click we zoom rather than move around the hero
    if(leftMouseButton == 100) {
        mainCamera.zoom((x - mouseX));
    }
    // change positon of the camera
    if(leftMouseButton == GLUT_DOWN) {
        float theta = ((x - mouseX) * .005);
        float phi = ((mouseY - y) * .005);
        
        mainCamera.revolve(theta, phi);
        glutPostRedisplay();	    // redraw our scene from our new camera POV
    }
    mouseX = x;
    mouseY = y;
}

void normal_keys_down(unsigned char key, int x, int y) {
  if (key == 'q' || key == 'Q' || key == 27) {
    exit(0);
  }

  keys[key] = true;
    
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
  keys[key] = false;
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

void check_keys() {
  float speed = 0.03;
  if (keys_down[W]) {
    wh_z -= speed;
  }
  if (keys_down[S]) {
    wh_z += speed;
  }
  if (keys_down[A]) {
    wh_x -= speed;
  }
  if (keys_down[D]) {
    wh_x += speed;
  }

  if (wh_x < 0) {
    wh_x = 0;
  } else if (wh_x > t.get_side_length()) {
    wh_x = t.get_side_length();
  }

  if (wh_z < 0) {
    wh_z = 0;
  } else if (wh_z > t.get_side_length()) {
    wh_z = t.get_side_length();
  }
    
    if(keys['i'] || keys['I']) {
        mainCamera.moveForward();
        glutPostRedisplay();
    }
    else if(keys['k'] || keys['K']) {
        mainCamera.moveBackward();
        glutPostRedisplay();
    }
 
}

void menu_callback(int option) {
  if (option == 0) {
    exit(0);
  }
}

void subMenu_callback(int option) {
    if (option == 0) currentHero = artoria;
    else if (option == 1) currentHero = finjuh;
}

void subMenu2_callback(int option) {
    if (option == 0) mainCamera.switchMode(1);
    else if (option == 1) mainCamera.switchMode(2);
}

// Timer functions.

void render_timer(int value) {
  check_keys();
  glutPostRedisplay();

  glutTimerFunc(1000.0 / 24.0, render_timer, 0);
}

void anim_timer(int value) {
  // Do animation stuff here
  artoria->shakeTail();
  glutTimerFunc(1000.0 / 10.0, anim_timer, 0);
}

// Misc setup functions.

void create_menu() {
  int subMenu = glutCreateMenu(subMenu_callback);
  glutAddMenuEntry("Artoria", 0);
  glutAddMenuEntry("Finjuh", 1);
  glutAddMenuEntry("WB", 2);
  int subMenu2 = glutCreateMenu(subMenu2_callback);
    glutAddMenuEntry("Free Cam Mode", 0);
    glutAddMenuEntry("Arcball Cam Mode", 1);
    glutAddMenuEntry("Hero POV view", 2);
  glutCreateMenu(menu_callback);
  glutAddSubMenu("Change Hero", subMenu);
    glutAddSubMenu("Change Camera", subMenu2);
  glutAddMenuEntry("Quit", 0);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void init_scene() {
  glEnable(GL_DEPTH_TEST);

  float lightCol[4] = { 1, 1, 1, 1 };
  float ambientCol[4] = { 0.0, 0.0, 0.0, 1.0 };
  float lPosition[4] = { 0, 80, -220, 1 };
  glLightfv( GL_LIGHT0, GL_POSITION, lPosition );
  glLightfv( GL_LIGHT0, GL_DIFFUSE, lightCol );
  glLightfv( GL_LIGHT0, GL_AMBIENT, ambientCol );
  glEnable( GL_LIGHTING );
  glEnable( GL_LIGHT0 );
      
  glEnable( GL_COLOR_MATERIAL );
  glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );

  glShadeModel(GL_SMOOTH);

  generate_env_dl();
}

int main(int argc, char** argv) {
  float cameraTheta = M_PI / 3.0f;
  float cameraPhi = 2.8f;
  float cameraRadius = 300;

  // draw the heroes
  artoria = new Artoria(Point(0, 0, 0), Vector(0, 0, 0));
  finjuh = new Finjuh(Point(20, 0, 20), Vector(0, 0, 0));
  
  // set camera to arcball initially
  mainCamera = Camera(2, 0, 0, 0, cameraRadius, cameraTheta, cameraPhi);
  currentHero = artoria;
    
  // initialize glut and make sure we have everything set up
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(window_width, window_height);
  glutCreateWindow("Guild Wars");

  // register callbacks
  glutSetKeyRepeat(GLUT_KEY_REPEAT_ON);
  glutDisplayFunc(render);
  glutReshapeFunc(resize);
  glutKeyboardFunc(normal_keys_down);
  glutKeyboardUpFunc(normal_keys_up);
  glutMouseFunc(mouseCallback);
  glutMotionFunc(mouseMotion);
  anim_timer(0);
  render_timer(0);

  init_scene();
  create_menu();
  glutMainLoop();

  return 0;
}
