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

void Camera::moveForward() {
    position = camDir * 2 + position;
    recomputeOrientation();
}

void Camera::moveBackward() {
    position = camDir * 2 + position * -1;
    recomputeOrientation();
}

void Camera::zoom(float radiusChange) {
    camRadius += radiusChange;
}

void Camera::revolve(float theta, float phi) {
    camTheta += theta;
    
    camPhi += phi;
    if(camPhi > 3.14) camPhi -= phi;
    else if(camPhi < 0) camPhi -= phi;
    
    recomputeOrientation();
}

void Camera::switchMode(int setMode) {
    mode = setMode;
}

void Camera::recomputeOrientation() {
    camDir.x = sin(camTheta) * sin(camPhi);
    camDir.y = cos(camPhi);
    camDir.z = cos(camTheta) * sin(camPhi);
    
    // and NORMALIZE this directional vector!!!
    float magnitude = sqrt(camDir.x * camDir.x + camDir.y * camDir.y + camDir.z * camDir.z);
    camDir.x = camDir.x / magnitude;
    camDir.y = camDir.y / magnitude;
    camDir.z = camDir.z / magnitude;
}

void Camera::look() {
    switch(mode) {
        case 1 :
            gluLookAt(position.x, position.y, position.z,
                      camDir.x+position.x, camDir.y+position.y, camDir.z+position.z,
                      0, 1, 0);
            break;
        case 2 :
            gluLookAt(position.x, position.y, position.z,
                      0, 0, 0,
                      0, 1, 0);
            break;
    }
}
