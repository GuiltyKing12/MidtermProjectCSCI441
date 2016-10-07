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
    if(mode == 1) {
        position = camDir * 2 + position;
        recomputeOrientation();
    }
}

void Camera::moveBackward() {
    if(mode == 1) {
        position = camDir * -2 + position;
        recomputeOrientation();
    }
}

void Camera::zoom(float radiusChange) {
    if(mode == 2) camRadius += radiusChange;
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
    if(setMode == 1) {
        camDir = Vector(lastLook.x + -position.x , lastLook.y + -position.y, lastLook.z + -position.z);
        position =  Point(-camDir.x * sinf(camTheta) * sin(camPhi) + 100, -camDir.y * -cosf(camPhi) + 100, -camDir.z * -cosf(camTheta) * sinf(camPhi) + 100);
        camPhi = M_PI / 3.0f;
        camTheta = 2.8f;
    }
    recomputeOrientation();
}

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
            Point arcPosition = currentDir + look;
            //position = currentDir + look;
            gluLookAt(arcPosition.x, arcPosition.y, arcPosition.z,
                      look.x, look.y, look.z,
                      0, 1, 0);
            break;
    }
}
