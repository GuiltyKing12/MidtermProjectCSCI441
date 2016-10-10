#ifdef __APPLE__			// if compiling on Mac OS
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else					// else compiling on Linux OS
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include "camera.h"
#include <math.h>

/** MODE 1 - Free cam control **/
void Camera::moveForward() {
    if(mode == 1) {
        position = Point(camDir.x * 2+position.x, camDir.y * 2+position.y, camDir.z * 2+position.z);
        recomputeOrientation();
    }
}

void Camera::moveBackward() {
    if(mode == 1) {
        position = Point(camDir.x * -2+position.x, camDir.y * -2+position.y, camDir.z * -2+position.z);
        recomputeOrientation();
    }
}

/** MODE 2 - ARCball controls **/
void Camera::zoom(float radiusChange) {
    if(mode == 2) camRadius += radiusChange;
}

/** General movement control for both Modes **/
void Camera::revolve(float theta, float phi) {
    
        camTheta += theta;
    
        camPhi += phi;
        if(camPhi > 3.14) camPhi -= phi;
        else if(camPhi < 0) camPhi -= phi;
    
        recomputeOrientation();
    
}

void Camera::switchMode(int setMode) {
    mode = setMode;
    if(setMode == 1) {
        position =  Point(-camDir.x * sinf(camTheta) * sin(camPhi) + 100, -camDir.y * -cosf(camPhi) + 100, -camDir.z * -cosf(camTheta) * sinf(camPhi) + 100);
        camPhi = M_PI / 3.0f;
        camTheta = -1.0f;
    }
    else if(setMode == 2) {
        camTheta = M_PI / 3.0f;
        camPhi = 2.8f;
    }
    recomputeOrientation();
}

/** Functions to control the orientation and where to look **/

void Camera::recomputeOrientation() {
    camDir.x = sinf(camTheta) * sin(camPhi);
    camDir.y = -cosf(camPhi);
    camDir.z = -cosf(camTheta) * sinf(camPhi);
    
    // and NORMALIZE this directional vector!!!
    float magnitude = sqrt(camDir.x * camDir.x + camDir.y * camDir.y + camDir.z * camDir.z);
    camDir.x = camDir.x / magnitude;
    camDir.y = camDir.y / magnitude;
    camDir.z = camDir.z / magnitude;
}

void Camera::look(Point look) {
    switch(mode) {
        case 1 :
            gluLookAt(position.x, position.y, position.z,
                      camDir.x+position.x, camDir.y+position.y, camDir.z+position.z,
                      0, 1, 0);
            break;
        case 2 :
            Vector currentDir = camDir * camRadius;
            lastLook = look;
            Point arcPosition = Point(currentDir.x+look.x, currentDir.y+look.y, currentDir.z+look.z);
            gluLookAt(arcPosition.x, arcPosition.y, arcPosition.z,
                      look.x, look.y, look.z,
                      0, 1, 0);
            break;
    }
}

void Camera::fpvLook(Point heroPos, Vector heroDir, float heroHeading) {
    Vector ortho = heroDir.Cross(Vector(1, 0, 0));
    Vector u_n = heroDir / heroDir.magnitude();
    float c = cos(heroHeading);
    float s = sin(heroHeading);
    Vector pt(
      (c + u_n.x * u_n.x * (1 - c)) * ortho.x
        + (u_n.x * u_n.y * (1 - c) - u_n.z * s) * ortho.y
        + (u_n.x * u_n.z * (1 - c) + u_n.y * s) * ortho.z,

      (u_n.y * u_n.x * (1 - c) + u_n.z * s) * ortho.x
        + (c + u_n.y * u_n.y * (1 - c)) * ortho.y
        + (u_n.y * u_n.z * (1 - c) - u_n.x * s) * ortho.z,

      (u_n.z * u_n.x * (1 - c) - u_n.y * s) * ortho.x
        + (u_n.z * u_n.y * (1 - c) + u_n.x * s) * ortho.y
        + (c + u_n.z * u_n.z * (1 - c)) * ortho.z
    );

    u_n = u_n * 15;
    pt = pt * 10;

    gluLookAt(heroPos.x + u_n.x, heroPos.y + u_n.y, heroPos.z + u_n.z,
              heroPos.x + pt.x, heroPos.y + pt.y, heroPos.z + pt.z,
              0, 1, 0);
}
