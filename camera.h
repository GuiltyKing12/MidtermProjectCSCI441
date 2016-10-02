#pragma once
#include "point.h"
#include "vector.h"

class Camera {
  public:
    int mode;
    Point position;
    Vector camDir;
    float camRadius, camTheta, camPhi;
    
  // consturctors for camera
  public:
    Camera() {}
    
    Camera(int setMode, float posX, float posY, float posZ,
                        float cameraRadius, float cameraTheta, float cameraPhi) {
        mode = setMode;
        
        camRadius = cameraRadius;
        camTheta = cameraTheta;
        camPhi = cameraPhi;
        
        switch(mode) {
            case 1 :
                position = Point(posX, posY, posZ);
                break;
            case 2 :
                position = Point(posX+cameraRadius, posY+cameraRadius, posZ+cameraRadius);
                break;
        
        }
        
        recomputeOrientation();
    }
    
  // camera controls
  public:
    void moveForward();
    void moveBackward();
    
    void zoom(float radiusChange);
    void revolve(float theta, float phi);
    
    void switchMode(int setMode);
    
    void recomputeOrientation();
    void look();
};
