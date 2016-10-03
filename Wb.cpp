#include "Wb.h"

Wb::Wb() {
  x = 0;
  z = 0;
  heading = 0;
  dx = 0;
  dz = 0.1;
  count = 0;
  count_dir = true;
  calculate_direction();
}

Wb::Wb(float** t, std::string pet_path_fn) {
  x = 0;
  z = 0;
  heading = 0;
  dx = 0;
  dz = 0.1;
  count = 0;
  count_dir = true;
  terrain = t;
  calculate_direction();
  load_pet_path_pts(pet_path_fn);
  calculate_pet_path_coefficients();
}

void Wb::move(bool w_down, bool s_down, bool a_down, bool d_down, float x_min, float x_max, float z_min, float z_max) {
  if (w_down) {
    x += dx;
    z += dz;
  }
  if (s_down) {
    x -= dx;
    z -= dz;
  }
  if (a_down) {
    heading += 0.2;
    calculate_direction();
  }
  if (d_down) {
    heading -= 0.2;
    calculate_direction();
  }

  if (x < x_min) {
    x = x_min;
  } else if (x > x_max - 1) {
    x = x_max - 1;
  }
  if (z < z_min) {
    z = z_min;
  } else if (z > z_max - 1) {
    z = z_max - 1;
  }
}

void Wb::anim() {
  if (count_dir) {
    count += 1;

    if (count == 10) {
      count_dir = !count_dir;
    }
  } else {
    count -= 1;
    
    if (count == 0) {
      count_dir = !count_dir;
    }
  }

  if (p_count_dir) {
    p_count += 0.01;

    if (p_count >= 0.2) {
      p_count_dir = !p_count_dir;
    }
  } else {
    p_count -= 0.01;

    if (p_count <= 0.0) {
      p_count_dir = !p_count_dir;
    }
  }

  pet_t += 0.05;
  if (pet_t >= (bez_pts.size() - 1) / 3) {
    pet_t = 0.0;
  }
}

void Wb::calculate_direction() {
  float speed = 0.3;
  dx = speed * sin(heading);
  dz = speed * cos(heading);
}

float Wb::get_y() {
  int x_i = x;
  int z_i = z;
  float left = x - x_i;
  float top = z - z_i;

  // I have no idea why these have to be backwards but it works!
  float top_lerp = (1.0 - left) * terrain[z_i][x_i] + left * terrain[z_i][x_i + 1];
  float bot_lerp = (1.0 - left) * terrain[z_i + 1][x_i] + left * terrain[z_i + 1][x_i + 1];

  return (1.0 - top) * top_lerp + top * bot_lerp;
}

void Wb::load_pet_path_pts(std::string fn) {
  std::ifstream infile(fn.c_str());
  std::string line;
  while (infile) {
    if (!getline(infile, line)) {
      break;
    }

    float x = atof(line.substr(0, line.find_first_of(',')).c_str());
    float y = atof(line.substr(line.find_first_of(',') + 1, line.find_last_of(',') - line.find_first_of(',') - 1).c_str());
    float z = atof(line.substr(line.find_last_of(',') + 1).c_str());

    Point p;
    p.x = x;
    p.y = y;
    p.z = z;

    bez_pts.push_back(p);
  }
  infile.close();
}

void Wb::calculate_pet_path_coefficients() {
  for (size_t i = 3; i < bez_pts.size(); i += 3) {
    Point p0 = bez_pts[i - 3];
    Point p1 = bez_pts[i - 2];
    Point p2 = bez_pts[i - 1];
    Point p3 = bez_pts[i];
    Point a;
    a.x = -1.0 * p0.x + 3.0 * p1.x - 3.0 * p2.x + p3.x;
    a.y = -1.0 * p0.y + 3.0 * p1.y - 3.0 * p2.y + p3.y;
    a.z = -1.0 * p0.z + 3.0 * p1.z - 3.0 * p2.z + p3.z;
    Point b;
    b.x = 3.0 * p0.x - 6.0 * p1.x + 3.0 * p2.x;
    b.y = 3.0 * p0.y - 6.0 * p1.y + 3.0 * p2.y;
    b.z = 3.0 * p0.z - 6.0 * p1.z + 3.0 * p2.z;
    Point c;
    c.x = -3.0 * p0.x + 3.0 * p1.x;
    c.y = -3.0 * p0.y + 3.0 * p1.y;
    c.z = -3.0 * p0.z + 3.0 * p1.z;
    as.push_back(a);
    bs.push_back(b);
    cs.push_back(c);
    ds.push_back(p0);
  }
}

Point Wb::get_pet_path_pt(float t) {
  int ai = 0;
  while (t >= 1.0) {
    t -= 1.0;
    ai += 1;
  }
  float t2 = t * t;
  float t3 = t2 * t;
  Point p;
  p.x = t3 * as[ai].x + t2 * bs[ai].x + t * cs[ai].x + ds[ai].x;
  p.y = t3 * as[ai].y + t2 * bs[ai].y + t * cs[ai].y + ds[ai].y;
  p.z = t3 * as[ai].z + t2 * bs[ai].z + t * cs[ai].z + ds[ai].z;
  return p;
}

void Wb::draw(bool key_down, bool control_cage, bool curve) {
  float arm_angle = 30;
  if (key_down) {
    arm_angle = 30 + count * 4;
  }
  glPushMatrix();
  glTranslatef(x, get_y() + 0.5 + (float)count / 15.0, z);
  glScalef(0.5, 0.5, 0.5);
  glRotatef(heading * 180.0 / 3.14159, 0, 1, 0);
    draw_body();

    draw_legs(-25);

    glPushMatrix();
    glTranslatef(0, -0.75, 0.4);
    glRotatef(180, 0, 1, 0);
      draw_legs(-25);
    glPopMatrix();

    draw_arm(arm_angle);

    glPushMatrix();
    glRotatef(180, 0, 1, 0);
      draw_arm(arm_angle);
    glPopMatrix();

    draw_head();

    draw_wing(20 + count);
    draw_wing(-20 - count);
    
    if (control_cage) {
      for (size_t i = 0; i < bez_pts.size(); i += 1) {
        glPushMatrix();
        glTranslatef(bez_pts[i].x, bez_pts[i].y, bez_pts[i].z);
          glColor3f(0, 1, 0);
          glutSolidSphere(0.2, 20, 20);
        glPopMatrix();
      }

      glDisable(GL_LIGHTING);
      glPushMatrix();
        glColor3f(1, 1, 0);
        glLineWidth(1);
        glBegin(GL_LINE_STRIP);
          for (size_t i = 0; i < bez_pts.size(); i += 1) {
            glVertex3f(bez_pts[i].x, bez_pts[i].y, bez_pts[i].z);
          }
        glEnd();
      glPopMatrix();
      glEnable(GL_LIGHTING);
    }

    if (curve) {
      glDisable(GL_LIGHTING);
      glPushMatrix();
        glColor3f(0, 1, 1);
        glLineWidth(3);
        glBegin(GL_LINE_STRIP);
          for (float t = 0; t < (bez_pts.size() - 1) / 3; t += 0.01) {
            Point f = get_pet_path_pt(t);
            glVertex3f(f.x, f.y, f.z);
          }
        glEnd();
      glPopMatrix();
      glEnable(GL_LIGHTING);
    }

    Point pf = get_pet_path_pt(pet_t);
    glPushMatrix();
    glTranslatef(pf.x, pf.y, pf.z);
      draw_pet();
    glPopMatrix();
  glPopMatrix();
}

void Wb::draw_body() {
  glPushMatrix();
    glPushMatrix();
    glScalef(1, 1.0, 0.4);
      glColor3f(1, 0.8, 0.6);
      glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, -0.7, 0);
    glScalef(1, 0.4, 0.4);
      glColor3f(0, 0.3, 1);
      glutSolidCube(1);
    glPopMatrix();
  glPopMatrix();
}

void Wb::draw_legs(float angle) {
  glPushMatrix();
  glTranslatef(0, -0.5, 0);
  glRotatef(angle, 1, 0, 0);
  glTranslatef(-0.3, -0.7, -0.1);
    draw_leg();

    glPushMatrix();
    glTranslatef(0.6, 0, 0);
      draw_leg();
    glPopMatrix();
  glPopMatrix();
}

void Wb::draw_leg() {
  glPushMatrix();
  glScalef(0.4, 1, 0.4);
    glColor3f(0, 0.3, 1);
    glutSolidCube(1);
  glPopMatrix();
}

void Wb::draw_arm(float angle) {
  glPushMatrix();
  glTranslatef(0.45, 0.2, 0);
  glRotatef(angle, 0, 0, 1);
  glTranslatef(0, -0.5, 0);
  glScalef(0.4, 1.3, 0.4);
    glColor3f(1, 0.8, 0.6);
    glutSolidCube(1);
  glPopMatrix();
}

void Wb::draw_head() {
  glPushMatrix();
  glTranslatef(0, 0.9, 0);
    glColor3f(1, 0.8, 0.6);
    gluSphere(gluNewQuadric(), 0.5, 50, 50);

    glPushMatrix();
    glTranslatef(0, 0.3, 0);
    glRotatef(-90, 1, 0, 0);
      glColor3f(0.8, 0.2, 0.1);
      glutSolidCone(0.7, 0.7, 50, 1);
    glPopMatrix();
  glPopMatrix();
}

void Wb::draw_wing(float angle) {
  glPushMatrix();
  glRotatef(angle, 0, 1, 0);
    glPushMatrix();
    glTranslatef(0, 0.4, -0.8);
    glRotatef(-35, 1, 0, 0);
    glScalef(0.1, 2, 1);
      glColor3f(0, 0.7, 0.2);
      glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, -0.2, -0.8);
    glRotatef(-35, 1, 0, 0);
    glScalef(0.1, 1, 1.2);
      glColor3f(0, 0.7, 0.2);
      glutSolidCube(1);
    glPopMatrix();
  glPopMatrix();
}

void Wb::draw_pet() {
  glPushMatrix();
    draw_pet_head_top();
    draw_pet_eyes();
    draw_pet_head_bottom();
    draw_pet_mouth(p_count);
  glPopMatrix();
}

void Wb::draw_pet_head_top() {
  glPushMatrix();
  glTranslatef(0, 1, 0);
  glScalef(0.8, 0.5, 0.1);
    glColor3f(1, 1, 1);
    glutSolidCube(1);
  glPopMatrix();
}

void Wb::draw_pet_eyes() {
  glPushMatrix();
  glTranslatef(-0.32, 0.7, 0);
  glScalef(0.15, 0.2, 0.1);
    glColor3f(1, 1, 1);
    glutSolidCube(1);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0, 0.7, 0);
  glScalef(0.15, 0.2, 0.1);
    glColor3f(1, 1, 1);
    glutSolidCube(1);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0.32, 0.7, 0);
  glScalef(0.15, 0.2, 0.1);
    glColor3f(1, 1, 1);
    glutSolidCube(1);
  glPopMatrix();
}

void Wb::draw_pet_head_bottom() {
  glPushMatrix();
  glTranslatef(0, 0.5, 0);
  glScalef(0.8, 0.2, 0.1);
    glColor3f(1, 1, 1);
    glutSolidCube(1);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0, 0.4, 0);
  glScalef(0.4, 0.2, 0.1);
    glColor3f(1, 1, 1);
    glutSolidCube(1);
  glPopMatrix();
}

void Wb::draw_pet_mouth(float t) {
  glPushMatrix();
  glTranslatef(0, t, 0);
  glScalef(0.4, 0.1, 0.1);
    glColor3f(1, 1, 1);
    glutSolidCube(1);
  glPopMatrix();
}
