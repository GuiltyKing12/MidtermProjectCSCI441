#ifndef POINT_H
#define POINT_H

class Point {
  public:
    float x, y, z;

    Point() {
      x = 0; y = 0; z = 0;
    }

    Point(float nx, float ny, float nz) {
      x = nx; y = ny; z = nz;
    }

    Point operator*(float m) {
      return Point(x * m, y * m, z * m);
    }

    Point operator+(Point o) {
      return Point(x + o.x, y + o.y, z + o.z);
    }
};

#endif
