#pragma once
#include <math.h>

class Vector {
  public:
    float x, y, z;
    
    Vector() {
        x = 0; y = 0; z = 0;
    }
    
    Vector(float x, float y, float z) {
        this->x = x; this->y = y; this->z = z;
    }
    
    Vector operator*(float m) {
        return Vector(x * m, y * m, z * m);
    }
    
    Vector operator+(Vector v) {
        return Vector(x + v.x, y + v.y, z + v.z);
    }

    Vector operator/(float f) {
      return Vector(x / f, y / f, z / f);
    }
    
    // Point operator+(Point p) {
    //     return Point(x + p.x, y + p.y, z + p.z);
    // }
    
    float magnitude() {
        return sqrt(x * x + y * y + z * z);
    }
    
    float Dot(Vector a) {
        return acos((a.x * x + a.y * y + a.z * z) / (a.magnitude() * this->magnitude()));
    }
    
    Vector Cross(Vector a) {
        float nx = a.y * z - y * a.z;
        float ny = -(a.x * z - x * a.z);
        float nz = a.x * y - x * a.y;
        
        return Vector(nx, ny, nz);
    }
};
