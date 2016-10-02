#include "terrain.h"

Terrain::Terrain() {
  load_points("terrain_pts.csv");
}

Terrain::Terrain(std::string fn) {
  load_points(fn);
}

void Terrain::draw() {
  float step = 0.01;

  glPushMatrix();
    glColor3f(0.1, 0.8, 0.1);
    glBegin(GL_TRIANGLES);
      for (float z = 0.0; z < side_length; z += step) {
        for (float x = 0.0; x < side_length; x += step) {
          Point p_tl = bez_patch((int)x, (int)z, x, z);
          Point p_tr = bez_patch((int)(x + step), (int)z, x + step, z);
          Point p_bl = bez_patch((int)x, (int)(z + step), x, z + step);
          Point p_br = bez_patch((int)(x + step), (int)(z + step), x + step, z + step);

          vertex(p_tl);
          vertex(p_bl);
          vertex(p_br);

          vertex(p_tl);
          vertex(p_br);
          vertex(p_tr);
        }
      }
    glEnd();
  glPopMatrix();
}

void Terrain::load_points(std::string fn) {
  std::ifstream infile(fn.c_str());
  std::string line;
  getline(infile, line);
  side_length = atoi(line.c_str());

  control_points = new Point***[side_length];
  for (int i = 0; i < side_length; i += 1) {
    control_points[i] = new Point**[side_length];
    for (int j = 0; j < side_length; j += 1) {
      control_points[i][j] = new Point*[4];
      for (int k = 0; k < 4; k += 1) {
        control_points[i][j][k] = new Point[4];
      }
    }
  }

  int index = 0;
  while (infile) {
    if (!getline(infile, line)) {
      break;
    }

    float x = atof(line.substr(0, line.find_first_of(',')).c_str());
    float y = atof(line.substr(line.find_first_of(',') + 1, line.find_last_of(',') - line.find_first_of(',') - 1).c_str());
    float z = atof(line.substr(line.find_last_of(',') + 1).c_str());

    Point p(x, y, z);
    int inner_index = index % 16;
    int outer_index = index / 16;
    control_points[outer_index / side_length][outer_index % side_length][inner_index / 4][inner_index % 4] = p;
    index += 1;
  }
}

void Terrain::vertex(Point p) {
  glVertex3f(p.x, p.y, p.z);
}

Point Terrain::bez_patch(int ox, int oz, float x, float z) {
  if (ox == side_length) { 
    ox -= 1;
  }
  if (oz == side_length) {
    oz -= 1;
  }

  return bez_curve(z,
    bez_curve(x,
      control_points[oz][ox][0][0], control_points[oz][ox][0][1],
      control_points[oz][ox][0][2], control_points[oz][ox][0][3]),
    bez_curve(x,
      control_points[oz][ox][1][0], control_points[oz][ox][1][1],
      control_points[oz][ox][1][2], control_points[oz][ox][1][3]),
    bez_curve(x,
      control_points[oz][ox][2][0], control_points[oz][ox][2][1],
      control_points[oz][ox][2][2], control_points[oz][ox][2][3]),
    bez_curve(x,
      control_points[oz][ox][3][0], control_points[oz][ox][3][1],
      control_points[oz][ox][3][2], control_points[oz][ox][3][3]));
}

Point Terrain::bez_curve(float t, Point p0, Point p1, Point p2, Point p3) {
  float t2 = t * t;
  float t3 = t2 * t;
  float mt = 1.0 - t;
  float mt2 = mt * mt;
  float mt3 = mt2 * mt;

  return p0 * mt3 + p1 * (3.0 * mt2 * t) + p2 * (3.0 * mt * t2) + p3 * t3;
}
