#include "track.h"

Track::Track() {
  load_points("terrain_pts.csv");
}

Track::Track(std::string fn) {
  load_points(fn);
}

void Track::draw() {
  int res = 100;

  glPushMatrix();
    glColor3f(1, 1, 0);
    glBegin(GL_LINES);
      for (int t = 0; t < segments * res; t += 1) {
        vertex(get_point((float)t / res));
        vertex(get_point((float)(t + 1) / res));
      }
    glEnd();
  glPopMatrix();
}

void Track::load_points(std::string fn) {
  std::ifstream infile(fn.c_str());
  std::string line;
  getline(infile, line);
  int patch_sl = atoi(line.c_str());
  for (int i = 0; i < patch_sl * patch_sl * 16; i += 1) {
    getline(infile, line);
  }

  getline(infile, line);
  segments = atoi(line.c_str());

  control_points = new Point*[segments];
  for (int i = 0; i < segments; i += 1) {
    control_points[i] = new Point[4];
  }

  for (int i = 0; i < segments * 4; i += 1) {
    if (!getline(infile, line)) {
      break;
    }

    float x = atof(line.substr(0, line.find_first_of(',')).c_str());
    float y = atof(line.substr(line.find_first_of(',') + 1, line.find_last_of(',') - line.find_first_of(',') - 1).c_str());
    float z = atof(line.substr(line.find_last_of(',') + 1).c_str());

    Point p(x, y, z);
    int inner_index = i % 4;
    int outer_index = i / 4;
    control_points[outer_index][inner_index] = p;
  }

  infile.close(); 
}

void Track::vertex(Point p) {
  glVertex3f(p.x, p.y, p.z);
}

Point Track::get_point(float t) {
  int o = 0;
  while (t > 1.0) {
    o += 1;
    t -= 1.0;
  }

  return bez_curve(t, 
    control_points[o][0], control_points[o][1],
    control_points[o][2], control_points[o][3]);
}

Point Track::bez_curve(float t, Point p0, Point p1, Point p2, Point p3) {
  float t2 = t * t;
  float t3 = t2 * t;
  float mt = 1.0 - t;
  float mt2 = mt * mt;
  float mt3 = mt2 * mt;

  return p0 * mt3 + p1 * (3.0 * mt2 * t) + p2 * (3.0 * mt * t2) + p3 * t3;
}