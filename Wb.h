#ifndef WB_H
#define WB_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "hero.h"

struct Point {
  float x;
  float y;
  float z;
};

class Wb {
  public:
    Wb();
    Wb(float** t, std::string pet_path_fn);

    void draw(bool key_down, bool control_cage, bool curve);
    void move(bool w_down, bool s_down, bool a_down, bool d_down, float x_min, float x_max, float z_min, float z_max);
    float get_y();
    void anim();

    float get_x() { return x; }
    float get_z() { return z; }
    float get_heading() { return heading; }
    float get_dx() { return dx; }
    float get_dz() { return dz; }
    void set_x(float nx) { x = nx; }
    void set_z(float nz) { z = nz; }
    void set_heading(float nh) { heading = nh; calculate_direction(); }

  private:
    float** terrain;
    float x, z, heading;
    float dx, dz;
    float pet_t;
    int count;
    float p_count;
    bool count_dir, p_count_dir;
    std::vector<Point> bez_pts;
    std::vector<Point> as;
    std::vector<Point> bs;
    std::vector<Point> cs;
    std::vector<Point> ds;

    void calculate_direction();
    void load_pet_path_pts(std::string fn);
    void calculate_pet_path_coefficients();
    Point get_pet_path_pt(float t);
    void draw_body();
    void draw_legs(float angle);
    void draw_leg();
    void draw_arm(float angle);
    void draw_head();
    void draw_wing(float angle);
    void draw_pet();
    void draw_pet_head_top();
    void draw_pet_eyes();
    void draw_pet_head_bottom();
    void draw_pet_mouth(float t);
};

#endif
