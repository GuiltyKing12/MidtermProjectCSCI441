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
#include "point.h"
#include "vector.h"

class Wb : public Hero {
  public:
    Wb();
    Wb(Point p, Vector d, std::string pet_path_fn);

    float heading;

    void draw(bool key_down);
    void move(bool w_down, bool s_down, bool a_down, bool d_down, float x_min, float x_max, float z_min, float z_max);
    void anim();

  private:
    float pet_t;
    int count;
    float p_count;
    bool count_dir, p_count_dir;
    std::vector<Point> bez_pts;
    std::vector<Point> as;
    std::vector<Point> bs;
    std::vector<Point> cs;
    std::vector<Point> ds;

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
