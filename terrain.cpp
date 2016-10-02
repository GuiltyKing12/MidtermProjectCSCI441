#include "terrain.h"

Terrain::Terrain() {
  load_points("terrain_pts.csv");
}

Terrain::Terrain(std::string fn) {
  load_points(fn);
}

void Terrain::draw() {
  float step = 0.01;

  glDisable(GL_LIGHTING);

  glPushMatrix();
    glColor3f(1, 1, 1);
    glBegin(GL_LINES);
      for (float z = 0.0; z < 1.0; z += step) {
        for (float x = 0.0; x < 1.0; x += step) {
          Point p = bez_patch(x, z);
          Point px = bez_patch(x + step, z);
          Point pz = bez_patch(x, z + step);

          glVertex3f(p.x, p.y, p.z);
          glVertex3f(px.x, px.y, px.z);
          glVertex3f(p.x, p.y, p.z);
          glVertex3f(pz.x, pz.y, pz.z);
        }
      }
    glEnd();
  glPopMatrix();

  glEnable(GL_LIGHTING);
}

void Terrain::load_points(std::string fn) {
  control_points = new Point*[4];
  for (int i = 0; i < 4; i += 1) {
    control_points[i] = new Point[4];
  }

  std::ifstream infile(fn.c_str());
  std::string line;
  int index = 0;
  while (infile) {
    if (!getline(infile, line)) {
      break;
    }

    float x = atof(line.substr(0, line.find_first_of(',')).c_str());
    float y = atof(line.substr(line.find_first_of(',') + 1, line.find_last_of(',') - line.find_first_of(',') - 1).c_str());
    float z = atof(line.substr(line.find_last_of(',') + 1).c_str());

    Point p(x, y, z);
    control_points[index / 4][index % 4] = p;
    index += 1;
  }
}

Point Terrain::bez_patch(float x, float z) {
  return bez_curve(z,
    bez_curve(x,
      control_points[0][0], control_points[0][1],
      control_points[0][2], control_points[0][3]),
    bez_curve(x,
      control_points[1][0], control_points[1][1],
      control_points[1][2], control_points[1][3]),
    bez_curve(x,
      control_points[2][0], control_points[2][1],
      control_points[2][2], control_points[2][3]),
    bez_curve(x,
      control_points[3][0], control_points[3][1],
      control_points[3][2], control_points[3][3]));
}

Point Terrain::bez_curve(float t, Point p0, Point p1, Point p2, Point p3) {
  float t2 = t * t;
  float t3 = t2 * t;
  float mt = 1.0 - t;
  float mt2 = mt * mt;
  float mt3 = mt2 * mt;

  return p0 * mt3 + p1 * (3.0 * mt2 * t) + p2 * (3.0 * mt * t2) + p3 * t3;
}
